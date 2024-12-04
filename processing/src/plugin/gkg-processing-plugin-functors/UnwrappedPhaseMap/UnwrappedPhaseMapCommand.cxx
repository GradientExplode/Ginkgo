#include <gkg-processing-plugin-functors/UnwrappedPhaseMap/UnwrappedPhaseMapCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/PhaseUnwrapperFactory.h>
#include <gkg-processing-signal/PhaseUnwrapperFunction.h>
#include <gkg-processing-signal/PseudoCorrelation.h>
#include <gkg-processing-signal/ZUnwrapping.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/SeparableConvolution3d_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-mask/MorphoFilledMask.h>
#include <gkg-processing-extrapolation/HoleFillingExtrapolator3d_i.h>
#include <gkg-processing-extrapolation/RecursiveExtrapolator3d_i.h>
#include <gkg-processing-extrapolation/FourierExtrapolator3d_i.h>
#include <gkg-processing-extrapolation/PolynomExtrapolator3d_i.h>
#include <gkg-processing-extrapolation/Extrapolator2d_i.h>
#include <gkg-processing-extrapolation/CosinusExtrapolatorFunction.h>
#include <gkg-processing-extrapolation/PolynomExtrapolatorFunction.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>


void extractCoilData(
                    const gkg::Volume< std::complex< float > > & doubleEchoData,
                    gkg::Volume< float >& magnitude1stEcho,
                    gkg::Volume< float >& magnitude2ndEcho,
                    gkg::Volume< float >& phaseDifference,
                    int32_t coilCount )
{

  try
  {

    int32_t c;
    int32_t x, y, z;

    int32_t sizeX = doubleEchoData.getSizeX();
    int32_t sizeY = doubleEchoData.getSizeY();
    int32_t sizeZ = doubleEchoData.getSizeZ();

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          // taking magnitude of 1st and 2nd echo
          magnitude1stEcho( x, y, z ) = 0.0;
          magnitude2ndEcho( x, y, z ) = 0.0;
          for ( c = 0; c < coilCount; c++ )
          {

            magnitude1stEcho( x, y, z ) +=
                          std::norm( doubleEchoData( x, y, z, c ) );
            magnitude2ndEcho( x, y, z ) +=
                          std::norm( doubleEchoData( x, y, z, c + coilCount ) );

          }
          magnitude1stEcho( x, y, z ) =
                          std::sqrt( magnitude1stEcho( x, y, z ) );
          magnitude2ndEcho( x, y, z ) =
                          std::sqrt( magnitude2ndEcho( x, y, z ) );


          // taking phase difference 2nd echo - 1st echo
          phaseDifference( x, y, z ) = 0.0;
          std::complex< float > sum( 0.0, 0.0 );
          for ( c = 0; c < coilCount; c++ )
          {

            if ( magnitude1stEcho( x, y, z ) < 1e-6 )
            {

              sum +=
                doubleEchoData( x, y, z, c + coilCount );

            }
            else
            {

              sum +=
                std::conj( doubleEchoData( x, y, z, c ) ) *
                doubleEchoData( x, y, z, c + coilCount ) /
                magnitude1stEcho( x, y, z );

            }

          }
          phaseDifference( x, y, z ) = std::arg( sum );

        }

      }

    }

  }
  GKG_CATCH( "void extractCoilData( "
             "const gkg::Volume< std::complex< float > > & doubleEchoData, "
             "gkg::Volume< float >& magnitude1stEcho, "
             "gkg::Volume< float >& magnitude2ndEcho, "
             "gkg::Volume< float >& phaseDifference, "
             "int coilCount )" );

}



void getGaussianKernel( std::vector< float >& kernel, float sigma, 
                        int32_t radius )
{

  try
  {

    kernel = std::vector< float >( 2 * radius + 1 );
    float sum=0.0, value=0.0;
    int32_t k;
    for( k = -radius; k <= radius; k++ )
    {

      if ( sigma > 1e-6 )
      {

        value = std::exp( -( k * k ) / ( 2.0 * sigma * sigma ) );
      }
      else
      {

        if ( k == 0 )
        {

          value = 1.0;

        }
        else
        {

          value = 0.0;

        }

      }
      kernel[ k + radius + 1 ] = value;
      sum += value;

    }
    for ( k = 0; k < 2 * radius + 1; k++ )
    {

      kernel[ k ] /= sum;

    }

  }
  GKG_CATCH( "void getGaussianKernel( std::vector< float >& kernel, "
             "float sigma, int radius )" );

}


