#include <gkg-dmri-plugin-functors/DwiQSpaceResampler/DwiQSpaceResamplerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/SphericalQSpaceSampling.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>


#define EPSILON 1e-6


//
// void resamplingSphericalHarmonicsSingleShell()
//

static void resamplingSphericalHarmonicsSingleShell( 
                     const gkg::Volume< float >& t2,
                     gkg::Volume< float >& dw,
                     const gkg::Volume< int16_t >& /*mask*/,
                     gkg::RCPointer< gkg::QSpaceSampling > inputQSpaceSampling,
                     gkg::RCPointer< gkg::QSpaceSampling > outputQSpaceSampling,
	             const std::vector< double >& resamplingParameters,
                     bool verbose )
{

  try
  {

    if ( !resamplingParameters.empty() && ( resamplingParameters.size() != 2U ) )
    {
    
      throw std::runtime_error( "bad resampling parameter count" );
    
    }

    int32_t maximumSphericalHarmonicOrder = 4;
    double laplaceBeltramiFactor = 0.006;
    if ( !resamplingParameters.empty() )
    {
    
      maximumSphericalHarmonicOrder = ( int32_t )( resamplingParameters[ 0 ] +
                                                   0.5 );
      laplaceBeltramiFactor = resamplingParameters[ 1 ];
    
    }

    // processing maximum j according to maximum order
    int32_t N = ( ( maximumSphericalHarmonicOrder + 1 ) *
                  ( maximumSphericalHarmonicOrder + 2 ) ) / 2;
  
    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building Laplace-Beltrami smoothness matrix for regularization
    gkg::Matrix L( N, N );
    L.setZero();
    double sphericalLaplacian = 0;
    int32_t n, l, m;
    for ( n = 0; n < N; n++ )
    {

      factory->getSymmetricalSphericalHarmonicLM( n, l, m );
      sphericalLaplacian = - l * ( l + 1 );
      L( n, n ) = sphericalLaplacian * sphericalLaplacian;

    }

    // collecting input and output orientation count
    int32_t inputOrientationCount = inputQSpaceSampling->getCount();
    int32_t outputOrientationCount = outputQSpaceSampling->getCount();

    // collecting size of the 3D volume
    int32_t sizeX = dw.getSizeX();
    int32_t sizeY = dw.getSizeY();
    int32_t sizeZ = dw.getSizeZ();

    // reallocating DW volume
    dw.reallocate( sizeX, sizeY, sizeZ, outputOrientationCount );

    // building matrix of spherical harmonics on input orientation(s)
    gkg::Vector3d< float > orientation;
    double radial = 0.0;
    double theta = 0.0;
    double phi = 0.0;

    gkg::Matrix B( inputOrientationCount, N );

    for ( m = 0; m < inputOrientationCount; m++ )
    {

      orientation = inputQSpaceSampling->getOrientation( m );
      factory->getCartesianToSphericalCoordinates( ( double )orientation.x,
                                                   ( double )orientation.y,
                                                   ( double )orientation.z,
                                                   radial,
                                                   theta,
                                                   phi );
      for ( n = 0; n < N; n++ )
      {

        B( m, n ) = factory->getSymmetricalSphericalHarmonic( n, phi, theta );

      }

    }

    // building Bplus matrix with size N x M
    // Bplus = (B^T B + l L )^(-1) B^T
    gkg::Matrix Bplus( N, inputOrientationCount );
    factory->getThikonovPseudoInverse( B, L, laplaceBeltramiFactor, Bplus );

    // looping over voxel site(s)
    gkg::Vector3d< int32_t > site;
    int32_t sh = 0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      if ( verbose )
      {

        if ( site.z != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " [ " << std::setw( 3 ) << site.z + 1 
        	  << " / " << std::setw( 3 ) << sizeZ
        	  << " ]" << std::flush;

      }

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          // processing average T2 value
          double averageT2 = ( double )t2( site );
          if ( averageT2 <= 0 )
          {

            averageT2 = EPSILON;

          }

          // building valid normalized signal vector
          gkg::Vector normalizedSignal( inputOrientationCount );
          for ( m = 0; m < inputOrientationCount; m++ )
          {
 
            float dwValue = dw( site, m );
            normalizedSignal( m ) = ( double )dwValue / averageT2;

          }

          // processing DW spherical harmonics coefficient(s)
          gkg::Vector dwSphericalHarmonicsCoefficients = Bplus.getComposition(
                                                             normalizedSignal );

          // resampling on the ouput orientation set
          for ( m = 0; m < outputOrientationCount; m++ )
          {

            // collecting outlier orientation
            orientation = outputQSpaceSampling->getOrientation( m );

            // converting cartesian coordinate(s) to spherical coordinate(s)
            factory->getCartesianToSphericalCoordinates(
                                                        ( double )orientation.x,
                                                        ( double )orientation.y,
                                                        ( double )orientation.z,
                                                        radial,
                                                        theta,
                                                        phi );
            // looping over spherical harmonic(s)
            double resampledDwValue = 0;
            for ( sh = 0; sh < N; sh++ )
            {

              resampledDwValue += 
                     dwSphericalHarmonicsCoefficients( sh ) *
                     factory->getSymmetricalSphericalHarmonic( sh, phi, theta );

            }

            // rescaling result with T2 value
            resampledDwValue *= averageT2;
            if ( resampledDwValue < 0 )
            {

               resampledDwValue = 0;

            }

            // storing back corrected value
            dw( site, m ) = ( float )resampledDwValue;

          }

        }

      }

    }

    outputQSpaceSampling->addInformationToHeader( dw );

  }
  GKG_CATCH( "void resamplingSphericalHarmonicsSingleShell( "
             "const gkg::Volume< float >& t2, "
             "gkg::Volume< float >& dw, "
             "const gkg::Volume< int16_t >& /*mask*/, "
             "gkg::RCPointer< gkg::QSpaceSampling > inputQSpaceSampling, "
             "gkg::RCPointer< gkg::QSpaceSampling > outputQSpaceSampling, "
	     "const std::vector< double >& resamplingParameters, "
             "bool verbose )" );

}


