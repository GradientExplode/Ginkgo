#include <gkg-fmri-plugin-functors/BoldAnalysis/BoldAnalysisCommand.h>
#include <gkg-fmri-statistics/StatisticalFunctionFactory.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-processing-numericalanalysis/KalmanFilter.h>
#include <gkg-fmri-paradigm/Paradigm.h>
#include <gkg-fmri-stimulus/DiscreteStimulus.h>
#include <gkg-fmri-stimulus/ContinuousStimulus.h>
#include <gkg-fmri-hrf/CanonicalHRF.h>
#include <gkg-fmri-glm/BoldGeneralLinearModelSolver_i.h>
#include <gkg-fmri-contrast/ContrastEffects.h>

#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>

#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>


gkg::BoldAnalysisCommand::BoldAnalysisCommand( int32_t argc,
                                               char* argv[],
                                               bool loadPlugin,
                                               bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::BoldAnalysisCommand::BoldAnalysisCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BoldAnalysisCommand::BoldAnalysisCommand(
                                   const std::string& fileNameBOLD,
                                   const std::string& fileNameAverageBOLD,
                                   const std::string& fileNameSmoothedBOLD,
                                   const std::string& fileNameMask,
                                   const std::string& fileNameParadigm,
                                   const std::string& fileNameContrastEffects,
                                   const std::string& fileNameDesignMatrix,
                                   const std::string& fileNameEffects,
                                   const std::string& fileNameEffectVariances,
                                   const std::vector< std::string >& mapAndName,
                                   double startingTime,
                                   double repetitionTime,
                                   int32_t dummyScanCount,
                                   int32_t driftPolynomOrder,
                                   float smoothingFWHM,
                                   const std::string& smoothingAxis,
                                   const std::string& fileNameHRF,
                                   double hrfDelay, 
                                   double hrfDelayUndershoot, 
                                   double hrfDispersion, 
                                   double hrfDispersionUndershoot,
                                   double hrfRatio, 
                                   bool ascii,
                                   const std::string& volumeFormat,
                                   bool verbose )
                        : gkg::Command()
{

  try
  {

    execute( fileNameBOLD, fileNameAverageBOLD, fileNameSmoothedBOLD,
             fileNameMask, fileNameParadigm, fileNameContrastEffects,
             fileNameDesignMatrix, fileNameEffects, fileNameEffectVariances,
             mapAndName, startingTime, repetitionTime, dummyScanCount,
             driftPolynomOrder, smoothingFWHM, smoothingAxis, fileNameHRF,
             hrfDelay, hrfDelayUndershoot, hrfDispersion,
             hrfDispersionUndershoot, hrfRatio, ascii, volumeFormat, verbose );

  }
  GKG_CATCH( "gkg::BoldAnalysisCommand::BoldAnalysisCommand( "
             "const std::string& fileNameBOLD, "
             "const std::string& fileNameAverageBOLD, "
             "const std::string& fileNameSmoothedBOLD, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameParadigm, "
             "const std::string& fileNameContrastEffects, "
             "const std::string& fileNameDesignMatrix, "
             "const std::string& fileNameEffects, "
             "const std::string& fileNameEffectVariances, "
             "const std::vector< std::string >& mapAndName, "
             "double startingTime, double repetitionTime, "
             "int32_t dummyScanCount, int32_t driftPolynomOrder, "
             "float smoothingFWHM, const std::string& smoothingAxis, "
             "const std::string& fileNameHRF, double hrfDelay, "
             "double hrfDelayUndershoot, double hrfDispersion, "
             "double hrfDispersionUndershoot, double hrfRatio, "
             "bool ascii, const std::string& volumeFormat, bool verbose )" );

}


gkg::BoldAnalysisCommand::BoldAnalysisCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBOLD );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameAverageBOLD );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSmoothedBOLD );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameParadigm );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameHRF );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDesignMatrix );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameContrastEffects );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameEffects );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameEffectVariances );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         mapAndName );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_FLOATING_PARAMETER( parameters, double, startingTime );
    DECLARE_FLOATING_PARAMETER( parameters, double, repetitionTime );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, dummyScanCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, driftPolynomOrder );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_FLOATING_PARAMETER( parameters, float, smoothingFWHM );
    DECLARE_FLOATING_PARAMETER( parameters, float, smoothingKernelSize );
    DECLARE_STRING_PARAMETER( parameters, std::string, smoothingAxis );
    DECLARE_FLOATING_PARAMETER( parameters, double, hrfDelay );
    DECLARE_FLOATING_PARAMETER( parameters, double, hrfDelayUndershoot );
    DECLARE_FLOATING_PARAMETER( parameters, double, hrfDispersion );
    DECLARE_FLOATING_PARAMETER( parameters, double, hrfDispersionUndershoot );
    DECLARE_FLOATING_PARAMETER( parameters, double, hrfRatio );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBOLD, fileNameAverageBOLD, fileNameSmoothedBOLD,
             fileNameMask, fileNameParadigm, fileNameContrastEffects,
             fileNameDesignMatrix, fileNameEffects, fileNameEffectVariances,
             mapAndName, startingTime, repetitionTime, dummyScanCount,
             driftPolynomOrder, smoothingFWHM, smoothingAxis, fileNameHRF,
             hrfDelay, hrfDelayUndershoot, hrfDispersion,
             hrfDispersionUndershoot, hrfRatio, ascii, volumeFormat, verbose );

  }
  GKG_CATCH( "gkg::BoldAnalysisCommand::BoldAnalysisCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BoldAnalysisCommand::~BoldAnalysisCommand()
{
}


std::string gkg::BoldAnalysisCommand::getStaticName()
{

  try
  {

    return "BoldAnalysis";

  }
  GKG_CATCH( "std::string gkg::BoldAnalysisCommand::getStaticName()" );

}


void gkg::BoldAnalysisCommand::parse()
{

  try
  {

    std::string fileNameBOLD;
    std::string fileNameAverageBOLD = "";
    std::string fileNameSmoothedBOLD = "";
    std::string fileNameMask = "";
    std::string fileNameParadigm;
    std::string fileNameHRF = "";
    std::string fileNameDesignMatrix = "";
    std::string fileNameContrastEffects;
    std::string fileNameEffects;
    std::string fileNameEffectVariances;

    std::vector< std::string > mapAndName;

    std::string volumeFormat = "gis";

    double startingTime = 0.0;
    double repetitionTime = -1.0;
    int32_t dummyScanCount = 0;
    int32_t driftPolynomOrder = 3;

    float smoothingFWHM = 0.0;
    std::string smoothingAxis = "xyz";

    double hrfDelay = GKG_BOLD_CANONICAL_HRF_DELAY;
    double hrfDelayUndershoot = GKG_BOLD_CANONICAL_HRF_DELAY_UNDERSHOOT;
    double hrfDispersion = GKG_BOLD_CANONICAL_HRF_DISPERSION;
    double hrfDispersionUndershoot =
                                   GKG_BOLD_CANONICAL_HRF_DISPERSION_UNDERSHOOT;
    double hrfRatio = GKG_BOLD_CANONICAL_HRF_RATIO;

    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // building BOLD map list
    ////////////////////////////////////////////////////////////////////////////

    std::string seriesText = "Output map/map name(s)\n"
                             "syntax:\n"
                             "-stats <map1> <name1> <map2> <name2>\n"
                             "with map one of :\n";
    std::list< std::string >
      statisticalFunctionNames = gkg::StatisticalFunctionFactory::getInstance().
                                                  getStatisticalFunctionNames();
    std::list< std::string >::const_iterator
      f = statisticalFunctionNames.begin(),
      fe = statisticalFunctionNames.end();
    while ( f != fe )
    {

      seriesText += "- " + *f + "\n";
      ++ f;

    }

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, 
                                  "BOLD functional analysis",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input BOLD EPI file name",
                                 fileNameBOLD );
    application.addSingleOption( "-a",
                                 "Average BOLD EPI file name",
                                 fileNameAverageBOLD,
                                 true );
    application.addSingleOption( "-s",
                                 "Smoothed BOLD EPI file name",
                                 fileNameSmoothedBOLD,
                                 true );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-p",
                                 "Paradigm description text file",
                                 fileNameParadigm );
    application.addSingleOption( "-c",
                                 "Contrast effects text file",
                                 fileNameContrastEffects );
    application.addSingleOption( "-d",
                                 "Design matrix image file name",
                                 fileNameDesignMatrix,
                                 true );
    application.addSingleOption( "-effects",
                                 "Output effects file name",
                                 fileNameEffects );
    application.addSingleOption( "-variances",
                                 "Output variance of effects file name",
                                 fileNameEffectVariances );
    application.addSeriesOption( "-stats",
                                 seriesText,
                                 mapAndName );
    application.addSingleOption( "-startingTime",
                                 "Starting time (default=0s)",
                                 startingTime,
                                 true );
    application.addSingleOption( "-repetitionTime",
                                 "Repetition time in seconds "
                                 "(default=TR read in the file)",
                                 repetitionTime,
                                 true );
    application.addSingleOption( "-dummyScanCount",
                                 "Dummy scan count (default=0)",
                                 dummyScanCount,
                                 true );
    application.addSingleOption( "-driftPolynomOrder",
                                 "Drift polynom order (default=3)",
                                 driftPolynomOrder,
                                 true );
    application.addSingleOption( "-smoothing",
                                 "Isotropic smoothing kernel FWHM in mm"
                                 " (default=0.0)",
                                 smoothingFWHM,
                                 true );
    application.addSingleOption( "-smoothingAxis",
                                 "Smoothing axis (default=xyz)",
                                 smoothingAxis,
                                 true );
    application.addSingleOption( "-hrf",
                                 "HRF text file name",
                                 fileNameHRF,
                                 true );
    application.addSingleOption( "-hrfDelay",
                                 "Canonical HRF delay (default=6.0s)",
                                 hrfDelay,
                                 true );
    application.addSingleOption( "-hrfDelayUndershoot",
                                 "Canonical HRF delay undershoot "
                                 "(default=16.0s)",
                                 hrfDelayUndershoot,
                                 true );
    application.addSingleOption( "-hrfDispersion",
                                 "Canonical HRF dispersion (default=1.0)",
                                 hrfDispersion,
                                 true );
    application.addSingleOption( "-hrfDispersionUndershoot",
                                 "Canonical HRF dispersion undershoot "
                                 "(default=1.0)",
                                 hrfDispersionUndershoot,
                                 true );
    application.addSingleOption( "-hrfRatio",
                                 "Canonical HRF ratio (default=6.0)",
                                 hrfRatio,
                                 true );
     application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameBOLD, fileNameAverageBOLD, fileNameSmoothedBOLD,
             fileNameMask, fileNameParadigm, fileNameContrastEffects,
             fileNameDesignMatrix, fileNameEffects, fileNameEffectVariances,
             mapAndName, startingTime, repetitionTime, dummyScanCount,
             driftPolynomOrder, smoothingFWHM, smoothingAxis, fileNameHRF,
             hrfDelay, hrfDelayUndershoot, hrfDispersion,
             hrfDispersionUndershoot, hrfRatio, ascii, volumeFormat, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BoldAnalysisCommand::parse()" );

}


void gkg::BoldAnalysisCommand::execute( 
                                   const std::string& fileNameBOLD,
                                   const std::string& fileNameAverageBOLD,
                                   const std::string& fileNameSmoothedBOLD,
                                   const std::string& fileNameMask,
                                   const std::string& fileNameParadigm,
                                   const std::string& fileNameContrastEffects,
                                   const std::string& fileNameDesignMatrix,
                                   const std::string& fileNameEffects,
                                   const std::string& fileNameEffectVariances,
                                   const std::vector< std::string >& mapAndName,
                                   double startingTime,
                                   double repetitionTime,
                                   int32_t dummyScanCount,
                                   int32_t driftPolynomOrder,
                                   float smoothingFWHM,
                                   const std::string& smoothingAxis,
                                   const std::string& fileNameHRF,
                                   double hrfDelay, 
                                   double hrfDelayUndershoot, 
                                   double hrfDispersion, 
                                   double hrfDispersionUndershoot,
                                   double hrfRatio, 
                                   bool ascii,
                                   const std::string& volumeFormat,
                                   bool verbose )
{

  try
  {

    float radius = -1.0;
    float thresholdRatio = 0.04;
    float smoothingKernelSize = 0.0;

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( !mapAndName.empty() && ( mapAndName.size() % 2 ) )
    {

      throw std::runtime_error( " bad maps" );

    }

    if ( smoothingFWHM < 0 )
    {

      throw std::runtime_error( "smoothing kernel size must be positive" );

    }
    else
    {

      // sigma is related to full width half maximum by the expression
      //  sigma = FWHM / 2sqrt(2log2) = FWHM / 2.35
      smoothingKernelSize = smoothingFWHM /
                            ( 2.0 * std::sqrt( 2.0 * std::log( 2.0 ) ) );

    }

    if ( ( smoothingAxis != "xyz" ) &&
         ( smoothingAxis != "xy" ) &&
         ( smoothingAxis != "xz" ) &&
         ( smoothingAxis != "yz" ) &&
         ( smoothingAxis != "x" ) &&
         ( smoothingAxis != "y" ) &&
         ( smoothingAxis != "z" ) )
    {

      throw std::runtime_error( "not a valid smoothing axis set" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading BOLD data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameBOLD << "' : " << std::flush;

    }
    gkg::Volume< int16_t > bold;
    gkg::Reader::getInstance().read( fileNameBOLD, bold );
    int sizeX = bold.getSizeX();
    int sizeY = bold.getSizeY();
    int sizeZ = bold.getSizeZ();
    int sizeT = bold.getSizeT();
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( bold.getHeader().hasAttribute( "resolutionX" ) &&
         bold.getHeader().hasAttribute( "resolutionY" ) &&
         bold.getHeader().hasAttribute( "resolutionZ" ) )
    {

      bold.getHeader().getAttribute( "resolutionX", resolutionX );
      bold.getHeader().getAttribute( "resolutionY", resolutionY );
      bold.getHeader().getAttribute( "resolutionZ", resolutionZ );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // finding repetition time in image header
    ////////////////////////////////////////////////////////////////////////////

    double TR;

    if ( verbose )
    {

     std::cout << "repetition time TR : " << std::flush;

    }
    //  DICOM case
    if ( bold.getHeader().hasAttribute( "tr" ) )
    {

      bold.getHeader().getAttribute( "tr", TR );
      TR /= 1000.0;

    }
    else if ( bold.getHeader().hasAttribute( "dicom" ) )
    {

      gkg::Dictionary dicomDictionary;
      bold.getHeader().getAttribute( "dicom", dicomDictionary );

      if ( dicomDictionary.find( "(0018,0080)" ) != dicomDictionary.end() )
      {

        std::string tmp = dicomDictionary[ "(0018,0080)" ]->getString();
        gkg::StringConverter::toScalar( tmp, TR );
        // converting from ms to s
        TR /= 1000.0;

      }
      else
      {

        if ( repetitionTime > 0 )
        {

          TR = repetitionTime;

        }
        else
        {

          throw std::runtime_error( "tag (0018,0080) missing in DICOM header" );

        }

      }

    }
    // GEHC p-file case
    else if ( bold.getHeader().hasAttribute( "pool_header.rdb_hdr_image.tr" ) )
    {

      bold.getHeader().getAttribute( "pool_header.rdb_hdr_image.tr", TR );
      // converting from ms to s
      TR /= 1e6;

    }
    else
    {

      if ( repetitionTime > 0 )
      {

        TR = repetitionTime;

      }
      else
      {

        throw std::runtime_error( "could not determine repetition time TR" );

      }

    }
    if ( verbose )
    {

      std::cout << TR << " seconds" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building average BOLD data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building average scan count : " << std::flush;

    }
    gkg::Volume< int16_t > averageBold( sizeX, sizeY, sizeZ );
    averageBold.getHeader().addAttribute( "resolutionX", resolutionX );
    averageBold.getHeader().addAttribute( "resolutionY", resolutionY );
    averageBold.getHeader().addAttribute( "resolutionZ", resolutionZ );

    int32_t x, y, z, t;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          double accumulator = 0.0;
          for ( t = dummyScanCount; t < sizeT; t++ )
          {

            accumulator += ( double )bold( x, y, z, t );

          }
          averageBold( x, y, z ) = ( int16_t )( accumulator /
                                         ( double )( sizeT - dummyScanCount ) );

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( !fileNameAverageBOLD.empty() )
    {

      if ( verbose )
      {

       std::cout << "saving '" << fileNameAverageBOLD << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameAverageBOLD, averageBold,
                                        ascii );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int16_t > mask;
    if ( fileNameMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask from average BOLD data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        radius = 2.0 * std::min( resolutionX,
                                 std::min( resolutionY, resolutionZ ) );

      }
      gkg::ClosedMask< int16_t, int16_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( averageBold, mask, verbose );

      gkg::Writer::getInstance().write( "mask.ima", mask );
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
    // smoothing BOLD data
    ////////////////////////////////////////////////////////////////////////////

    if ( smoothingKernelSize > 0.0 )
    {

      if ( verbose )
      {

       std::cout << "smoothing with " << smoothingKernelSize
                 << " mm Gaussian kernel : " << std::flush;

      }
      gkg::DericheGaussianCoefficients< float > dericheGaussianCoefficients(
                         smoothingKernelSize,
                         gkg::DericheGaussianCoefficients< float >::Smoothing );
      if ( smoothingAxis.find( "x" ) != std::string::npos )
      {

        if ( verbose )
        {

           std::cout << "x" << std::flush;

        }
        gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  bold,
                                  dericheGaussianCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::X_AXIS,
                                  bold );

      }
      if ( smoothingAxis.find( "y" ) != std::string::npos )
      {

        if ( verbose )
        {

           std::cout << "y" << std::flush;

        }
        gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  bold,
                                  dericheGaussianCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::Y_AXIS,
                                  bold );

      }
      if ( smoothingAxis.find( "z" ) != std::string::npos )
      {

        if ( verbose )
        {

           std::cout << "z" << std::flush;

        }
        gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  bold,
                                  dericheGaussianCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::Z_AXIS,
                                  bold );

      }
      if ( verbose )
      {

        std::cout << " done" << std::endl;

      }

      if ( !fileNameSmoothedBOLD.empty() )
      {

        if ( verbose )
        {

          std::cout << "saving '" << fileNameSmoothedBOLD << "' : "
                    << std::flush;

        }
        gkg::Writer::getInstance().write( fileNameSmoothedBOLD, bold, ascii );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // building paradigm
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building paradigm : " << std::flush;

    }

    gkg::Paradigm paradigm;
    gkg::Reader::getInstance().read( fileNameParadigm, paradigm );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

      std::cout << "regressor count : "
                << paradigm.getRegressorCount()
                << std::endl;

      std::cout << "stimuli list description :" << std::endl << std::endl;

      std::list< gkg::Stimulus* >::const_iterator
        s = paradigm.getStimuli().begin(),
        se = paradigm.getStimuli().end();
      while ( s != se )
      {

        if ( dynamic_cast< gkg::DiscreteStimulus* >( *s ) )
        {

          gkg::DiscreteStimulus* discreteStimulus = 
            dynamic_cast< gkg::DiscreteStimulus* >( *s );

          std::cout << "  "
                    << discreteStimulus->getName()
                    << std::endl;
          std::cout << "    type = discrete"
                    << std::endl;
          std::cout << "    times = "
                    << discreteStimulus->getTimes()
                    << std::endl;
          std::cout << "    durations = "
                    << discreteStimulus->getDurations()
                    << std::endl;
          std::cout << "    magnitudes = "
                    << discreteStimulus->getMagnitudes()
                    << std::endl;

        }
        else if ( dynamic_cast< gkg::ContinuousStimulus* >( *s ) )
        {

          gkg::ContinuousStimulus* continuousStimulus = 
            dynamic_cast< gkg::ContinuousStimulus* >( *s );

          std::cout << "  "
                    << continuousStimulus->getName()
                    << std::endl;
          std::cout << "    type = continuous"
                    << std::endl;
          std::cout << "    starting_time = "
                    << continuousStimulus->getStartingTime()
                    << std::endl;
          std::cout << "    sampling_period = "
                    << continuousStimulus->getSamplingPeriod()
                    << std::endl;
          std::cout << "    magnitudes = "
                    << continuousStimulus->getMagnitudes()
                    << std::endl;

        }

        std::cout << std::endl;
        ++ s;

      }


    }


    ////////////////////////////////////////////////////////////////////////////
    // building hemodynamic response function
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building canonical HRF : " << std::flush;

    }
    gkg::CanonicalHRF canonicalHRF( hrfDelay,
                                    hrfDelayUndershoot,
                                    hrfDispersion,
                                    hrfDispersionUndershoot,
                                    hrfRatio );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( !fileNameHRF.empty() )
    {

      if ( verbose )
      {

       std::cout << "saving '" << fileNameHRF << "' : " << std::flush;

      }
      int32_t hrfCount = ( int32_t )( GKG_BOLD_HRF_DURATION /
                                      GKG_BOLD_HRF_CONVOLUTION_STEP );
      std::ofstream os( fileNameHRF.c_str() );
      for ( t = 0; t < hrfCount; t++ )
      {

        os << canonicalHRF.getValue( t * GKG_BOLD_HRF_CONVOLUTION_STEP )
           << std::endl;

      }
      os.close();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building scan time vector
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building scan time vector : " << std::flush;

    }
    int32_t effectiveScanCount = sizeT - dummyScanCount;
    gkg::Vector scanTimes( effectiveScanCount );
    for ( t = 0; t < effectiveScanCount; t++ )
    {

      scanTimes( t ) = startingTime + ( t + dummyScanCount ) * TR;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "scan times : " << scanTimes << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building slice order vector  (to be managed in the future!)
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {

      std::cout << "finding slice order : " << std::flush;

    }
    std::vector< int32_t > sliceOrder( sizeZ );
    for ( z = 0; z < sizeZ; z++ )
    {

      sliceOrder[ z ] = z;

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "slice order : " << std::flush;
      for ( z = 0; z < sizeZ; z++ )
      {

         std::cout << sliceOrder[ z ] << " ";

      }
      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building Kalman lookup table
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building Kalman lookup table : " << std::flush;

    }
    std::vector< gkg::Vector3d< int32_t > > sites;
    gkg::Volume< gkg::Kalman::Parameters* > kalmanLut( sizeX, sizeY, sizeZ );
    kalmanLut.getHeader().addAttribute( "resolutionX", resolutionX );
    kalmanLut.getHeader().addAttribute( "resolutionY", resolutionY );
    kalmanLut.getHeader().addAttribute( "resolutionZ", resolutionZ );
    kalmanLut.fill( 0 );
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( mask( x, y, z ) )
          {

            kalmanLut( x, y, z ) = new gkg::Kalman::Parameters(
                                              paradigm.getRegressorCount() + 1 +
                                              driftPolynomOrder );
            sites.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "Kalman lookup table site count : "
                << sites.size()
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building design matrix
    ////////////////////////////////////////////////////////////////////////////

    gkg::Matrix designMatrix;
    paradigm.getDesignMatrix( scanTimes,
                              canonicalHRF,
                              driftPolynomOrder,
                              designMatrix,
                              verbose );

    if ( !fileNameDesignMatrix.empty() )
    {

      if ( verbose )
      {

        std::cout << "converting design matrix to image : " << std::flush;

      }
      gkg::Volume< float > dm( designMatrix.getSize2() * 20,
                               designMatrix.getSize1() );
      dm.getHeader().addAttribute( "resolutionX", 2.0 );
      dm.getHeader().addAttribute( "resolutionY", 2.0 );
      int32_t s1, s2, l;
      for ( s1 = 0; s1 < designMatrix.getSize1(); s1++ )
      {

        for ( s2 = 0; s2 < designMatrix.getSize2(); s2++ )
        {

          for ( l = 0; l < 20; l++ )
          {

            dm( s2 * 20 + l, s1 ) = ( float )designMatrix( s1, s2 );

          }

        }

      }                      
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( verbose )
      {

        std::cout << "saving '" << fileNameDesignMatrix << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameDesignMatrix, dm, ascii );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading contrast effects
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading contrast effects : " << std::flush;

    }
    gkg::ContrastEffects contrastEffects;
    gkg::Reader::getInstance().read( fileNameContrastEffects, contrastEffects );
    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "contrasts list description :" << std::endl << std::endl;

      int32_t index;
      for ( index = 0; index < contrastEffects.getContrastCount(); index++ )
      {

        const gkg::ContrastEffects::Information&
          information = contrastEffects.getInformation( index );

        std::cout << "  "
                  << information.name
                  << std::endl;

        std::cout << "    regressor names : " << std::flush;
        int32_t r = 0;
        for ( r = 0; r < ( int32_t )information.regressorNames.size(); r++ )
        {

          std::cout << information.regressorNames[ r ] << " ";

        }
        std::cout << std::endl;

        std::cout << "    weights : " << std::flush;
        int32_t w = 0;
        for ( w = 0; w < information.weights.getSize(); w++ )
        {

          std::cout << information.weights( w ) << " ";

        }
        std::cout << std::endl;
        std::cout << std::endl;

      }

    }

    contrastEffects.setParadigm( paradigm );

    ////////////////////////////////////////////////////////////////////////////
    // building General Linear Model
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building general linear model : " << std::flush;

    }
    gkg::BoldGeneralLinearModelSolver glm( designMatrix,
                                           scanTimes );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // solving General Linear Model
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "solving general linear model : " << std::flush;

    }
    glm.solve( bold, dummyScanCount, sliceOrder, kalmanLut, sites, verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing contrast effects and their variances
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "processing contrast effects : " << std::flush;

    }
    gkg::Volume< float > volumeOfEffects;
    gkg::Volume< float > volumeOfEffectVariances;
    contrastEffects.process( kalmanLut,
                             volumeOfEffects,
                             volumeOfEffectVariances );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving contrast effects
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving contrast effects : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameEffects, volumeOfEffects, ascii );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving contrast effect variances
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving contrast effect variances : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameEffectVariances,
                                      volumeOfEffectVariances,
                                      ascii );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing and saving statistical maps
    ////////////////////////////////////////////////////////////////////////////


    int32_t contrastCount = contrastEffects.getContrastCount();
    gkg::Volume< float > outputVolume;
    std::vector< std::string >::const_iterator 
      m = mapAndName.begin(),
      me = mapAndName.end();

    while ( m != me )
    {

      std::string statisticalFunctionName = *m;
      ++ m;
      std::string outputFileName = *m;
      ++ m;

      gkg::StatisticalFunctionFactory::getInstance().applyFunction(
                                                        statisticalFunctionName,
                                                        effectiveScanCount,
                                                        contrastCount,
                                                        volumeOfEffects,
                                                        volumeOfEffectVariances,
                                                        outputVolume );

      if ( verbose )
      {

        std::cout << "saving " << statisticalFunctionName 
                  << " map : " << std::flush;

      }
      gkg::Writer::getInstance().write( outputFileName,
                                        outputVolume,
                                        ascii,
                                        volumeFormat );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // deleting Kalman loopkup table items
    ////////////////////////////////////////////////////////////////////////////

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( kalmanLut( x, y, z ) )
          {

            delete kalmanLut( x, y, z );

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::BoldAnalysisCommand::execute( "
             "const std::string& fileNameBOLD, "
             "const std::string& fileNameAverageBOLD, "
             "const std::string& fileNameSmoothedBOLD, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameParadigm, "
             "const std::string& fileNameContrastEffects, "
             "const std::string& fileNameDesignMatrix, "
             "const std::string& fileNameEffects, "
             "const std::string& fileNameEffectVariances, "
             "const std::vector< std::string >& mapAndName, "
             "double startingTime, double repetitionTime, "
             "int32_t dummyScanCount, int32_t driftPolynomOrder, "
             "float smoothingFWHM, const std::string& smoothingAxis, "
             "const std::string& fileNameHRF, double hrfDelay, "
             "double hrfDelayUndershoot, double hrfDispersion, "
             "double hrfDispersionUndershoot, double hrfRatio, "
             "bool ascii, const std::string& volumeFormat, bool verbose )" );

}


RegisterCommandCreator( 
    BoldAnalysisCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameBOLD ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameAverageBOLD ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameSmoothedBOLD ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameParadigm ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameHRF ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDesignMatrix ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameContrastEffects ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameEffects ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameEffectVariances ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( mapAndName ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_FLOATING_PARAMETER_HELP( startingTime ) +
    DECLARE_FLOATING_PARAMETER_HELP( repetitionTime ) +
    DECLARE_INTEGER_PARAMETER_HELP( dummyScanCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( driftPolynomOrder ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_FLOATING_PARAMETER_HELP( smoothingFWHM ) +
    DECLARE_FLOATING_PARAMETER_HELP( smoothingKernelSize ) +
    DECLARE_STRING_PARAMETER_HELP( smoothingAxis ) +
    DECLARE_FLOATING_PARAMETER_HELP( hrfDelay ) +
    DECLARE_FLOATING_PARAMETER_HELP( hrfDelayUndershoot ) +
    DECLARE_FLOATING_PARAMETER_HELP( hrfDispersion ) +
    DECLARE_FLOATING_PARAMETER_HELP( hrfDispersionUndershoot ) +
    DECLARE_FLOATING_PARAMETER_HELP( hrfRatio ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