void regularize( const gkg::Volume< float >& unwrappedPhaseDifference,
                 const gkg::Volume< uint8_t >& qualityMask,
                 const gkg::Volume< uint8_t >& morphoFilledMask,
                 gkg::Volume< float >& regularizedPhaseMap,
                 int32_t degree,
                 const std::string& regularizationMethod,
                 bool verbose )
{

  try
  {

    gkg::EqualToFunction< uint8_t > equalToFunction( 1U );

    if ( regularizationMethod == "none" )
    {

      regularizedPhaseMap = unwrappedPhaseDifference;

    }
    else
    {

      if ( regularizationMethod == "fill" )
      {

        if ( verbose )
        {

          std::cout << "hole filling regularizing : " << std::flush;

        }
        gkg::HoleFillingExtrapolator3d< float, uint8_t, float >
          extrapolator;
        extrapolator.extrapolate( unwrappedPhaseDifference,
                                  qualityMask,
                                  morphoFilledMask,
                                  equalToFunction,
                                  regularizedPhaseMap,
                                  verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else if ( regularizationMethod == "median" )
      {

        if ( verbose )
        {

          std::cout << "median regularizing : " << std::flush;

        }
        gkg::MedianFilter< std::vector< float >, float > medianFilter;
        gkg::RecursiveExtrapolator3d< float, uint8_t, float >
          extrapolator( medianFilter, gkg::Neighborhood3d::Neighborhood3d_26 );
        extrapolator.extrapolate( unwrappedPhaseDifference,
                                  qualityMask,
                                  morphoFilledMask,
                                  equalToFunction,
                                  regularizedPhaseMap,
                                  verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else if ( regularizationMethod == "polynom2d" )
      {

        if ( verbose )
        {

          std::cout << "2D polynomial regularizing : " << std::flush;

        }
        gkg::PolynomExtrapolatorFunction polynomExtrapolatorFunction;
        gkg::Extrapolator2d< float, uint8_t, float >
          extrapolator( degree, polynomExtrapolatorFunction );
        extrapolator.extrapolate( unwrappedPhaseDifference,
                                  qualityMask,
                                  morphoFilledMask,
                                  equalToFunction,
                                  regularizedPhaseMap,
                                  verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else if ( regularizationMethod == "fourier2d" )
      {

        if ( verbose )
        {

          std::cout << "2D fourier regularizing : " << std::flush;

        }
        gkg::CosinusExtrapolatorFunction cosinusExtrapolatorFunction;
        gkg::Extrapolator2d< float, uint8_t, float >
          extrapolator( degree, cosinusExtrapolatorFunction );
        extrapolator.extrapolate( unwrappedPhaseDifference,
                                  qualityMask,
                                  morphoFilledMask,
                                  equalToFunction,
                                  regularizedPhaseMap,
                                  verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else if ( regularizationMethod == "polynom3d" )
      {

        if ( verbose )
        {

          std::cout << "3D polynomial regularizing : " << std::flush;

        }
        gkg::PolynomExtrapolator3d< float, uint8_t, float >
          extrapolator( degree );
        extrapolator.extrapolate( unwrappedPhaseDifference,
                                  qualityMask,
                                  morphoFilledMask,
                                  equalToFunction,
                                  regularizedPhaseMap,
                                  verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else if ( regularizationMethod == "fourier3d" )
      {

        if ( verbose )
        {

          std::cout << "3D fourier regularizing : " << std::flush;

        }
        gkg::FourierExtrapolator3d< float, uint8_t, float >
          extrapolator( degree );
        extrapolator.extrapolate( unwrappedPhaseDifference,
                                  qualityMask,
                                  morphoFilledMask,
                                  equalToFunction,
                                  regularizedPhaseMap,
                                  verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

    }

    if ( verbose )
    {

      std::cout << "masking regularization : " << std::flush;

    }
    int32_t sizeX = morphoFilledMask.getSizeX();
    int32_t sizeY = morphoFilledMask.getSizeY();
    int32_t sizeZ = morphoFilledMask.getSizeZ();

    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;

    if ( morphoFilledMask.getHeader().hasAttribute( "resolutionX" ) &&
         morphoFilledMask.getHeader().hasAttribute( "resolutionY" ) &&
         morphoFilledMask.getHeader().hasAttribute( "resolutionZ" ) )
    {

      morphoFilledMask.getHeader().getAttribute( "resolutionX", resolutionX );
      morphoFilledMask.getHeader().getAttribute( "resolutionY", resolutionY );
      morphoFilledMask.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }

    if ( verbose )
    {

      std::cout << "dilating mask -> " << std::flush;

    }
    float dilationRadius = 3.0 * std::min( std::min( resolutionX,
                                                     resolutionY ),
                                           resolutionZ );
    gkg::Dilation< uint8_t, uint8_t > dilation( dilationRadius,
                                                equalToFunction,
                                                1U,
                                                0U );

    gkg::Volume< uint8_t > dilatedMask;
    dilation.dilate( morphoFilledMask, dilatedMask, verbose );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 17 ) << std::flush;

    }

    if ( verbose )
    {

      std::cout << "smoothing ->" << std::flush;

    }
    gkg::BoundingBox< int32_t > bBox( 0, sizeX - 1,
                                      0, sizeY - 1,
                                      0, sizeZ - 1 );
    int32_t x, y, z, nx, ny, nz;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {


        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( !dilatedMask( x, y, z ) )
          {

            regularizedPhaseMap( x, y, z ) = 0.0;

          }
          else if ( dilatedMask( x, y, z ) && !morphoFilledMask( x, y, z ) )
          {

            int32_t count = 0;
            float sum = 0;
            for ( nz = z - 3; nz <= z + 3; nz++ )
            {

              for ( ny = y - 3; ny <= y + 3; ny++ )
              {

                for ( nx = x - 3; nx <= x + 3; nx++ )
                {

                  if ( bBox.contains( nx, ny, nz ) )
                  {

                    if ( morphoFilledMask( nx, ny, nz ) )
                    {

                      sum += regularizedPhaseMap( nx, ny, nz );

                    }
                    ++ count;

                  }

                }

              }

            }
            regularizedPhaseMap( x, y, z ) = sum / ( float )count;

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 33 ) << std::flush;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void regularize( "
             "const gkg::Volume< float >& unwrappedPhaseDifference, "
             "const gkg::Volume< unsigned char >& qualityMask, "
             "const gkg::Volume< unsigned char >& morphoFilledMask, "
             "gkg::Volume< float >& regularizedPhaseMap, "
             "int degree, "
             "const std::string& regularizationMethod, "
             "bool verbose )" );

}


//
//   UnwrappedPhaseMapCommand
//


gkg::UnwrappedPhaseMapCommand::UnwrappedPhaseMapCommand( int32_t argc,
                                 			 char* argv[],
                                 			 bool loadPlugin,
                                 			 bool removeFirst )
                             : gkg::Command( argc, argv,
                                             loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::UnwrappedPhaseMapCommand::UnwrappedPhaseMapCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::UnwrappedPhaseMapCommand::UnwrappedPhaseMapCommand(
                            const std::string& fileNameDoubleEcho,
    			    const std::string& fileNameMagnitude1stEcho,
    			    const std::string& fileNameMagnitude2ndEcho,
    			    const std::string& fileNameWrappedPhaseDifference,
    			    const std::string& fileNameMorphoFilledMask,
    			    float closingRadiusFactor,
    			    double noisePosition,
    			    double noiseRatio,
    			    int32_t thrown,
    			    int32_t kept,
    			    const std::string& fileNameQualityMap,
    			    const std::string& fileNameQualityMask,
    			    float qualityThreshold,
    			    const std::string& fileNameUnwrappedPhaseDifference,
    			    const std::string& fileNameRegularizedPhaseMap,
    			    int32_t degree,
    			    float preFilteringSigma,
    			    const std::string& regularizationMethod,
    			    bool removeRampFlag,
    			    bool zLineUnwrapping,
    			    bool verbose )
                             : gkg::Command()
{

  try
  {

    execute( fileNameDoubleEcho, fileNameMagnitude1stEcho, 
             fileNameMagnitude2ndEcho, fileNameWrappedPhaseDifference,
             fileNameMorphoFilledMask, closingRadiusFactor, noisePosition,
             noiseRatio, thrown, kept, fileNameQualityMap, fileNameQualityMask,
             qualityThreshold, fileNameUnwrappedPhaseDifference,
             fileNameRegularizedPhaseMap, degree, preFilteringSigma, 
             regularizationMethod, removeRampFlag, zLineUnwrapping, verbose );

  }
  GKG_CATCH( "gkg::UnwrappedPhaseMapCommand::UnwrappedPhaseMapCommand( "
             "const std::string& fileNameDoubleEcho, "
    	     "const std::string& fileNameMagnitude1stEcho, "
    	     "const std::string& fileNameMagnitude2ndEcho, "
    	     "const std::string& fileNameWrappedPhaseDifference, "
    	     "const std::string& fileNameMorphoFilledMask, "
    	     "float closingRadiusFactor, double noisePosition, "
    	     "double noiseRatio, int32_t thrown, int32_t kept, "
    	     "const std::string& fileNameQualityMap, "
    	     "const std::string& fileNameQualityMask, float qualityThreshold, "
    	     "const std::string& fileNameUnwrappedPhaseDifference, "
    	     "const std::string& fileNameRegularizedPhaseMap, "
    	     "int32_t degree, float preFilteringSigma, "
    	     "const std::string& regularizationMethod, "
    	     "bool removeRampFlag, bool zLineUnwrapping, bool verbose )" );

}


gkg::UnwrappedPhaseMapCommand::UnwrappedPhaseMapCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDoubleEcho );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameMagnitude1stEcho );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameMagnitude2ndEcho );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameWrappedPhaseDifference );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameMorphoFilledMask );
    DECLARE_FLOATING_PARAMETER( parameters, float, closingRadiusFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, noisePosition );
    DECLARE_FLOATING_PARAMETER( parameters, double, noiseRatio );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, thrown );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, kept );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameQualityMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameQualityMask );
    DECLARE_FLOATING_PARAMETER( parameters, float, qualityThreshold );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameUnwrappedPhaseDifference );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameRegularizedPhaseMap );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, degree );
    DECLARE_FLOATING_PARAMETER( parameters, float, preFilteringSigma );
    DECLARE_STRING_PARAMETER( parameters, std::string, regularizationMethod );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, removeRampFlag );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, zLineUnwrapping );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameDoubleEcho, fileNameMagnitude1stEcho, 
             fileNameMagnitude2ndEcho, fileNameWrappedPhaseDifference,
             fileNameMorphoFilledMask, closingRadiusFactor, noisePosition,
             noiseRatio, thrown, kept, fileNameQualityMap, fileNameQualityMask,
             qualityThreshold, fileNameUnwrappedPhaseDifference,
             fileNameRegularizedPhaseMap, degree, preFilteringSigma, 
             regularizationMethod, removeRampFlag, zLineUnwrapping, verbose );

  }
  GKG_CATCH( "gkg::UnwrappedPhaseMapCommand::UnwrappedPhaseMapCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::UnwrappedPhaseMapCommand::~UnwrappedPhaseMapCommand()
{
}


std::string gkg::UnwrappedPhaseMapCommand::getStaticName()
{

  try
  {

    return "UnwrappedPhaseMap";

  }
  GKG_CATCH( "std::string gkg::UnwrappedPhaseMapCommand::getStaticName()" );

}


void gkg::UnwrappedPhaseMapCommand::parse()
{

  try
  {

    std::string fileNameDoubleEcho;
    std::string fileNameMagnitude1stEcho;
    std::string fileNameMagnitude2ndEcho;
    std::string fileNameWrappedPhaseDifference;
    std::string fileNameMorphoFilledMask;
    float closingRadiusFactor = 2.0;
    double noisePosition = 0.9;
    double noiseRatio = 0.01;
    int32_t thrown = 20;
    int32_t kept = 100;
    std::string fileNameQualityMap;
    std::string fileNameQualityMask;
    float qualityThreshold = 0.9;
    std::string fileNameUnwrappedPhaseDifference;
    std::string fileNameRegularizedPhaseMap;
    int32_t degree = 2;
    float preFilteringSigma = 0.0;
    std::string regularizationMethod = "median";
    bool removeRampFlag = false;
    bool zLineUnwrapping = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Phase map estimation from double echoes "
                                  "GRE 2D complex acquisition",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Double echo complex file name",
                                 fileNameDoubleEcho );
    application.addSingleOption( "-m1",
                                 "Output 1st echo magnitude name",
                                 fileNameMagnitude1stEcho,
                                 true );
    application.addSingleOption( "-m2",
                                 "Output 2nd echo magnitude name",
                                 fileNameMagnitude2ndEcho,
                                 true );
    application.addSingleOption( "-w",
                                 "Output wrapped phase difference",
                                 fileNameWrappedPhaseDifference,
                                 true );
    application.addSingleOption( "-M",
                                 "Output morphological filled mask",
                                 fileNameMorphoFilledMask,
                                 true );
    application.addSingleOption( "-r",
                                 "Closing radius factor [default=2.0]\n"
                                 "Morphological closing is done with the"
                                 " minimum in-plane resolution times this "
                                 "factor",
                                 closingRadiusFactor,
                                 true );
    application.addSingleOption( "-noisePosition",
                                 "Noise position along x axis (default=0.9)",
                                 noisePosition,
                                 true );
    application.addSingleOption( "-noiseRatio",
                                 "Noise ratio (default=0.01)",
                                 noiseRatio,
                                 true );
    application.addSingleOption( "-thrown",
                                 "Maximum thrown count (default=20)",
                                 thrown,
                                 true );
    application.addSingleOption( "-kept",
                                 "Maximum kept (default=100)",
                                 kept,
                                 true );
    application.addSingleOption( "-q",
                                 "Output quality map name",
                                 fileNameQualityMap,
                                 true );
    application.addSingleOption( "-Q",
                                 "Output binarized quality mask",
                                 fileNameQualityMask,
                                 true );
    application.addSingleOption( "-t",
                                 "Quality threshold [default=0.9]",
                                 qualityThreshold,
                                 true );
    application.addSingleOption( "-u",
                                 "Unwrapped phase map",
                                 fileNameUnwrappedPhaseDifference,
                                 true );
    application.addSingleOption( "-ur",
                                 "Regularized unwrapped phase map",
                                 fileNameRegularizedPhaseMap,
                                 true );
    application.addSingleOption( "-d",
                                 "Extrapolation degree [default=2]",
                                 degree,
                                 true );
    application.addSingleOption( "-preFilteringSigma",
                                 "Pre-filtering data with gaussian filter "
                                 "in x-y plane (default=0.0->no smoothing)",
                                 preFilteringSigma,
                                 true );
    application.addSingleOption( "-R",
                                 "Regularization method :\n"
                                 "- none\n"
                                 "- fill -> hole filling\n"
                                 "- median -> median recursive filling\n"
                                 "- polynom2d -> hole filling + 2D poly. fit\n"
                                 "- fourier2d -> hole fill. + 2D fourier. fit\n"
                                 "- polynom3d -> hole filling + 3D poly. fit\n"
                                 "- fourier3d -> hole fill. + 3D fourier. fit\n"
                                 "[default=median];\n",
                                 regularizationMethod,
                                 true );
    application.addSingleOption( "-removeRamp",
                                 "Remove ramp(s) according to x, y, and z axis",
                                 removeRampFlag,
                                 true );
    application.addSingleOption( "-zLineUnwrapping",
                                 "Z line unwrapping after Z slice unwrapping",
                                 zLineUnwrapping,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameDoubleEcho, fileNameMagnitude1stEcho, 
             fileNameMagnitude2ndEcho, fileNameWrappedPhaseDifference,
             fileNameMorphoFilledMask, closingRadiusFactor, noisePosition,
             noiseRatio, thrown, kept, fileNameQualityMap, fileNameQualityMask,
             qualityThreshold, fileNameUnwrappedPhaseDifference,
             fileNameRegularizedPhaseMap, degree, preFilteringSigma, 
             regularizationMethod, removeRampFlag, zLineUnwrapping, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::UnwrappedPhaseMapCommand::parse()" );

}


void gkg::UnwrappedPhaseMapCommand::execute(
                  	    const std::string& fileNameDoubleEcho,
    			    const std::string& fileNameMagnitude1stEcho,
    			    const std::string& fileNameMagnitude2ndEcho,
    			    const std::string& fileNameWrappedPhaseDifference,
    			    const std::string& fileNameMorphoFilledMask,
    			    float closingRadiusFactor,
    			    double noisePosition,
    			    double noiseRatio,
    			    int32_t thrown,
    			    int32_t kept,
    			    const std::string& fileNameQualityMap,
    			    const std::string& fileNameQualityMask,
    			    float qualityThreshold,
    			    const std::string& fileNameUnwrappedPhaseDifference,
    			    const std::string& fileNameRegularizedPhaseMap,
    			    int32_t degree,
    			    float preFilteringSigma,
    			    const std::string& regularizationMethod,
    			    bool removeRampFlag,
    			    bool zLineUnwrapping,
    			    bool verbose )
{

  try
  {


    // sanity checks
    if ( preFilteringSigma < 0 )
    {

      throw std::runtime_error(
                      "Gaussian pre-filtering std deviation must be positive" );

    }


    if ( ( regularizationMethod != "none" ) &&
         ( regularizationMethod != "fill" ) &&
         ( regularizationMethod != "median" ) &&
         ( regularizationMethod != "polynom2d" ) &&
         ( regularizationMethod != "fourier2d" ) &&
         ( regularizationMethod != "polynom3d" ) &&
         ( regularizationMethod != "fourier3d" ) )
    {

      throw std::runtime_error( "wrong regularization method" );

    }
         

    // reading double echoes data
    if ( verbose )
    {

      std::cout << "reading '" << fileNameDoubleEcho << "' : " << std::flush;

    }
    gkg::Volume< std::complex< float > > doubleEchoData;
    gkg::Reader::getInstance().read( fileNameDoubleEcho, doubleEchoData );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // collecting coil count information
    if ( verbose )
    {

      std::cout << "coil count : " << std::flush;

    }

    int32_t coilCount = 0;
    if ( doubleEchoData.getHeader().hasAttribute(
                                       "pool_header.rdb_hdr_image.psd_iname" ) )
    {

      std::string psdName = "";
      doubleEchoData.getHeader().getAttribute(
                               "pool_header.rdb_hdr_image.psd_iname", psdName );
      if ( ( psdName != "GkgB0Map" ) &&
           ( psdName != "NmrB0Map" ) )
      {

        throw std::runtime_error( "not a GKG B0 pulse sequence");

      }

      // collecting coil information
      double start_rcv, stop_rcv;
      doubleEchoData.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
      doubleEchoData.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
      coilCount = ( int32_t )( ( int32_t )( stop_rcv + 0.5 ) -
                               ( int32_t )( start_rcv + 0.5 ) + 1 );

    }
    else
    {

      // on 3T, we know that it is a monocanal RF chain
      coilCount = 1;

    }

    if ( verbose )
    {

      std::cout << coilCount << std::endl;

    }

    if ( verbose )
    {

      std::cout << "extracting m1, m2, phi : " << std::flush;

    }

    gkg::Volume< float > magnitude1stEcho( doubleEchoData.getSizeX(),
                                           doubleEchoData.getSizeY(),
                                           doubleEchoData.getSizeZ() );
    magnitude1stEcho.getHeader() = doubleEchoData.getHeader();
    magnitude1stEcho.getHeader()[ "sizeT" ] = 1;
    magnitude1stEcho.getHeader()[ "item_type" ] = gkg::TypeOf<float>::getName();

    gkg::Volume< float > magnitude2ndEcho( doubleEchoData.getSizeX(),
                                           doubleEchoData.getSizeY(),
                                           doubleEchoData.getSizeZ() );
    magnitude2ndEcho.getHeader() = doubleEchoData.getHeader();
    magnitude2ndEcho.getHeader()[ "sizeT" ] = 1;
    magnitude2ndEcho.getHeader()[ "item_type" ] = gkg::TypeOf<float>::getName();

    gkg::Volume< float > phaseDifference( doubleEchoData.getSizeX(),
                                          doubleEchoData.getSizeY(),
                                          doubleEchoData.getSizeZ() );
    phaseDifference.getHeader() = doubleEchoData.getHeader();
    phaseDifference.getHeader()[ "sizeT" ] = 1;
    phaseDifference.getHeader()[ "item_type" ] = gkg::TypeOf<float>::getName();

    extractCoilData( doubleEchoData,
                     magnitude1stEcho,
                     magnitude2ndEcho,
                     phaseDifference,
                     coilCount );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // prefiltering wrapped phase difference
    if ( preFilteringSigma > 1e-10 )
    {

      if ( verbose )
      {

        std::cout << "prefiltering wrapped map (sigma="
                  << preFilteringSigma << "mm : " << std::flush;

      }

      double resolutionX = 1.0;
      if ( phaseDifference.getHeader().hasAttribute( "resolutionX" ) )
      {

        phaseDifference.getHeader().getAttribute( "resolutionX", resolutionX );

      }
      double resolutionY = 1.0;
      if ( phaseDifference.getHeader().hasAttribute( "resolutionY" ) )
      {

        phaseDifference.getHeader().getAttribute( "resolutionY", resolutionY );

      }

      float sigmaX, sigmaY;
      sigmaX = preFilteringSigma / ( float )resolutionX;
      sigmaY = preFilteringSigma / ( float )resolutionY;
      int32_t radiusX = ( ( int32_t )( sigmaX - 0.001 ) ) * 2 + 3;
      int32_t radiusY = ( ( int32_t )( sigmaY - 0.001 ) ) * 2 + 3;

      std::vector< float > kernelX;
      std::vector< float > kernelY;
      std::vector< float > kernelZ( 1, 1.0 );

      getGaussianKernel( kernelX, sigmaX, radiusX );
      getGaussianKernel( kernelY, sigmaY, radiusY );

      gkg::SeparableConvolution3d< float, float, float > convolution;
      convolution.convolve( phaseDifference,
                            kernelX,
                            kernelY,
                            kernelZ,
                            phaseDifference );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // saving magnitude of the 1st echo
    if ( !fileNameMagnitude1stEcho.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving 1st echo magnitude : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameMagnitude1stEcho,
                                        magnitude1stEcho );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    // saving magnitude of the 2nd echo
    if ( !fileNameMagnitude2ndEcho.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving 2nd echo magnitude : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameMagnitude2ndEcho,
                                        magnitude2ndEcho );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // processing morphological filled mask
    gkg::Volume< uint8_t > morphoFilledMask;
    if ( !fileNameMorphoFilledMask.empty() ||
         !fileNameRegularizedPhaseMap.empty() )
    {

      if ( verbose )
      {

        std::cout << "processing morphological filled mask : " << std::flush;

      }
      double resolutionX = 0.0, resolutionY = 0.0;
      if ( magnitude2ndEcho.getHeader().hasAttribute( "resolutionX" ) &&
           magnitude2ndEcho.getHeader().hasAttribute( "resolutionY" ) )
      {

        magnitude2ndEcho.getHeader().getAttribute( "resolutionX", resolutionX );
        magnitude2ndEcho.getHeader().getAttribute( "resolutionY", resolutionY );

      }
      else
      {

        throw std::runtime_error( "cannot find X and Y in plane resolution" );

      }
      gkg::MorphoFilledMask< float, uint8_t >
        morphoFilledMaskAlgo( closingRadiusFactor * std::min( resolutionX,
                                                              resolutionY ),
                              noisePosition,
                              noiseRatio,
                              thrown,
                              kept );
      morphoFilledMaskAlgo.getMask( magnitude2ndEcho, morphoFilledMask,
                                    verbose );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( !fileNameMorphoFilledMask.empty() )
      {

        if ( verbose )
        {

          std::cout << "saving morphological filled mask : " << std::flush;

        }
        gkg::Writer::getInstance().write( fileNameMorphoFilledMask,
                                          morphoFilledMask );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

    }

    // saving wrapped phase difference
    if ( !fileNameWrappedPhaseDifference.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving wrapped phase difference : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameWrappedPhaseDifference,
                                        phaseDifference );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // processing quality map
    if ( verbose )
    {

      std::cout << "processing quality map : " << std::flush;

    }
    gkg::Volume< float > qualityMap( phaseDifference.getSizeX(),
                                     phaseDifference.getSizeY(),
                                     phaseDifference.getSizeZ() );
    qualityMap.getHeader() = doubleEchoData.getHeader();
    qualityMap.getHeader()[ "sizeT" ] = 1;
    qualityMap.getHeader()[ "item_type" ] = gkg::TypeOf<float>::getName();                             
    gkg::PseudoCorrelation( phaseDifference, qualityMap, 3 );


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( !fileNameQualityMap.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving quality map : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameQualityMap,
                                        qualityMap );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // processing quality mask
    if ( verbose )
    {

      std::cout << "processing quality mask : " << std::flush;

    }
    gkg::GreaterOrEqualToFunction< float > 
     greaterOrEqualToFunction( qualityThreshold );
    gkg::Binarizer< gkg::Volume< float >, gkg::Volume< uint8_t > >
      binarizer( greaterOrEqualToFunction, 1, 0 );
    gkg::Volume< uint8_t > qualityMask( qualityMap.getSizeX(),
                                        qualityMap.getSizeY(),
                                        qualityMap.getSizeZ() );
    qualityMask.getHeader() = qualityMap.getHeader();
    qualityMask.getHeader()[ "item_type" ] = gkg::TypeOf< uint8_t >::getName();
    binarizer.binarize( qualityMap, qualityMask );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( !fileNameQualityMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving quality mask : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameQualityMask,
                                        qualityMask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // unwrapping difference phase map (2D multigrid algorithm)
    if ( verbose )
    {

      std::cout << "unwrapping phase difference : " << std::flush;

    }
    gkg::PhaseUnwrapper< float >*
      phaseUnwrapper = gkg::PhaseUnwrapperFactory< float >::getInstance().
        createPhaseUnwrapper( "weighted-full-multigrid-phase-unwrapper" );

    gkg::Volume< float > unwrappedPhaseDifference( phaseDifference );
    phaseUnwrapper->unwrap( unwrappedPhaseDifference, qualityMap, verbose );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // unwrapping phase along z axis
    int32_t referenceSlice = unwrappedPhaseDifference.getSizeZ() / 2;
    gkg::ZUnwrapping( unwrappedPhaseDifference,
                      qualityMap,
                      referenceSlice );
    if ( zLineUnwrapping )
    {

      gkg::ZLineUnwrapping( unwrappedPhaseDifference,
                            qualityMap,
                            referenceSlice );

    }

    // removing ramp(s)
    if ( removeRampFlag )
    {

      if ( verbose )
      {

        std::cout << "removing ramp : " << std::flush;

      }
      gkg::removeRamp( unwrappedPhaseDifference, qualityMap );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // removing constant offset
    if ( verbose )
    {

      std::cout << "removing constant phase offset : " << std::flush;

    }
    gkg::removeOffset( unwrappedPhaseDifference, qualityMask );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // saving unwrapped phase map
    if ( !fileNameUnwrappedPhaseDifference.empty() )
    {

      if ( verbose )
      {

        std::cout << "saving unwrapped phase difference : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameUnwrappedPhaseDifference,
                                        unwrappedPhaseDifference );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // regularizing phase map
    if ( !fileNameRegularizedPhaseMap.empty() )
    {

      gkg::Volume< float > regularizedPhaseMap( unwrappedPhaseDifference );
      regularize( unwrappedPhaseDifference,
                  qualityMask,
                  morphoFilledMask,
                  regularizedPhaseMap,
                  degree,
                  regularizationMethod,
                  verbose );

      if ( verbose )
      {

        std::cout << "saving regularized phase map : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameRegularizedPhaseMap,
                                        regularizedPhaseMap );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::UnwrappedPhaseMapCommand::execute( "
             "const std::string& fileNameDoubleEcho, "
    	     "const std::string& fileNameMagnitude1stEcho, "
    	     "const std::string& fileNameMagnitude2ndEcho, "
    	     "const std::string& fileNameWrappedPhaseDifference, "
    	     "const std::string& fileNameMorphoFilledMask, "
    	     "float closingRadiusFactor, double noisePosition, "
    	     "double noiseRatio, int32_t thrown, int32_t kept, "
    	     "const std::string& fileNameQualityMap, "
    	     "const std::string& fileNameQualityMask, float qualityThreshold, "
    	     "const std::string& fileNameUnwrappedPhaseDifference, "
    	     "const std::string& fileNameRegularizedPhaseMap, "
    	     "int32_t degree, float preFilteringSigma, "
    	     "const std::string& regularizationMethod, "
    	     "bool removeRampFlag, bool zLineUnwrapping, bool verbose )" );

}


RegisterCommandCreator(
             UnwrappedPhaseMapCommand,
    	     DECLARE_STRING_PARAMETER_HELP( fileNameDoubleEcho ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameMagnitude1stEcho ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameMagnitude2ndEcho ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameWrappedPhaseDifference ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameMorphoFilledMask ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( closingRadiusFactor ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( noisePosition ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( noiseRatio ) +
    	     DECLARE_INTEGER_PARAMETER_HELP( thrown ) +
    	     DECLARE_INTEGER_PARAMETER_HELP( kept ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameQualityMap ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameQualityMask ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( qualityThreshold ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameUnwrappedPhaseDifference ) +
    	     DECLARE_STRING_PARAMETER_HELP( fileNameRegularizedPhaseMap ) +
    	     DECLARE_INTEGER_PARAMETER_HELP( degree ) +
    	     DECLARE_FLOATING_PARAMETER_HELP( preFilteringSigma ) +
    	     DECLARE_STRING_PARAMETER_HELP( regularizationMethod ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( removeRampFlag ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( zLineUnwrapping ) +
    	     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
