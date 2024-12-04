#include <gkg-mri-reconstruction-plugin-functors/BrukerEpiReconstructor/BrukerEpiReconstructorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-processing-signal/Scrolling3d.h>
#include <gkg-mri-reconstruction-core/HalfKSpaceReconstructor2d.h>
#include <gkg-mri-reconstruction-correction/ModulationTransferFunction.h>
#include <gkg-mri-reconstruction-correction/DcOffsetCorrection.h>
#include <gkg-mri-reconstruction-correction/TrapezoidROGradient.h>
#include <gkg-mri-reconstruction-correction/RegridingCorrection.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-algobase/RealPartConverter_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iomanip>


//
// void reorderingLines()
//

static void reorderingLines( gkg::Volume< std::complex< float > >& rawVolume )
{

  int32_t sizeX = rawVolume.getSizeX();
  int32_t lineCount = rawVolume.getSizeY();
  int32_t sliceCount = rawVolume.getSizeZ();

  // 1) even line are time-flipped ( -> sizeX - 1 - x )
  // 2) we reorder lines because k-space lines are acquired from ky=-overscan
  //    to ky=+kymax/2, and they are stored in the Bruker raw, using that order
  //    ( -> lineCount - 1 - line )

  int32_t x, line, slice;

  std::vector< std::complex< float > > tmpLine( sizeX );

  // processing even lines (0, 2, 4, 6, 8, ...)
  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( line = 0; line < lineCount; line += 2 )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        tmpLine[ x ] = rawVolume( x, lineCount - 1 - line, slice );

      }

      for ( x = 0; x < sizeX; x++ )
      {

        rawVolume( sizeX - 1 - x, lineCount - 1 - line, slice ) =
        rawVolume( x, line, slice );

      }

      for ( x = 0; x < sizeX; x++ )
      {

        rawVolume( x, line, slice ) = tmpLine[ x ];

      }

    }

  }

}


//
// void copyRaw2Rec()
//

static void copyRaw2Rec( const gkg::Volume< std::complex< float > >& rawVolume,
                         gkg::Volume< std::complex< float > >& recVolume,
                         bool verbose )
{

  try
  {

    int32_t sizeX = rawVolume.getSizeX();
    int32_t lineCount = rawVolume.getSizeY();
    int32_t sliceCount = rawVolume.getSizeZ();

    int32_t x, line, slice;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          recVolume( x, line, slice ) = rawVolume( x, line, slice );

        }

      }
      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  catch ( std::exception& e )
  {

    throw std::runtime_error( std::string( "copyRaw2Rec :" ) + e.what() );

  }

}


//
// class BrukerEpiReconstructorCommand
//

gkg::BrukerEpiReconstructorCommand::BrukerEpiReconstructorCommand(
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
  GKG_CATCH( "gkg::BrukerEpiReconstructorCommand::"
             "BrukerEpiReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrukerEpiReconstructorCommand::BrukerEpiReconstructorCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
                                            bool noTrajectoryCorrection,
                                            float forcedYScrollingRatio,
                                            bool applyMTF,
                                            bool keepFirstMTF,
                                            int32_t referenceSliceMTF,
                                            bool useCentreSliceMTF,
                                            const std::string& ppg,
                                            const std::string& outputFormat,
                                            bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             noTrajectoryCorrection,
             forcedYScrollingRatio,
             applyMTF,
             keepFirstMTF,
             referenceSliceMTF,
             useCentreSliceMTF,
             ppg,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerEpiReconstructorCommand::"
             "BrukerEpiReconstructorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "bool noTrajectoryCorrection, "
             "float forcedYScrollingRatio, "
             "bool applyMTF, "
             "bool keepFirstMTF, "
             "int32_t referenceSliceMTF, "
             "bool useCentreSliceMTF, "
             "const std::string& ppg, "
             "const std::string& outputFormat, "
             "bool verbose  )" );

}


gkg::BrukerEpiReconstructorCommand::BrukerEpiReconstructorCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, xApodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, yApodizationName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeY );
    DECLARE_FLOATING_PARAMETER( parameters, float, scale );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, noTrajectoryCorrection );
    DECLARE_FLOATING_PARAMETER( parameters, float, forcedYScrollingRatio );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, applyMTF );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, keepFirstMTF );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, referenceSliceMTF );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, useCentreSliceMTF );
    DECLARE_STRING_PARAMETER( parameters, std::string, ppg );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             fileNameOut,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             noTrajectoryCorrection,
             forcedYScrollingRatio,
             applyMTF,
             keepFirstMTF,
             referenceSliceMTF,
             useCentreSliceMTF,
             ppg,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerEpiReconstructorCommand::"
             "BrukerEpiReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrukerEpiReconstructorCommand::~BrukerEpiReconstructorCommand()
{
}


