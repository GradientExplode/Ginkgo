#include <gkg-mri-reconstruction-plugin-functors/BrukerPhaseMap3dReconstructor/BrukerPhaseMap3dReconstructorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-processing-signal/Scrolling3d.h>
#include <gkg-mri-reconstruction-core/FullKSpaceReconstructor3d.h>
#include <gkg-mri-reconstruction-correction/DcOffsetCorrection.h>
#include <gkg-mri-reconstruction-correction/TrapezoidROGradient.h>
#include <gkg-mri-reconstruction-correction/RegridingCorrection.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iomanip>


//
// void getScrollingRatios()
//

static void getScrollingRatios(
              const gkg::VolumeProxy< std::complex< float > >& inputVolumeProxy,
              float& yScrollingRatio,
              float& zScrollingRatio )
{

  try
  {

    if ( inputVolumeProxy.getHeader().hasAttribute( 
                                                  "PVM_SPackArrPhase1Offset" ) )
    {

      std::vector< double > PVM_SPackArrPhase1Offset;
      inputVolumeProxy.getHeader().getAttribute( "PVM_SPackArrPhase1Offset",
                                                 PVM_SPackArrPhase1Offset );
      yScrollingRatio =  ( float )PVM_SPackArrPhase1Offset[ 0 ];

    }
    if ( inputVolumeProxy.getHeader().hasAttribute( 
                                                  "PVM_SPackArrSliceOffset" ) )
    {

      std::vector< double > PVM_SPackArrSliceOffset;
      inputVolumeProxy.getHeader().getAttribute( "PVM_SPackArrSliceOffset",
                                                 PVM_SPackArrSliceOffset );
      zScrollingRatio =  ( float )PVM_SPackArrSliceOffset[ 0 ];

    }

  }
  GKG_CATCH( "void getScrollingRatios( float& yScrollingRatio, "
             "float& zScrollingRatio )" );

}


//
// void mirroring()
//

static void mirroring( gkg::Volume< std::complex< float > >& recVolume )
{

  int32_t sizeX = recVolume.getSizeX();
  int32_t lineCount = recVolume.getSizeY();
  int32_t sliceCount = recVolume.getSizeZ();
  int32_t halfSizeX = sizeX / 2;
  int32_t halfLineCount = lineCount / 2;

  int32_t x, line, slice;

  // X mirroring
  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( line = 0; line < lineCount; line ++ )
    {

      for ( x = 0; x < halfSizeX; x++ )
      {

        std::swap( recVolume( x, line, slice ),
                   recVolume( sizeX - 1 - x, line, slice ) );

      }

    }

  }

  // Y mirroring
  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      for ( line = 0; line < halfLineCount; line ++ )
      {

        std::swap( recVolume( x, line, slice ),
                   recVolume( x, lineCount - 1 - line, slice ) );

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
// class BrukerPhaseMap3dReconstructorCommand
//

gkg::BrukerPhaseMap3dReconstructorCommand::BrukerPhaseMap3dReconstructorCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                          : gkg::Command( argc, argv, 
                                                          loadPlugin,
                                                          removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::BrukerPhaseMap3dReconstructorCommand::"
             "BrukerPhaseMap3dReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrukerPhaseMap3dReconstructorCommand::BrukerPhaseMap3dReconstructorCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            const std::string& zApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            int32_t reconstructionSizeZ,
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
             zApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             reconstructionSizeZ,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerPhaseMap3dReconstructorCommand::"
             "BrukerPhaseMap3dReconstructorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "const std::string& zApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "int32_t reconstructionSizeZ, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


gkg::BrukerPhaseMap3dReconstructorCommand::BrukerPhaseMap3dReconstructorCommand(
                                             const gkg::Dictionary& parameters )
                                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, xApodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, yApodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, zApodizationName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeY );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeZ );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             fileNameOut,
             xApodizationName,
             yApodizationName,
             zApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             reconstructionSizeZ,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerPhaseMap3dReconstructorCommand::"
             "BrukerPhaseMap3dReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrukerPhaseMap3dReconstructorCommand::
                                         ~BrukerPhaseMap3dReconstructorCommand()
{
}