//
// class DwiQSpaceResamplerCommand
//

gkg::DwiQSpaceResamplerCommand::DwiQSpaceResamplerCommand( int32_t argc,
                                                           char* argv[],
                                                           bool loadPlugin,
                                                           bool removeFirst )
                               : gkg::Command( argc, argv, loadPlugin,
                                               removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiQSpaceResamplerCommand::DwiQSpaceResamplerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiQSpaceResamplerCommand::DwiQSpaceResamplerCommand(
                              const std::string& fileNameT2,
                              const std::string& fileNameDW,
                              const std::string& fileNameMask,
                              const std::string& fileNameResampledDW,
                              const std::string& fileNameInputOrientations,
                              double bValue,
                              const std::string& fileNameOutputOrientations,
                              int32_t outputOrientationCount,
                              const std::string& outputOrientationScheme,
                              const std::vector< double >& resamplingParameters,
                              float radius,
                              float thresholdRatio,
                              const std::string& volumeFormat,
                              bool ascii,
                              bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             fileNameResampledDW,
             fileNameInputOrientations,
             bValue,
             fileNameOutputOrientations,
             outputOrientationCount,
             outputOrientationScheme,
             resamplingParameters,
             radius,
             thresholdRatio,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceResamplerCommand::DwiQSpaceResamplerCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameResampledDW, "
             "const std::string& fileNameInputOrientations, "
             "double bValue, "
             "const std::string& fileNameOutputOrientations, "
             "int32_t outputOrientationCount, "
             "const std::string& outputOrientationScheme, "
             "const std::vector< double >& resamplingParameters, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiQSpaceResamplerCommand::DwiQSpaceResamplerCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameResampledDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameInputOrientations );
    DECLARE_FLOATING_PARAMETER( parameters, double, bValue );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameOutputOrientations );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              outputOrientationScheme );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           resamplingParameters );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             fileNameResampledDW,
             fileNameInputOrientations,
             bValue,
             fileNameOutputOrientations,
             outputOrientationCount,
             outputOrientationScheme,
             resamplingParameters,
             radius,
             thresholdRatio,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiQSpaceResamplerCommand::DwiQSpaceResamplerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiQSpaceResamplerCommand::~DwiQSpaceResamplerCommand()
{
}


std::string gkg::DwiQSpaceResamplerCommand::getStaticName()
{

  try
  {

    return "DwiQSpaceResampler";

  }
  GKG_CATCH( "std::string gkg::DwiQSpaceResamplerCommand::getStaticName()" );

}


void gkg::DwiQSpaceResamplerCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask;
    std::string fileNameResampledDW;
    std::string fileNameInputOrientations;
    double bValue = 0.0;
    std::string fileNameOutputOrientations;
    int32_t outputOrientationCount = -1;
    std::string outputOrientationScheme = "gkg";
    std::vector< double > resamplingParameters;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    std::string volumeFormat = "gis";
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI Q-space resampler",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input T2-weighted volume file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW-weighted volume file name",
                                 fileNameDW );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-o",
                                 "Output resampled DW volume file name",
                                 fileNameResampledDW );
    application.addSingleOption( "-iof",
                                 "Input Q-space orientation file name"
                                 " (default=look into Q-space meta-info)",
                                 fileNameInputOrientations,
                                 true );
    application.addSingleOption( "-bValue",
                                 "b-value corresponding to the DW data"
                                 " (default=0.0)",
                                 bValue,
                                 true );
    application.addSingleOption( "-oof",
                                 "Output Q-space orientation file name"
                                 " (default=none)",
                                 fileNameOutputOrientations,
                                 true );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count (default=-1)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-outputOrientationScheme",
                                 "Output orientation scheme among:\n"
                                 "- gkg\n"
                                 "- smsvb15\n"
                                 "(default=gkg)",
                                 outputOrientationScheme,
                                 true );
    application.addSeriesOption( "-parameters",
                                 "Parameters of the resampling algorithm: \n"
				 "- spherical_single-shell:\n"
				 "   <SH_order> <laplace_beltrami_factor>\n"
				 "   (default=4/0.006)",
				 resamplingParameters );
    application.addSingleOption( "-radius",
                                 "Radius of morphological operation for mask "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio for mask (default=0.04)",
                                 thresholdRatio,
                                 true );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             fileNameResampledDW,
             fileNameInputOrientations,
             bValue,
             fileNameOutputOrientations,
             outputOrientationCount,
             outputOrientationScheme,
             resamplingParameters,
             radius,
             thresholdRatio,
             volumeFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiQSpaceResamplerCommand::parse()" );

}