std::string gkg::BrukerEpiReconstructorCommand::getStaticName()
{

  try
  {

    return "BrukerEpiReconstructor";

  }
  GKG_CATCH( "std::string gkg::BrukerEpiReconstructorCommand::getStaticName()" );

}


void gkg::BrukerEpiReconstructorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string xApodizationName = "tukey";
    std::string yApodizationName = "tukey";
    int32_t reconstructionSizeX = -1;
    int32_t reconstructionSizeY = -1;
    float scale = 0.01;
    bool noTrajectoryCorrection = false;
    float forcedYScrollingRatio = 10000.0;
    bool applyMTF = true;
    bool keepFirstMTF = true;
    int32_t referenceSliceMTF = -1;
    bool useCentreSliceMTF = true;
    std::string ppg = "irmfmrs";
    std::string outputFormat = "gis";
    bool verbose = false;

    std::string apodizationNames;
    std::list< std::string > apodizationNameList = 
      gkg::ApodizationFactory< float >::getInstance().getNames();
    std::list< std::string >::const_iterator n = apodizationNameList.begin(),
                                             ne = apodizationNameList.end();
    while ( n != ne )
    {

      apodizationNames += *n + '/';
      ++ n;

    }
    apodizationNames = apodizationNames.substr( 0,
                                                apodizationNames.length() - 1 );

    gkg::Application application( _argc, _argv,
                                  "Reconstructor for Bruker 3T GRE/SE"
                                  " EchoPlanar pulse sequence",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input Bruker raw data directory name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Reconstructed GIS image file name",
                                 fileNameOut );
    application.addSingleOption( "-ax",
                                 std::string( "X apodization (" ) +
                                 apodizationNames + ") " +
                                 "(default=tukey)",
                                 xApodizationName,
                                 true );
    application.addSingleOption( "-ay",
                                 std::string( "Y apodization (" ) +
                                 apodizationNames + ") " +
                                 "(default=tukey)",
                                 yApodizationName,
                                 true );
    application.addSingleOption( "-sx",
                                 "Reconstruction X size "
                                 "(default=closest upper power of two)",
                                 reconstructionSizeX,
                                 true );
    application.addSingleOption( "-sy",
                                 "Reconstruction Y size "
                                 "(default=closest upper power of two)",
                                 reconstructionSizeY,
                                 true );
    application.addSingleOption( "-scale",
                                 "Scale factor applied to reconstructed data"
                                 " (default=0.1)",
                                 scale,
                                 true );
    application.addSingleOption( "-notrajectory",
                                 "Not correcting for trajectories"
                                 " (default=false)",
                                 noTrajectoryCorrection,
                                 true );
    application.addSingleOption( "-yScrollingRatio",
                                 "Scrolling ratio along Y"
                                 " (default=0.25 for mecaDTI1128, 0.0 else)",
                                 forcedYScrollingRatio,
                                 true );
    application.addSingleOption( "-applyMTF",
                                 "Apply MTF correction (default=true)",
                                 applyMTF,
                                 true );
    application.addSingleOption( "-keepFirstMTF",
                                 "Keep MTF result on the first rank for the "
                                 "following rank(s) (default=true)",
                                 keepFirstMTF,
                                 true );
    application.addSingleOption( "-referenceSliceMTF",
                                 "Use the MTF estimates for the given slice "
                                 "and apply it to all others slices "
                                 "(default=not used)",
                                 referenceSliceMTF,
                                 true );
    application.addSingleOption( "-useCentreSliceMTF",
                                 "Use the MTF estimates from the centre slice "
                                 "and apply it to all others slices "
                                 "(default=true)",
                                 useCentreSliceMTF,
                                 true );
    application.addSingleOption( "-ppg",
                                 "Pulse program name, if not 'irmfmrs' "
                                 "look at the PULPROG ppg name to check "
                                 " it is coherent; if set to 'irmfmrs', do"
                                 " not check the consistency and assume it is "
                                 "and IRMFMRS' pulse sequence "
                                 "(default=irmfmrs)",
                                 ppg,
                                 true );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             noTrajectoryCorrection,
             forcedYScrollingRatio,
             applyMTF,
             keepFirstMTF,
             referenceSliceMTF,
             useCentreSliceMTF,
             ppg,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrukerEpiReconstructorCommand::parse()" );

}


void gkg::BrukerEpiReconstructorCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
                                            bool noTrajectoryCorrection,
                                            float forcedYScrollingRatio,
                                            bool applyMTF,
                                            bool keepFirstMTF,
                                            int32_t referenceSliceMTF,
                                            bool useCentreSliceMTF,
                                            const std::string& ppg,
                                            const std::string& outputFormat,
                                            bool verbose )
{

  try
  {

    std::string apodizationNames;
    std::list< std::string > apodizationNameList = 
      gkg::ApodizationFactory< float >::getInstance().getNames();
    std::list< std::string >::const_iterator n = apodizationNameList.begin(),
                                             ne = apodizationNameList.end();
    while ( n != ne )
    {

      apodizationNames += *n + '/';
      ++ n;

    }
    apodizationNames = apodizationNames.substr( 0,
                                                apodizationNames.length() - 1 );

    // sanity checks
    if ( !gkg::ApodizationFactory< float >::getInstance().hasApodization(
                                                         xApodizationName ) )
    {

      throw std::runtime_error(
              std::string( "X apodization should be one of " ) +
              apodizationNames );

    }
    if ( !gkg::ApodizationFactory< float >::getInstance().hasApodization(
                                                         yApodizationName ) )
    {

      throw std::runtime_error( 
              std::string( "Y apodization should be one of " ) +
              apodizationNames );

    }
/*
    if ( reconstructionSizeX != -1 )
    {

      int32_t powerOfTwo = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                                         reconstructionSizeX );
      if ( powerOfTwo != reconstructionSizeX )
      {

        if ( verbose )
        {

          std::cout << "warning: taking " << powerOfTwo
                    << " instead of " << reconstructionSizeX
                    << " for X reconstruction size"
                    << std::endl;

        }
        reconstructionSizeX = powerOfTwo;

      }

    }

    if ( reconstructionSizeY != -1 )
    {

      int32_t powerOfTwo = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                                         reconstructionSizeY );
      if ( powerOfTwo != reconstructionSizeY )
      {

        if ( verbose )
        {

          std::cout << "warning: taking " << powerOfTwo
                    << " instead of " << reconstructionSizeY
                    << " for Y reconstruction size"
                    << std::endl;

        }
        reconstructionSizeY = powerOfTwo;

      }

    }
*/
    // checking that data is readable and collecting sizes
    if ( verbose )
    {

      std::cout << "reading Bruker raw file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< std::complex< float > > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                       gkg::Volume< std::complex< float > > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "bruker-raw" )
    {

      throw std::runtime_error( "wrong format, should be \'bruker-raw\'");

    }

    // check that pulse program is one of the good EPI pulse program
    // and managing EPI attributes
    std::string pulseProgramName = "";
    inputVolumeProxy.getHeader().getAttribute( "PULPROG", pulseProgramName );

    double fovX = 0.0, fovY = 0.0, slthick = 0.0;
    int32_t overScanCount = 0;
    bool partialFourier = false;
    float yScrollingRatio = 0.0;

    std::cout << "pulse program = " << pulseProgramName << std::endl;

    if ( pulseProgramName == "fh_inepi.ppg" )  // old PV1 sequence from F.Hennel
    {

      // it is always full k-space
      partialFourier = false;

      // overScanCount is half Y size
      overScanCount = inputVolumeProxy.getSizeY() / 2;

      // getting field of view and slice thickness
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      std::vector< double > ACQ_slice_sepn;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_sepn",
                                                 ACQ_slice_sepn );
      slthick = ACQ_slice_sepn[ 0 ];

      if ( forcedYScrollingRatio != 10000.0 )
      {

        yScrollingRatio = forcedYScrollingRatio;

      }
      else
      {

        yScrollingRatio = 0.0;

      }

    }
    else if ( pulseProgramName == "epi_shfj.ppg" )
    {

      // is it full or partial Fourier?
      std::string partialFourierEnum = "";
      if ( inputVolumeProxy.getHeader().hasAttribute( "PartialFourierAcq" ) )
      {

        inputVolumeProxy.getHeader().getAttribute( "PartialFourierAcq",
                                                   partialFourierEnum );
        partialFourier = (  partialFourierEnum == "No" ? false : true );

      }
      else
      {

        partialFourier = false;

      }

      // adding overScanCount for EPI data
      if ( !partialFourier )
      {

       overScanCount = inputVolumeProxy.getSizeY() / 2;

      }
      else
      {

        inputVolumeProxy.getHeader().getAttribute( "N_overscans", 
                                                   overScanCount );

      }

      // getting fov and slice thickness
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      std::vector< double > ACQ_slice_sepn;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_sepn",
                                                 ACQ_slice_sepn );
      slthick = ACQ_slice_sepn[ 0 ];

      std::vector< double > PVM_SPackArrPhase1Offset;
      inputVolumeProxy.getHeader().getAttribute( "PVM_SPackArrPhase1Offset",
                                                 PVM_SPackArrPhase1Offset );

      if ( forcedYScrollingRatio != 10000.0 )
      {

        yScrollingRatio = forcedYScrollingRatio;

      }
      else
      {

        yScrollingRatio = ( float )PVM_SPackArrPhase1Offset[ 0 ] /
                          ( float )fovY;

      }

    }
    else if ( ( pulseProgramName == "mecaDTI1128.ppg" ) ||
              ( pulseProgramName == "mecaDTI.ppg" ) )
    {

      // is it full or partial Fourier?
      double echoPosition;
      inputVolumeProxy.getHeader().getAttribute( "PVM_EpiEchoPosition",
                                                 echoPosition );
      partialFourier = (  echoPosition > 0.5 ? false : true );

      // adding overScanCount for EPI data
      if ( !partialFourier )
      {

       overScanCount = inputVolumeProxy.getSizeY() / 2;

      }
      else
      {

        overScanCount = inputVolumeProxy.getSizeY() -
                        gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                            inputVolumeProxy.getSizeY() );

      }

      // getting fov and slice thickness
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      std::vector< double > ACQ_slice_sepn;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_sepn",
                                                 ACQ_slice_sepn );
      slthick = ACQ_slice_sepn[ 0 ];

      if ( forcedYScrollingRatio != 10000.0 )
      {

        yScrollingRatio = forcedYScrollingRatio;

      }
      else
      {

        yScrollingRatio = 0.25;

      }

    }
    else if ( ppg == "irmfmrs" )
    {

      // is it full or partial Fourier?
      double echoPosition;
      inputVolumeProxy.getHeader().getAttribute( "PVM_EpiEchoPosition",
                                                 echoPosition );
      partialFourier = (  echoPosition / 100.0 > 0.5 ? false : true );

      // adding overScanCount for EPI data
      if ( !partialFourier )
      {

       overScanCount = inputVolumeProxy.getSizeY() / 2;

      }
      else
      {

        if ( gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                            inputVolumeProxy.getSizeX() ) ==
             inputVolumeProxy.getSizeX() )
        {

          overScanCount = inputVolumeProxy.getSizeY() -
                        gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                            inputVolumeProxy.getSizeY() );

        }
        else
        {

          overScanCount = inputVolumeProxy.getSizeY() -
                          inputVolumeProxy.getSizeX() / 2;

        }

      }

      // getting fov and slice thickness
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      std::vector< double > ACQ_slice_sepn;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_sepn",
                                                 ACQ_slice_sepn );
      std::vector< double > ACQ_phase1_offset;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_phase1_offset",
                                                 ACQ_phase1_offset );
      slthick = ACQ_slice_sepn[ 0 ];

      if ( forcedYScrollingRatio != 10000.0 )
      {

        yScrollingRatio = forcedYScrollingRatio;

      }
      else
      {

        yScrollingRatio = ( float )ACQ_phase1_offset[ 0 ] / fovY;

      }

    }
    else if ( pulseProgramName == ppg )
    {

      // is it full or partial Fourier?
      double echoPosition;
      inputVolumeProxy.getHeader().getAttribute( "PVM_EpiEchoPosition",
                                                 echoPosition );
      partialFourier = (  echoPosition / 100.0 > 0.5 ? false : true );

      // adding overScanCount for EPI data
      if ( !partialFourier )
      {

       overScanCount = inputVolumeProxy.getSizeY() / 2;

      }
      else
      {

        if ( gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                            inputVolumeProxy.getSizeX() ) ==
             inputVolumeProxy.getSizeX() )
        {

          overScanCount = inputVolumeProxy.getSizeY() -
                        gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                            inputVolumeProxy.getSizeY() );

        }
        else
        {

          overScanCount = inputVolumeProxy.getSizeY() -
                          inputVolumeProxy.getSizeX() / 2;

        }

      }

      // getting fov and slice thickness
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      std::vector< double > ACQ_slice_sepn;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_sepn",
                                                 ACQ_slice_sepn );
      std::vector< double > ACQ_phase1_offset;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_phase1_offset",
                                                 ACQ_phase1_offset );
      slthick = ACQ_slice_sepn[ 0 ];

      if ( forcedYScrollingRatio != 10000.0 )
      {

        yScrollingRatio = forcedYScrollingRatio;

      }
      else
      {

        yScrollingRatio = ( float )ACQ_phase1_offset[ 0 ] / fovY;

      }

    }
    else
    {

      throw std::runtime_error( "not a Bruker EPI pulse program");

    }

    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();


    // sanity check
    if ( referenceSliceMTF >= sliceCount )
    {

      throw std::runtime_error( "bad reference slice for MTF" );

    }
         

    // collecting ramp sampling information if ramp sampling on
    bool rampSamplingOn = false;
    float rampTime = 0.0, acquisitionLength = 0.0, topFlatTime = 0.0;
    int32_t effectiveSizeX = sizeX;

    if ( !noTrajectoryCorrection )
    {

      // collecting ramp sampling information if ramp sampling on
      double SW_h = 0.0, ramp_time = 0.0;
      inputVolumeProxy.getHeader().getAttribute( "SW_h", SW_h );
      if ( inputVolumeProxy.getHeader().hasAttribute( "EPI_ramp_time" ) )
      {

        inputVolumeProxy.getHeader().getAttribute( "EPI_ramp_time", ramp_time );

      }
      else if ( inputVolumeProxy.getHeader().hasAttribute( "PREEMP_ramp_time" ))
      {

        inputVolumeProxy.getHeader().getAttribute( "PREEMP_ramp_time",
                                                   ramp_time);

      }
      else if ( inputVolumeProxy.getHeader().hasAttribute( "PVM_EpiRampTime" ))
      {

        inputVolumeProxy.getHeader().getAttribute( "PVM_EpiRampTime",
                                                   ramp_time);
        ramp_time *= 1000.0;  // ms to us convertion

      }
      if ( ramp_time == 0.0 )
      {

        rampSamplingOn = false;

      }
      else
      {

        rampSamplingOn = true;
        rampTime = ( float )ramp_time;
        acquisitionLength = 1e6 * ( float )sizeX / ( float )SW_h;
        topFlatTime = acquisitionLength - 2 * rampTime;
        //effectiveSizeX = -1;

      }

    }

    if ( verbose )
    {

      std::cout << "raw data geometry: "
                << sizeX << " x "
                << lineCount << " x "
                << sliceCount << " x "
                << timeCount << std::endl;
      std::cout << "overscan count: "
                << overScanCount
                << std::endl;
      std::cout << "field of view: "
                << fovX << " mm x "
                << fovY << " mm"
                << std::endl;
      std::cout << "slice thickness: "
                << slthick << " mm"
                << std::endl;
      std::cout << "Y scrolling ratio: "
                << yScrollingRatio
                << std::endl;
      if ( !noTrajectoryCorrection )
      {

        std::cout << "ramp sampling: "
                  << ( rampSamplingOn ? "on" : "off" )
                  << std::endl;

        if ( rampSamplingOn )
        {

          std::cout << "Sampling period in us: "
                    << acquisitionLength / sizeX
                    << std::endl;
          std::cout << "Acquisition length in us: "
                    << acquisitionLength
                    << std::endl;
          std::cout << "Ramp time in us: "
                    << rampTime
                    << std::endl;
          std::cout << "Top flat time in us: "
                    << topFlatTime
                    << std::endl;

        }

      }

    }

    // calculating reconstructed image size(s)
    if ( verbose )
    {

      std::cout << "evaluating reconstructed image size(s): " << std::flush;

    }
    if ( reconstructionSizeX == -1 )
    {

      reconstructionSizeX = gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                                             effectiveSizeX );

    }
    if ( reconstructionSizeY == -1 )
    {

      reconstructionSizeY = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                            ( lineCount - overScanCount ) * 2 );

    }
    reconstructionSizeY = std::max( reconstructionSizeY,
                                    ( lineCount - overScanCount ) * 2 );
    if ( verbose )
    {

      std::cout << reconstructionSizeX << " x "
                << reconstructionSizeY << " x "
                << sliceCount << " x "
                << timeCount << std::endl;

    }

    // creating apodization
    if ( verbose )
    {

      std::cout << "creating apodization(s): " << std::flush;

    }
    int32_t regridingSizeX = ( sizeX <= reconstructionSizeX ? sizeX :
                                                          reconstructionSizeX );
    int32_t xApodizationSize = regridingSizeX;
    int32_t yApodizationSize = ( lineCount - overScanCount ) * 2;
    gkg::Apodization< float >* xApodization =
      gkg::ApodizationFactory< float >::getInstance().createApodization(
                                          xApodizationName,
                                          xApodizationSize,
                                          gkg::Apodization< float >::Centered );
    gkg::Apodization< float >* yApodization =
        gkg::ApodizationFactory< float >::getInstance().createApodization(
                                          yApodizationName,
                                          yApodizationSize,
                                          gkg::Apodization< float >::Centered );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, lineCount - 1,
                                                  0, sliceCount - 1,
                                                  0, 0 );
    gkg::Volume< std::complex< float > > rawVolume;

    gkg::Volume< std::complex< float > > recVolume( reconstructionSizeX,
                                                    reconstructionSizeY,
                                                    sliceCount );
    gkg::Volume< float > realPartVolume( reconstructionSizeX,
                                         reconstructionSizeY,
                                         sliceCount );
    gkg::Volume< float > magnitudeVolume( reconstructionSizeX,
                                          reconstructionSizeY,
                                          sliceCount );
    gkg::Volume< int16_t > shortVolume( reconstructionSizeX,
                                        reconstructionSizeY,
                                        sliceCount );

    // preparing converter(s)
    gkg::RealPartConverter< gkg::Volume< std::complex< float > >,
                            gkg::Volume< float > > realPartConverter;
    gkg::MagnitudeConverter< gkg::Volume< float >,
                             gkg::Volume< float > > magnitudeRealConverter;
    gkg::MagnitudeConverter< gkg::Volume< std::complex< float > >,
                             gkg::Volume< float > > magnitudeCplxConverter;
    gkg::Scaler< gkg::Volume< float > > scaler( scale );
    gkg::LowerOrEqualToFunction< float > lowerOrEqualToFunction( 32767.0 );
    gkg::GreaterOrEqualToFunction< float > greaterOrEqualToFunction( 0.0 );
    gkg::Thresholder< gkg::Volume< float > > lowerThresholder(
                                                       lowerOrEqualToFunction,
                                                       32767.0 );
    gkg::Thresholder< gkg::Volume< float > > greaterThresholder(
                                                       greaterOrEqualToFunction,
                                                       0.0 );
    gkg::Converter< gkg::Volume< float >, gkg::Volume< int16_t > > converter;


    // preparing modulation transfer function(s)
    std::vector< std::vector< std::complex< float > > >
      modulationTF( sliceCount );


    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< int16_t > outputVolumeProxy( reconstructionSizeX,
                                                   reconstructionSizeY,
                                                   sliceCount,
                                                   timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "item_type" ] =
      gkg::TypeOf< int16_t >::getName();
    outputVolumeProxy.getHeader()[ "sizeX" ] = reconstructionSizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = reconstructionSizeY;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
    outputVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
    outputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                                fovX / reconstructionSizeX );
    outputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                                fovY / reconstructionSizeY );
    outputVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                                slthick );
    outputVolumeProxy.getHeader().addAttribute( "resolutionT",
                                                ( double )1.0 );

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, reconstructionSizeX - 1,
                                                   0, reconstructionSizeY - 1,
                                                   0, sliceCount - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                   gkg::Volume< int16_t > >( fileNameOut,
                                                             outputVolumeProxy,
                                                             outputFormat ) !=
         outputFormat )
    {

      throw std::runtime_error(
              std::string( "failed to prepare partial write with format \' " ) +
              outputFormat + "\'" );

    }
    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    // preparing regriding correction
    gkg::RegridingCorrection< float >* regridingCorrection = 0;
    if ( !rampSamplingOn )
    {

      if ( regridingSizeX != sizeX )
      {

        if ( verbose )
        {

          std::cout << "preparing sinc regriding correction: "
                    << std::flush;

        }
        regridingCorrection = new gkg::RegridingCorrection< float >( sizeX );
        regridingCorrection->setOutputSampleCount( regridingSizeX,
                                                   ( float )effectiveSizeX );
        if ( verbose )
        {

          std::cout << "done " << std::endl;
          std::cout << "Effective X size: " << effectiveSizeX << std::endl;
        }

      }

    }
    else
    {

      if ( !noTrajectoryCorrection )
      {

        if ( verbose )
        {

          std::cout << "preparing ramp sampling correction: "
                    << std::flush;

        }
        gkg::TrapezoidROGradient< float > roGradient( sizeX,
                                                      rampTime,
                                                      topFlatTime );
        regridingCorrection = new gkg::RegridingCorrection< float >(
                                                   roGradient.getTrajectory() );
        regridingCorrection->setOutputSampleCount(
                                   regridingSizeX,
                                   regridingCorrection->getEffectiveSize( 0 ) );
        if ( verbose )
        {

          std::cout << "done " << std::endl;
          std::cout << "Effective X size: "
                    << regridingCorrection->getEffectiveSize( 0 ) << std::endl;

        }

      }
      else
      {

        if ( regridingSizeX != sizeX )
        {

          if ( verbose )
          {

            std::cout << "preparing sinc regriding correction: "
                      << std::flush;

          }
          regridingCorrection = new gkg::RegridingCorrection< float >( sizeX );
          regridingCorrection->setOutputSampleCount( regridingSizeX,
                                                     ( float )effectiveSizeX );
          if ( verbose )
          {

            std::cout << "done " << std::endl;
            std::cout << "Effective X size: " << effectiveSizeX << std::endl;

          }

        }

      }

    }

    // looping over ranks(s)
    if ( verbose )
    {

      std::cout << "reconstructing: "
                << std::flush;

    }

    int32_t time;
    std::complex< float > dcOffset( 0.0, 0.0 );
    for ( time = 0; time < timeCount; time++ )
    {

      if ( verbose )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << timeCount
                  << " ] " << std::flush;

      }

      // reading raw data from P-file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      format =
          gkg::Reader::getInstance().partialBinaryRead<
                                         gkg::Volume< std::complex< float > > >(
                                                         inputVolumeProxy,
                                                         rawVolume,
                                                         inputBoundingBox,
                                                         "bruker-raw" );
      if ( format != "bruker-raw" )
      {

        throw std::runtime_error( "wrong format, should be \'bruker-raw\'");

      }


      // reordering raw data, and zero-filling
      if ( verbose )
      {

        std::cout << "reordering lines" << std::flush;

      }
      reorderingLines( rawVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 16 ) << std::flush;

      }

      // removing constant part of complex signal (DC offset correction)
      if ( verbose )
      {

        std::cout << "DcOffset->" << std::flush;

      }
      gkg::DcOffsetCorrection< float >::getInstance().apply( 
                                                           rawVolume,
                                                           sizeX,
                                                           lineCount,
                                                           sliceCount,
                                                           &dcOffset );
      if ( verbose )
      {

        std::cout << std:: setw( 30 ) << dcOffset
                  << gkg::Eraser( 40 ) << std::flush;

      }

      // correction for ramp sampling if ramp sampling on
      recVolume.fill( std::complex< float >( 0.0, 0.0 ) );
      if ( regridingCorrection )
      {

        if ( verbose )
        {

          std::cout << "Regriding->" << std::flush;

        }
        regridingCorrection->apply( rawVolume, recVolume, verbose );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 11 ) << std::flush;

        }

      }
      else
      {

        if ( verbose )
        {

          std::cout << "Copying->" << std::flush;

        }
        copyRaw2Rec( rawVolume, recVolume, verbose );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 9 ) << std::flush;

        }

      }

      // estimating modulation transfer function(s)
      if ( applyMTF && 
           ( !keepFirstMTF || ( keepFirstMTF && ( time == 0 ) )  ) )
      {

        if ( verbose )
        {

          std::cout << "MTF estimation->" << std::flush;

        }


        gkg::ModulationTransferFunction< float >::getInstance().estimate(
                                                                recVolume,
                                                                modulationTF,
                                                                yScrollingRatio,
                                                                verbose );
        if ( useCentreSliceMTF )
        {

          int32_t slice = 0;
          for ( slice = 0; slice < sliceCount; slice++ )
          {

            if ( slice != sliceCount / 2 )
            {

              modulationTF[ slice ] = modulationTF[ sliceCount / 2 ];

            }

          }

        }
        else if ( referenceSliceMTF >= 0 )
        {

          int32_t slice = 0;
          for ( slice = 0; slice < sliceCount; slice++ )
          {

            if ( slice != referenceSliceMTF )
            {

              modulationTF[ slice ] = modulationTF[ referenceSliceMTF ];

            }

          }

        }

        if ( verbose )
        {

          std::cout << gkg::Eraser( 16 ) << std::flush;

        }

      }
      else
      {

        int32_t slice = 0;
        for ( slice = 0; slice < sliceCount; slice++ )
        {

          if ( slice != referenceSliceMTF )
          {

            modulationTF[ slice ] = std::vector< std::complex< float > >( 
                                                   recVolume.getSizeX(), 1.0f );

          }

        }

      }

      // reconstructing from Half K-space
      if ( verbose )
      {

        std::cout << "Fourier->" << std::flush;

      }
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().setOverScanCount(
                                                                overScanCount );
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().setRawLineCount(
                                                                    lineCount );
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().
                                               setModulationTransferFunction(
                                                                modulationTF );
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().
                                                             setYScrollingRatio(
                                                              yScrollingRatio );
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().setXApodization(
                                                                 xApodization );
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().setYApodization(
                                                                 yApodization );
      gkg::HalfKSpaceReconstructor2d< float >::getInstance().reconstruct(
                                                                 recVolume,
                                                                 recVolume,
                                                                 verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 9 ) << std::flush;

      }

      // taking magnitude of real or complex part
      if ( overScanCount > 0.35 * lineCount ) // no use of Half-Fourier transf.
      {

        if ( verbose )
        {

          std::cout << "Magnitude cplx part" << std::flush;

        }
        magnitudeCplxConverter.convert( recVolume, magnitudeVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 19 ) << std::flush;

        }

      }
      else
      {

        if ( verbose )
        {

          std::cout << "Magnitude real part" << std::flush;

        }
        realPartConverter.convert( recVolume, realPartVolume );
        magnitudeRealConverter.convert( realPartVolume, magnitudeVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 19 ) << std::flush;

        }

      }

      // rescaling and converting to short
      if ( verbose )
      {

        std::cout << "Rescaling+short convertion" << std::flush;

      }
      scaler.scale( magnitudeVolume );
      lowerThresholder.threshold( magnitudeVolume, magnitudeVolume );
      greaterThresholder.threshold( magnitudeVolume, magnitudeVolume );
      converter.convert( magnitudeVolume, shortVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 26 ) << std::flush;

      }

      // saving reconstructed slice to disk
      if ( verbose )
      {

        std::cout << "Saving to disk" << std::flush;

      }
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );

      gkg::Writer::getInstance().partialBinaryWrite< gkg::Volume< int16_t > >(
                                                              outputVolumeProxy,
                                                              shortVolume,
                                                              outputBoundingBox,
                                                              outputFormat );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 14 ) << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 ) << "done" << std::endl;

    }

    // deleting allocated object
    delete xApodization;
    delete yApodization;
    if ( regridingCorrection )
    {

      delete regridingCorrection;

    }

  }
  GKG_CATCH( "void gkg::BrukerEpiReconstructorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "bool noTrajectoryCorrection, "
             "float forcedYScrollingRatio, "
             "bool applyMTF, "
             "bool keepFirstMTF, "
             "int32_t referenceSliceMTF, "
             "bool useCentreSliceMTF, "
             "const std::string& ppg, "
             "const std::string& outputFormat, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    BrukerEpiReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_FLOATING_PARAMETER_HELP( forcedYScrollingRatio ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( applyMTF ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( keepFirstMTF ) +
    DECLARE_INTEGER_PARAMETER_HELP( referenceSliceMTF ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( useCentreSliceMTF ) +
    DECLARE_STRING_PARAMETER_HELP( ppg ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