std::string gkg::BrukerPhaseMap3dReconstructorCommand::getStaticName()
{

  try
  {

    return "BrukerPhaseMap3dReconstructor";

  }
  GKG_CATCH( "std::string gkg::BrukerPhaseMap3dReconstructorCommand::getStaticName()" );

}


void gkg::BrukerPhaseMap3dReconstructorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string xApodizationName = "tukey";
    std::string yApodizationName = "tukey";
    std::string zApodizationName = "tukey";
    int32_t reconstructionSizeX = -1;
    int32_t reconstructionSizeY = -1;
    int32_t reconstructionSizeZ = -1;
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
                                  "Reconstructor for Bruker 3T 3D GRE"
                                  " double echoes phase map pulse sequence",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input Bruker raw-data directory name",
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
    application.addSingleOption( "-az",
                                 std::string( "Z apodization (" ) +
                                 apodizationNames + ") " +
                                 "(default=tukey)",
                                 zApodizationName,
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
    application.addSingleOption( "-sz",
                                 "Reconstruction Z size "
                                 "(default=kspace Z size)",
                                 reconstructionSizeZ,
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
             zApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             reconstructionSizeZ,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrukerPhaseMap3dReconstructorCommand::parse()" );

}


void gkg::BrukerPhaseMap3dReconstructorCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            const std::string& zApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            int32_t reconstructionSizeZ,
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
    if ( !gkg::ApodizationFactory< float >::getInstance().hasApodization(
                                                         zApodizationName ) )
    {

      throw std::runtime_error( 
              std::string( "Z apodization should be one of " ) +
              apodizationNames );

    }

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

    // check that pulse program is one of the good field map pulse program
    // and managing field map attributes
    std::string pulseProgramName = "";
    inputVolumeProxy.getHeader().getAttribute( "PULPROG", pulseProgramName );

    double fovX = 0.0, fovY = 0.0, fovZ = 0.0;
    if ( ( pulseProgramName == "fieldmap.ppg" ) ||
         ( pulseProgramName == "fh_ge3d.ppg" ) )
    {

      // getting field of view and slice thickness
      std::vector< double > fov( 3 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      fovZ = fov[ 2 ] * 10.0;

    }
    else
    {

      throw std::runtime_error( "not a Bruker GRE 3D phase map pulse program");

    }


    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    float yScrollingRatio = 0.0;
    float zScrollingRatio = 0.0;

    getScrollingRatios( inputVolumeProxy, yScrollingRatio, zScrollingRatio );
    yScrollingRatio /= -fovY;
    zScrollingRatio /= fovZ;

    if ( verbose )
    {

      std::cout << "raw data geometry: "
                << sizeX << " x "
                << lineCount << " x "
                << sliceCount << " x "
                << timeCount << std::endl;
      std::cout << "field of view: "
                << fovX << " mm x "
                << fovY << " mm x "
                << fovZ << " mm"
                << std::endl;

    }

    // calculating reconstructed image size(s)
    if ( verbose )
    {

      std::cout << "evaluating reconstructed image size(s): " << std::flush;

    }
    if ( reconstructionSizeX == -1 )
    {

      reconstructionSizeX = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                                                        sizeX );

    }
    reconstructionSizeX = std::max( reconstructionSizeX, sizeX );
    if ( reconstructionSizeY == -1 )
    {

      reconstructionSizeY = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                                                    lineCount );

    }
    reconstructionSizeY = std::max( reconstructionSizeY, lineCount );
    if ( reconstructionSizeZ == -1 )
    {

      reconstructionSizeZ = sliceCount;

    }
    reconstructionSizeZ = std::max( reconstructionSizeZ, sliceCount );
    if ( verbose )
    {

      std::cout << reconstructionSizeX << " x "
                << reconstructionSizeY << " x "
                << reconstructionSizeZ << " x "
                << timeCount << std::endl;

    }

    // creating apodization
    if ( verbose )
    {

      std::cout << "creating apodization(s): " << std::flush;

    }
    int32_t xApodizationSize = sizeX;
    int32_t yApodizationSize = lineCount;
    int32_t zApodizationSize = sliceCount;
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
    gkg::Apodization< float >* zApodization =
        gkg::ApodizationFactory< float >::getInstance().createApodization(
                                          zApodizationName,
                                          zApodizationSize,
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
                                                    reconstructionSizeZ );


    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< std::complex< float > >
                                         outputVolumeProxy( reconstructionSizeX,
                                                            reconstructionSizeY,
                                                            reconstructionSizeZ,
                                                            timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "item_type" ] =
      gkg::TypeOf< std::complex< float > >::getName();
    outputVolumeProxy.getHeader()[ "sizeX" ] = reconstructionSizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = reconstructionSizeY;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = reconstructionSizeZ;
    outputVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
    outputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                                fovX / reconstructionSizeX );
    outputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                                fovY / reconstructionSizeY );
    outputVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                                fovZ / reconstructionSizeZ );
    outputVolumeProxy.getHeader().addAttribute( "resolutionT",
                                                ( double )1.0 );

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, reconstructionSizeX - 1,
                                                   0, reconstructionSizeY - 1,
                                                   0, reconstructionSizeZ - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                     gkg::Volume< std::complex< float > > >( fileNameOut,
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

      // removing constant part of complex signal (DC offset correction)

      if ( verbose )
      {

        std::cout << "DcOffset->" << std::flush;

      }
      gkg::DcOffsetCorrection< float >::getInstance().apply( rawVolume,
                                                             sizeX,
                                                             lineCount,
                                                             sliceCount,
                                                             &dcOffset );
      if ( verbose )
      {

        std::cout << std:: setw( 30 ) << dcOffset
                  << gkg::Eraser( 40 ) << std::flush;

      }


      // scrolling according to Y offset
      if ( yScrollingRatio != 0.0 )
      {

        if ( verbose )
        {

          std::cout << "Y scrolling->" << std::flush;

        }
        gkg::Scrolling3d< float >::getInstance().apply(
                                              rawVolume,
                                              yScrollingRatio,
                                              gkg::Scrolling3d< float >::Y_AXIS,
                                              verbose );      
        if ( verbose )
        {

          std::cout << gkg::Eraser( 13 ) << std::flush;

        }

      }

      // scrolling according to Z offset
      if ( zScrollingRatio != 0.0 )
      {

        if ( verbose )
        {

          std::cout << "Z scrolling->" << std::flush;

        }
        gkg::Scrolling3d< float >::getInstance().apply(
                                              rawVolume,
                                              zScrollingRatio,
                                              gkg::Scrolling3d< float >::Z_AXIS,
                                              verbose );      
        if ( verbose )
        {

          std::cout << gkg::Eraser( 13 ) << std::flush;

        }

      }

      // copying raw to rec data
      recVolume.fill( std::complex< float >( 0.0, 0.0 ) );
      if ( verbose )
      {

        std::cout << "Copying->" << std::flush;

      }
      copyRaw2Rec( rawVolume, recVolume, verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 9 ) << std::flush;

      }

      // reconstructing from K-space
      if ( verbose )
      {

        std::cout << "Fourier->" << std::flush;

      }
      gkg::FullKSpaceReconstructor3d< float >::getInstance().setXApodization(
                                                                 xApodization );
      gkg::FullKSpaceReconstructor3d< float >::getInstance().setYApodization(
                                                                 yApodization );
      gkg::FullKSpaceReconstructor3d< float >::getInstance().setZApodization(
                                                                 zApodization );
      gkg::FullKSpaceReconstructor3d< float >::getInstance().reconstruct(
                                                                   recVolume,
                                                                   recVolume,
                                                                   verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 9 ) << std::flush;

      }

      // mirroring according to Y axis
      if ( verbose )
      {

        std::cout << "X+Y mirroring" << std::flush;

      }
      mirroring( recVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 13 ) << std::flush;

      }

      // saving reconstructed slice to disk
      if ( verbose )
      {

        std::cout << "Saving to disk" << std::flush;

      }
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );

      gkg::Writer::getInstance().partialBinaryWrite<
                      gkg::Volume< std::complex< float > > >( outputVolumeProxy,
                                                              recVolume,
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
    delete zApodization;

  }
  GKG_CATCH( "void gkg::BrukerPhaseMap3dReconstructorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "const std::string& zApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "int32_t reconstructionSizeZ, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


RegisterCommandCreator(
    BrukerPhaseMap3dReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( zApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeZ ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );