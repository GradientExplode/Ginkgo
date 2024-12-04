#include <gkg-dmri-plugin-functors/DwiSingleShellNoiseFilter/DwiSingleShellNoiseFilterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-algobase/StatisticAnalyzer_i.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/MinimumFilter_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-dmri-noise-correction/SingleShellSHRicianNoiseCorrection.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/Reader_i.h> 
#include <gkg-core-io/Writer_i.h> 
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <iomanip>


gkg::DwiSingleShellNoiseFilterCommand::DwiSingleShellNoiseFilterCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::DwiSingleShellNoiseFilterCommand::"
             "DwiSingleShellNoiseFilterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiSingleShellNoiseFilterCommand::DwiSingleShellNoiseFilterCommand(
                                       const std::string& fileNameT2,
                                       const std::string& fileNameDW,
                                       const std::string& fileNameMaskOfSignal,
                                       const std::string& fileNameMaskOfNoise,
                                       const std::string& fileNameDenoisedDW,
                                       const std::string& fileNameCorrectedMask,
                                       float normalizedNoiseStdDev,
                                       float lambda,
                                       int32_t maximumSHOrder,
                                       const std::string& optimizerType,
                                       int32_t maximumIterationCount,
                                       double stepSize,
                                       double maximumTestGradient,
                                       double maximumTolerance,
                                       float deltaParameter,
                                       float radius,
                                       float thresholdRatio,
                                       bool intermediate,
                                       const std::string& removeNegativeValues,
                                       bool verbose )
                                      : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMaskOfSignal,
             fileNameMaskOfNoise,
             fileNameDenoisedDW,
             fileNameCorrectedMask,
             normalizedNoiseStdDev,
             lambda,
             maximumSHOrder,
             optimizerType,
             maximumIterationCount,
             stepSize,
             maximumTestGradient,
             maximumTolerance,
             deltaParameter,
             radius,
             thresholdRatio,
             intermediate,
             removeNegativeValues,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiSingleShellNoiseFilterCommand::"
             "DwiSingleShellNoiseFilterCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMaskOfSignal, "
             "const std::string& fileNameMaskOfNoise, "
             "const std::string& fileNameDenoisedDW, "
             "const std::string& fileNameCorrectedMask, "
             "float normalizedNoiseStdDev, "
             "float lambda, "
             "int32_t maximumSHOrder, "
             "const std::string& optimizerType, "
             "int32_t maximumIterationCount, "
             "double stepSize, "
             "double maximumTestGradient, "
             "double maximumTolerance, "
             "float deltaParameter, "
             "float radius, "
             "float thresholdRatio, "
             "bool intermediate, "
             "const std::string& removeNegativeValues, "
             "bool verbose  )" );

}


gkg::DwiSingleShellNoiseFilterCommand::DwiSingleShellNoiseFilterCommand(
                                             const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMaskOfSignal );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMaskOfNoise );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDenoisedDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCorrectedMask );
    DECLARE_FLOATING_PARAMETER( parameters, float, normalizedNoiseStdDev );
    DECLARE_FLOATING_PARAMETER( parameters, float, lambda );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumSHOrder );
    DECLARE_STRING_PARAMETER( parameters, std::string, optimizerType );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumIterationCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, stepSize );
    DECLARE_FLOATING_PARAMETER( parameters, double, maximumTestGradient );
    DECLARE_FLOATING_PARAMETER( parameters, double, maximumTolerance );
    DECLARE_FLOATING_PARAMETER( parameters, float, deltaParameter );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, intermediate );
    DECLARE_STRING_PARAMETER( parameters, std::string, removeNegativeValues );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
 
    execute( fileNameT2,
             fileNameDW,
             fileNameMaskOfSignal,
             fileNameMaskOfNoise,
             fileNameDenoisedDW,
             fileNameCorrectedMask,
             normalizedNoiseStdDev,
             lambda,
             maximumSHOrder,
             optimizerType,
             maximumIterationCount,
             stepSize,
             maximumTestGradient,
             maximumTolerance,
             deltaParameter,
             radius,
             thresholdRatio,
             intermediate,
             removeNegativeValues,
             verbose  );


  }
  GKG_CATCH( "gkg::DwiSingleShellNoiseFilterCommand::"
             "DwiSingleShellNoiseFilterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiSingleShellNoiseFilterCommand::~DwiSingleShellNoiseFilterCommand()
{
}


std::string gkg::DwiSingleShellNoiseFilterCommand::getStaticName()
{

  try
  {

    return "DwiSingleShellNoiseFilter";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiSingleShellNoiseFilterCommand::getStaticName()" );

}


void gkg::DwiSingleShellNoiseFilterCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMaskOfSignal = "";
    std::string fileNameMaskOfNoise = "";
    std::string fileNameDenoisedDW;
    std::string fileNameCorrectedMask = "";
    float normalizedNoiseStdDev = -1.0;
    float lambda = 0;
    int32_t maximumSHOrder = 4;
    std::string optimizerType = "simplex";
    int32_t maximumIterationCount = 200;
    double stepSize = 1e-3;
    double maximumTestGradient = 1e-3;
    double maximumTolerance = 1e-4;
    float deltaParameter = 0.1;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    bool intermediate = false;
    std::string removeNegativeValues = "setting_median_on_neighborhood";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "DWI Noise filter dedicated to single shell "
                                  "data",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input T2 file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW file name",
                                 fileNameDW );
    application.addSingleOption( "-ms",
                                 "Input signal mask file name",
                                 fileNameMaskOfSignal,
                                 true );
    application.addSingleOption( "-mn",
                                 "Input noise mask file name",
                                 fileNameMaskOfNoise,
                                 true );
    application.addSingleOption( "-o",
                                 "Rician noise corrected DW file name",
                                 fileNameDenoisedDW );
    application.addSingleOption( "-c",
                                 "Corrected site mask file file name",
                                 fileNameCorrectedMask,
                                 true );

    application.addSingleOption( "-type",
                                 "Minimizer type: \n"
                                 "- simplex -> Nelder Mead Simplex optimizer\n"
                                 "- conjgrad -> Fletcher Reeves Conjugate "
                                 "Gradient optimizer\n",
                                 optimizerType,
                                 true );


    application.addSingleOption( "-normalizedNoiseStdDev",
                                 "Normalized noise standard deviation of the DW image"
                                 " (default=auto-estimated)",
                                 normalizedNoiseStdDev,
                                 true );

    application.addSingleOption( "-l",
                                 "Laplace-Beltrami regularization factor"
                                 " (default = 0.0)",
                                 lambda,
                                 true );
    application.addSingleOption( "-maximumSHOrder",
                                 "Maximum of the SH order (default = 4)",
                                 maximumSHOrder,
                                 true );

    application.addSingleOption( "-maximumIterationCount",
                                 "Maximum iteration count (default = 200)",
                                 maximumIterationCount,
                                 true );
    application.addSingleOption( "-stepSize",
                                 "Fletcher-Reeves step size (default=0.001)",
                                 stepSize,
                                 true );
    application.addSingleOption( "-maximumTestGradient",
                                 "Maximum Test gradient of the minimizer"
                                 " (default=0.001)",
                                 maximumTestGradient,
                                 true );
    application.addSingleOption( "-maximumTolerance",
                                 "Maximum tolerance of the minimizer"
                                 " (default=1e-4)",
                                 maximumTolerance,
                                 true );
    application.addSingleOption( "-delta",
                                 "Simplex delta parameters"
                                 " (default=(0.1)",
                                 deltaParameter,
                                 true );

    application.addSingleOption( "-radius",
                                 "Radius of morphological operation for mask "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio for mask (default=0.04)",
                                 thresholdRatio,
                                 true );

    application.addSingleOption( "-intermediate",
                                 "Save intermediate files : \n"
                                 "- mask of noise\n"
                                 "- normalized noise standard deviation map",
                                 intermediate,
                                 true );

    application.addSingleOption( "-removeNegativeValues",
                                 "Post-removal of negative values : \n"
                                 "- setting_median_on_neighborhhood\n"
                                 "- setting_zero\n"
                                 "- doing_nothing\n"
                                 "(default=setting_median_on_neighborhood)",
                                 removeNegativeValues,
                                 true );

    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameT2,
             fileNameDW,
             fileNameMaskOfSignal,
             fileNameMaskOfNoise,
             fileNameDenoisedDW,
             fileNameCorrectedMask,
             normalizedNoiseStdDev,
             lambda,
             maximumSHOrder,
             optimizerType,
             maximumIterationCount,
             stepSize,
             maximumTestGradient,
             maximumTolerance,
             deltaParameter,
             radius,
             thresholdRatio,
             intermediate,
             removeNegativeValues,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiSingleShellNoiseFilterCommand::parse()" );

}


void gkg::DwiSingleShellNoiseFilterCommand::execute(
                                       const std::string& fileNameT2,
                                       const std::string& fileNameDW,
                                       const std::string& fileNameMaskOfSignal,
                                       const std::string& fileNameMaskOfNoise,
                                       const std::string& fileNameDenoisedDW,
                                       const std::string& fileNameCorrectedMask,
                                       float normalizedNoiseStdDev,
                                       float lambda,
                                       int32_t maximumSHOrder,
                                       const std::string& optimizerType,
                                       int32_t maximumIterationCount,
                                       double stepSize,
                                       double maximumTestGradient,
                                       double maximumTolerance,
                                       float deltaParameter,
                                       float radius,
                                       float thresholdRatio,
                                       bool intermediate,
                                       const std::string& removeNegativeValues,
                                       bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    // processing maximum j according to maximum order
    int32_t parameterCount = ( ( maximumSHOrder + 1 ) *
                               ( maximumSHOrder + 2 ) ) / 2;


    if ( ( optimizerType != "simplex" ) &&
         ( optimizerType != "conjgrad" ) )
    {

      throw std::runtime_error( "bad optimizer type" );

    }
    std::vector< float > deltaParameters( parameterCount, deltaParameter );

    if ( ( removeNegativeValues != "setting_median_on_neighborhood" ) &&
         ( removeNegativeValues != "setting_zero" ) &&
         ( removeNegativeValues != "doing_nothing" ) )
    {

      throw std::runtime_error( "bad negative value removal method name" );

    }
         


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
    int32_t inputOrientationCount = dw.getSizeT();

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

    gkg::Volume< int16_t > maskOfSignal;
    if ( fileNameMaskOfSignal.empty() )
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
      closedMask.getMask( t2, maskOfSignal, verbose );
      if ( intermediate )
      {

        gkg::File file( fileNameDenoisedDW );
        std::string directoryName = file.getDirectoryName();
        gkg::Writer::getInstance().write( directoryName +
                                          "mask_of_signal",
                                          maskOfSignal );

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMaskOfSignal << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMaskOfSignal, maskOfSignal );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask of noise
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > maskOfNoise;
    if ( fileNameMaskOfNoise.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask of noise from T2 data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        radius = 2.0 * std::min( resolution.x,
                                 std::min( resolution.y, resolution.z ) );

      }

      gkg::StatisticAnalyzer< gkg::Volume< float > > statisticAnalyzer;
      float minimum = statisticAnalyzer.getMinimum( t2 );
      float maximum = statisticAnalyzer.getMaximum( t2 );
      float threshold = minimum +
               ( float )( ( double )( maximum - minimum ) * thresholdRatio );
      gkg::LowerThanFunction< float > lowerThanFunction( threshold );
      gkg::Binarizer< gkg::Volume< float >, gkg::Volume< int16_t > >
        binarizer( lowerThanFunction, 1, 0 );

      binarizer.binarize( t2, maskOfNoise );

      if ( intermediate )
      {

        gkg::File file( fileNameDenoisedDW );
        std::string directoryName = file.getDirectoryName();
        gkg::Writer::getInstance().write( directoryName +
                                          "mask_of_noise",
                                          maskOfNoise );

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMaskOfNoise << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMaskOfNoise, maskOfNoise );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing noise standard deviation
    // E(signal) = sigma * sqrt( Pi / 2 ) outside the brain mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing noise standard deviation : " << std::flush;

    }
    double computedNoiseStdDev = 0;
    gkg::Vector3d< int32_t > site;
    int32_t count = 0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfNoise( site ) )
          {

            computedNoiseStdDev += ( double )t2( site );
            ++ count;

          }

        }

      }

    }
    computedNoiseStdDev /= ( double )count;
    computedNoiseStdDev /= std::sqrt( M_PI / 2.0 );

    if ( verbose )
    {

      std::cout << computedNoiseStdDev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing average minimum signal
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing average minimum signal : " << std::flush;

    }
    gkg::Volume< float > minimumSignalMap( sizeX, sizeY, sizeZ );
    minimumSignalMap.fill( 0 );
    double averageMinimumSignal = 0;
    count = 0;
    int32_t o = 0;
    std::vector< int16_t > dwSignal( inputOrientationCount );
    int16_t minimumSignal = 0;
    gkg::MinimumFilter< std::vector< int16_t >, int16_t >
      minimumFilter;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) )
          {

            for ( o = 0; o < inputOrientationCount; o++ )
            {

              dwSignal[ o ] = dw( site, o );

            }
            minimumFilter.filter( dwSignal, minimumSignal );
            averageMinimumSignal += ( double )minimumSignal;
            minimumSignalMap( site ) = ( float )minimumSignal;
            ++ count;

          }

        }

      }

    }
    averageMinimumSignal /= ( double )count;
    if ( intermediate )
    {

      gkg::File file( fileNameDenoisedDW );
      std::string directoryName = file.getDirectoryName();
      gkg::Writer::getInstance().write( directoryName +
                                        "minimum_signal",
                                        minimumSignalMap );

    }

    if ( verbose )
    {

      std::cout << averageMinimumSignal << std::endl;
      std::cout << "estimated SNR : "
                << averageMinimumSignal / computedNoiseStdDev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing normalized standard deviation map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing normalized standard deviation map : "
                << std::flush;

    }
    gkg::BoundingBox< int32_t > boundingBox( t2 );
    gkg::Volume< float > normalizedNoiseStdDevMap( sizeX, sizeY, sizeZ );
    double averageNormalizedNoiseStdDev = 0.0;
    normalizedNoiseStdDevMap.fill( 0 );
    count = 0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) )
          {

            if ( normalizedNoiseStdDev > 0 )
            {

              normalizedNoiseStdDevMap( site ) = normalizedNoiseStdDev;
              averageNormalizedNoiseStdDev += normalizedNoiseStdDev;

            }
            else
            {

              if ( t2( site ) > 0 )
              {

                normalizedNoiseStdDevMap( site ) = computedNoiseStdDev /
                                                   t2( site );
                averageNormalizedNoiseStdDev +=
                                               normalizedNoiseStdDevMap( site );

              }
              else
              {

                double averageT2 = 0.0;
                gkg::Vector3d< int32_t > offset;
                int32_t neighborCount = 0;
                for ( offset.z = -1; offset.z <= 1; offset.z++ )
                {

                  for ( offset.y = -1; offset.y <= 1; offset.y++ )
                  {

                    for ( offset.x = -1; offset.x <= 1; offset.x++ )
                    {

                      gkg::Vector3d< int32_t > neighbor = site + offset;
                      if ( boundingBox.contains( neighbor ) )
                      {

                        averageT2 += t2( neighbor );
                        ++ neighborCount;

                      }

                    }

                  }

                }
                normalizedNoiseStdDevMap( site ) = computedNoiseStdDev /
                                                   ( averageT2 /
                                                     neighborCount );
                averageNormalizedNoiseStdDev +=
                                               normalizedNoiseStdDevMap( site );

              }

            }
            ++ count;

          }

        }

      }

    }
    averageNormalizedNoiseStdDev /= ( double )count;
    if ( intermediate )
    {

      gkg::File file( fileNameDenoisedDW );
      std::string directoryName = file.getDirectoryName();
      gkg::Writer::getInstance().write( directoryName +
                                        "normalized_noise_stddev",
                                        normalizedNoiseStdDevMap );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "average normalized noise standard deviation : "
                << averageNormalizedNoiseStdDev << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // estimating site count
    ////////////////////////////////////////////////////////////////////////////

    int32_t siteCount = 0;
    gkg::Volume< int16_t >::const_iterator
      s = maskOfSignal.begin(),
      se = maskOfSignal.end();
    while ( s != se )
    {

      if ( *s )
      {

        ++ siteCount;

      }
      ++ s;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating denoised DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating denoised DW volume : " << std::flush;

    }

    gkg::Volume< float > denoisedDw( sizeX, sizeY, sizeZ,
                                     inputOrientationCount );
    denoisedDw.getHeader().addAttribute( "resolutionX", resolution.x );
    denoisedDw.getHeader().addAttribute( "resolutionY", resolution.y );
    denoisedDw.getHeader().addAttribute( "resolutionZ", resolution.z );
    denoisedDw.fill( 0 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // read b-values / orientations from the input DW file and put it in the 
    // denoised DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "copying Q-space sampling to denoised DW volume : "
                << std::flush;

    }
    std::vector< double > bValues;
    dw.getHeader().getAttribute( "bvalues", bValues );

    gkg::GenericObjectList diffusion_gradient_orientations;
    dw.getHeader().getAttribute( "diffusion_gradient_orientations",
                                 diffusion_gradient_orientations );


    denoisedDw.getHeader().addAttribute( "bvalues", bValues );
    denoisedDw.getHeader().addAttribute( "diffusion_gradient_orientations",
                                         diffusion_gradient_orientations );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // denoising DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "denoising : " << std::flush;

    }

    gkg::Volume< int16_t > correctedMask;
    bool buildCorrectedMask = false;
    if ( !fileNameCorrectedMask.empty() )
    {

      buildCorrectedMask = true;
      correctedMask.reallocate( sizeX, sizeY, sizeZ );
      correctedMask.getHeader().addAttribute( "resolutionX", resolution.x );
      correctedMask.getHeader().addAttribute( "resolutionY", resolution.y );
      correctedMask.getHeader().addAttribute( "resolutionZ", resolution.z );

    }

    o = 0;
    count = 0;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) )
          {

            if ( verbose )
            {

              if ( ( ( count + 1 ) % 100 == 0 ) ||
                   ( count == 0 ) ||
                   ( count == siteCount - 1 ) )
              {

                if ( count )
                {

                  std::cout << gkg::Eraser( 22 );

                }
                std::cout << " [ " << std::setw( 7 ) << count + 1
                          << " / " << std::setw( 7 ) << siteCount
                          << " ]" << std::flush;

              }

            }
            gkg::Vector dwVector;
            if ( optimizerType == "conjgrad" )
            {

              // creating an instance of SingleShellSHRicianNoiseCorrection
              gkg::SingleShellSHRicianNoiseCorrection
                singleShellSHRicianNoiseCorrection(
                                              t2,
                                              dw,
                                              site,
                                              maximumSHOrder,
                                              lambda,
                                              normalizedNoiseStdDevMap( site ),
                                              maximumIterationCount,
                                              stepSize,
                                              maximumTestGradient,
                                              maximumTolerance );
              dwVector = singleShellSHRicianNoiseCorrection.getDenoisedDw();
              if ( buildCorrectedMask )
              {

                correctedMask( site ) = 
                  ( singleShellSHRicianNoiseCorrection.isCorrected() ? 1 : 0 );

              }

            }
            else if ( optimizerType == "simplex" )
            {

              // creating an instance of SingleShellSHRicianNoiseCorrection
              gkg::SingleShellSHRicianNoiseCorrection
                singleShellSHRicianNoiseCorrection(
                                              t2,
                                              dw,
                                              site,
                                              maximumSHOrder,
                                              lambda,
                                              normalizedNoiseStdDevMap( site ),
                                              deltaParameters,
                                              maximumIterationCount,
                                              maximumTolerance );
              dwVector = singleShellSHRicianNoiseCorrection.getDenoisedDw();
              if ( buildCorrectedMask )
              {

                correctedMask( site ) = 
                  ( singleShellSHRicianNoiseCorrection.isCorrected() ? 1 : 0 );

              }

            }


            // filling the DW volume
            for ( o = 0; o < inputOrientationCount; o++ )
            {

              denoisedDw( site, o ) = ( float )dwVector( o );

            }

            // increasing site counter
            ++ count;

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // removing negative values
    ////////////////////////////////////////////////////////////////////////////

    if ( removeNegativeValues == "setting_median_on_neighborhood" )
    {

      if ( verbose )
      {

        std::cout << "removing negative values : " << std::flush;

      }

      std::list< gkg::Vector3d< int32_t > >
        neighborOffsets = gkg::Neighborhood3d::getInstance().
                            getNeighborOffsets(
                              gkg::Neighborhood3d::Neighborhood3d_26 );

      gkg::Vector3d< int32_t > neighbor;
      int16_t denoisedValue = 0;
      double sum = 0.0;
      for ( o = 0; o < inputOrientationCount; o++ )
      {

        for ( site.z = 0; site.z < sizeZ; site.z++ )
        {

          for ( site.y = 0; site.y < sizeY; site.y++ )
          {

            for( site.x = 0; site.x < sizeX; site.x++ )
            {

              if ( maskOfSignal( site ) && ( denoisedDw( site, o ) < 0 ) )
              {

                count = 0;
                sum = 0.0;
                std::list< gkg::Vector3d< int32_t > >::const_iterator
                  no = neighborOffsets.begin(),
                  noe = neighborOffsets.end();
                while ( no != noe )
                {

                  neighbor = site + *no;
                  if ( boundingBox.contains( neighbor ) )
                  {

                    denoisedValue = denoisedDw( neighbor, o );
                    if ( maskOfSignal( neighbor ) && ( denoisedValue >= 0 ) )
                    {

                      sum += ( double )denoisedValue;
                      ++ count;

                    }

                  }
                  ++ no;

                }
                if ( count )
                {

                  denoisedDw( site, o ) = ( float )( sum / ( double )count );

                }
                else
                {

                  denoisedDw( site, o ) = 0.0;

                }

              }

            }

          }

        }

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( removeNegativeValues == "setting_setting_zero" )
    {

      if ( verbose )
      {

        std::cout << "removing negative values : " << std::flush;

      }

      gkg::GreaterOrEqualToFunction< float >
        greaterOrEqualToFunction( ( float )0 );
      gkg::Thresholder< gkg::Volume< float > >
        thresholder( greaterOrEqualToFunction, ( float )0 );
      thresholder.threshold( denoisedDw, denoisedDw );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }




    ////////////////////////////////////////////////////////////////////////////
    // writing the denoised DW volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing denoised DW volume : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameDenoisedDW, denoisedDw );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the corrected mask
    ////////////////////////////////////////////////////////////////////////////

    if ( buildCorrectedMask )
    {

      if ( verbose )
      {

        std::cout << "writing corrected mask : " << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameCorrectedMask, correctedMask );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::DwiSingleShellNoiseFilterCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMaskOfSignal, "
             "const std::string& fileNameMaskOfNoise, "
             "const std::string& fileNameDenoisedDW, "
             "const std::string& fileNameCorrectedMask, "
             "float normalizedNoiseStdDev, "
             "float lambda, "
             "int32_t maximumSHOrder, "
             "const std::string& optimizerType, "
             "int32_t maximumIterationCount, "
             "double stepSize, "
             "double maximumTestGradient, "
             "double maximumTolerance, "
             "float deltaParameter, "
             "float radius, "
             "float thresholdRatio, "
             "bool intermediate, "
             "const std::string& removeNegativeValues, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    DwiSingleShellNoiseFilterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMaskOfSignal ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMaskOfNoise ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDenoisedDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameCorrectedMask ) +
    DECLARE_FLOATING_PARAMETER_HELP( normalizedNoiseStdDev ) +
    DECLARE_FLOATING_PARAMETER_HELP( lambda ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumSHOrder ) +
    DECLARE_STRING_PARAMETER_HELP( optimizerType ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumIterationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( stepSize ) +
    DECLARE_FLOATING_PARAMETER_HELP( maximumTestGradient ) +
    DECLARE_FLOATING_PARAMETER_HELP( maximumTolerance ) +
    DECLARE_FLOATING_PARAMETER_HELP( deltaParameter ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( intermediate ) +
    DECLARE_STRING_PARAMETER_HELP( removeNegativeValues ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