void gkg::DwiQSpaceResamplerCommand::execute(
                          const std::string& fileNameT2,
                          const std::string& fileNameDW,
                          const std::string& fileNameMask,
                          const std::string& fileNameResampledDW,
                          const std::string& fileNameInputOrientations,
                          double bValue,
                          const std::string& fileNameOutputOrientations,
                          int32_t outputOrientationCount,
                          const std::string& outputOrientationScheme,
                          const std::vector< double >& resamplingParameters,
                          float radius,
                          float thresholdRatio,
                          const std::string& volumeFormat,
                          bool ascii,
                          bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // map of correction algorithm
    ////////////////////////////////////////////////////////////////////////////

    typedef void ( *ResamplingAlgorithm )( 
                     const gkg::Volume< float >&,
                     gkg::Volume< float >&,
                     const gkg::Volume< int16_t >&,
                     gkg::RCPointer< gkg::QSpaceSampling >,
                     gkg::RCPointer< gkg::QSpaceSampling >,
                     const std::vector< double >&,
                     bool );
					   
    std::map< std::string, ResamplingAlgorithm > resamplingAlgorithmMap;
    resamplingAlgorithmMap[ "spherical_single-shell" ] = 
      resamplingSphericalHarmonicsSingleShell;


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }

    gkg::Volume< float > t2;
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );
    int32_t sizeX = t2.getSizeX();
    int32_t sizeY = t2.getSizeY();
    int32_t sizeZ = t2.getSizeZ();
    gkg::Vector3d< double > resolution;
    t2.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }

    gkg::Volume< float > dw;
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( dw );
    dwVolumeReaderProcess.execute( fileNameDW );

    if ( ( dw.getSizeX() != sizeX ) ||
         ( dw.getSizeY() != sizeY ) ||
         ( dw.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error( "T2 and DW volumes are not of the same size!" );

    }

    gkg::Vector3d< double > tmpResolution;
    dw.getResolution( tmpResolution );
    if ( tmpResolution != resolution )
    {

      throw std::runtime_error( "T2 and DW voxel resolutions are not of the "
                                "same resolution!" );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask of signal
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > mask;
    if ( fileNameMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask of signal from T2 data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        radius = 2.0 * std::min( resolution.x,
                                 std::min( resolution.y, resolution.z ) );

      }
      gkg::ClosedMask< float, int16_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( t2, mask, verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMask << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMask, mask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // obtaining input Q-space sampling from DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "obtaining input Q-space sampling : " << std::flush;

    }
    gkg::RCPointer< gkg::QSpaceSampling > inputQSpaceSampling;

    // if we do not provide any file for the input orientation set
    if ( fileNameInputOrientations.empty() )
    {

      inputQSpaceSampling.reset(
                       gkg::QSpaceSamplingFactory::getInstance().create( dw ) );

    }
    // a file is provided for the input orientation set
    else
    {

      std::vector< std::string > stringParameters( 4 );
      stringParameters[ 0 ] = "spherical";
      stringParameters[ 1 ] = "single-shell";
      stringParameters[ 2 ] = "custom";
      stringParameters[ 3 ] = fileNameInputOrientations;

      std::vector< double > scalarParameters( 1 );
      scalarParameters[ 0 ] = bValue;

      gkg::IdentityTransform3d< float > identityTransform3d;

      gkg::Dictionary gradientCharacteristics;
      gradientCharacteristics[ "type" ] = std::string( "unknown" );

      inputQSpaceSampling.reset(
              gkg::QSpaceSamplingFactory::getInstance().create( 
                                                    stringParameters,
                                                    scalarParameters,
                                                    identityTransform3d,
                                                    gradientCharacteristics ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    std::string inputQSpaceSamplingName = "";
    bool isCartesian = inputQSpaceSampling->isCartesian();
    bool isSpherical = inputQSpaceSampling->isSpherical();

    if ( verbose )
    {

      if ( isCartesian )
      {
      
        std::cout << "sampling type : cartesian" << std::endl; 
        inputQSpaceSamplingName = "cartesian";

      }
      else if ( isSpherical )
      {
      
        std::cout << "sampling type : spherical" << std::flush; 
        inputQSpaceSamplingName = "spherical";

      }

    }
    
    bool isSingleShell = false;
    bool isMultipleShell = false;
    if ( isSpherical )
    {
    
      gkg::SphericalQSpaceSampling*
        sphericalQSpaceSampling = static_cast< gkg::SphericalQSpaceSampling* >(
	  inputQSpaceSampling.get() );
	  
      isSingleShell = sphericalQSpaceSampling->isSingleShell();
      isMultipleShell = sphericalQSpaceSampling->isMultipleShell();

      if ( isSingleShell )
      {
      
        std::cout << ", single-shell" << std::endl;
        inputQSpaceSamplingName += "_single-shell";

      }
      else if ( isMultipleShell )
      {
      
        std::cout << ", multiple-shell" << std::endl;
        inputQSpaceSamplingName += "_multiple-shell";

      }
    
    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks; we actually restrict the resampling to spherical
    // single-shell Q-space acquisitions
    ////////////////////////////////////////////////////////////////////////////

    if ( !isSpherical || !isSingleShell )
    {

      throw std::runtime_error(
       "resampler actually restricted to spherical single-shell Q-space data" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building output Q-space sampling
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building output Q-space sampling : " << std::flush;

    }
    gkg::RCPointer< gkg::QSpaceSampling > outputQSpaceSampling;

    // a file is provided for the output orientation set
    if ( !fileNameOutputOrientations.empty() )
    {

      std::vector< std::string > stringParameters( 4 );
      stringParameters[ 0 ] = "spherical";
      stringParameters[ 1 ] = "single-shell";
      stringParameters[ 2 ] = "custom";
      stringParameters[ 3 ] = fileNameOutputOrientations;

      std::vector< double > scalarParameters( 1 );
      scalarParameters[ 0 ] = bValue;

      gkg::IdentityTransform3d< float > identityTransform3d;

      gkg::Dictionary gradientCharacteristics;
      gradientCharacteristics[ "type" ] = std::string( "unknown" );

      outputQSpaceSampling.reset(
              gkg::QSpaceSamplingFactory::getInstance().create( 
                                                    stringParameters,
                                                    scalarParameters,
                                                    identityTransform3d,
                                                    gradientCharacteristics ) );

    }
    // an output orientation count and scheme are provided
    else
    {

      std::vector< std::string > stringParameters( 3 );
      stringParameters[ 0 ] = "spherical";
      stringParameters[ 1 ] = "single-shell";
      stringParameters[ 2 ] = outputOrientationScheme;

      std::vector< double > scalarParameters( 2 );
      scalarParameters[ 0 ] = bValue;
      scalarParameters[ 1 ] = ( double )outputOrientationCount;

      gkg::IdentityTransform3d< float > identityTransform3d;

      gkg::Dictionary gradientCharacteristics;
      gradientCharacteristics[ "type" ] = std::string( "unknown" );

      outputQSpaceSampling.reset(
              gkg::QSpaceSamplingFactory::getInstance().create( 
                                                    stringParameters,
                                                    scalarParameters,
                                                    identityTransform3d,
                                                    gradientCharacteristics ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // performing resampling
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "resampling data in Q-space : " << std::flush;

    }

    resamplingAlgorithmMap[ inputQSpaceSamplingName ]( t2,
                                                       dw,
                                                       mask,
                                                       inputQSpaceSampling,
                                                       outputQSpaceSampling,
                                                       resamplingParameters,
                                                       verbose );


    ////////////////////////////////////////////////////////////////////////////
    // saving result
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving ' " << fileNameResampledDW << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameResampledDW,
                                      dw,
                                      ascii,
                                      volumeFormat );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiQSpaceResamplerCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameResampledDW, "
             "const std::string& fileNameInputOrientations, "
             "double bValue, "
             "const std::string& fileNameOutputOrientations, "
             "int32_t outputOrientationCount, "
             "const std::string& outputOrientationScheme, "
             "const std::vector< double >& resamplingParameters, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "bool ascii, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    DwiQSpaceResamplerCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameResampledDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameInputOrientations ) +
    DECLARE_FLOATING_PARAMETER_HELP( bValue ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputOrientations ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
    DECLARE_STRING_PARAMETER_HELP( outputOrientationScheme ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( resamplerParameters ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef EPSILON
