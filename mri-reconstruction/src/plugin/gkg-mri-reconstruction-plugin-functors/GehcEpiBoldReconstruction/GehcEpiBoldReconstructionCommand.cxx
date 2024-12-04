#include <gkg-mri-reconstruction-plugin-functors/GehcEpiBoldReconstruction/GehcEpiBoldReconstructionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-mri-reconstruction-core/FullKSpaceReconstructor2d.h>
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
  //    to ky=+kymax/2, and they are stored in the PFile, using that order
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
// void mirroring()
//

static void mirroring( gkg::Volume< int16_t >& shortVolume )
{

  int32_t sizeX = shortVolume.getSizeX();
  int32_t lineCount = shortVolume.getSizeY();
  int32_t sliceCount = shortVolume.getSizeZ();
  int32_t halfLineCount = lineCount / 2;

  int32_t x, line, slice;

  // Y mirroring
  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      for ( line = 0; line < halfLineCount; line ++ )
      {

        std::swap( shortVolume( x, line, slice ),
                   shortVolume( x, lineCount - 1 - line, slice ) );

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
// class GehcEpiBoldReconstructionCommand
//

gkg::GehcEpiBoldReconstructionCommand::GehcEpiBoldReconstructionCommand(
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
  GKG_CATCH( "gkg::GehcEpiBoldReconstructionCommand::GehcEpiBoldReconstructionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcEpiBoldReconstructionCommand::GehcEpiBoldReconstructionCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
                                            bool keepFirstMTF,
                                            int32_t mtfSliceReference,
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
             keepFirstMTF,
             mtfSliceReference,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::GehcEpiBoldReconstructionCommand::"
             "GehcEpiBoldReconstructionCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "bool keepFirstMTF, "
             "int32_t mtfSliceReference, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


gkg::GehcEpiBoldReconstructionCommand::GehcEpiBoldReconstructionCommand(
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
    DECLARE_STRING_PARAMETER( parameters, std::string, phaseCorrectionMethod );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, keepFirstMTF );
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
             keepFirstMTF,
             outputFormat,
             verbose  );

  }
  GKG_CATCH( "gkg::GehcEpiBoldReconstructionCommand::"
             "GehcEpiDwReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcEpiBoldReconstructionCommand::~GehcEpiBoldReconstructionCommand()
{
}


std::string gkg::GehcEpiBoldReconstructionCommand::getStaticName()
{

  try
  {

    return "GehcEpiBoldReconstruction";

  }
  GKG_CATCH( "std::string gkg::GehcEpiBoldReconstructionCommand::getStaticName()" );

}


void gkg::GehcEpiBoldReconstructionCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string xApodizationName = "tukey";
    std::string yApodizationName = "tukey";
    int32_t reconstructionSizeX = -1;
    int32_t reconstructionSizeY = -1;
    float scale = 0.1;
    bool keepFirstMTF = false;
    int32_t mtfSliceReference = -1;
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
                                  "Reconstructor for GEHC Signa 1.5T EPIC"
                                  " GRE Bold Hyperscan"
                                  " EchoPlanar pulse sequence",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input P-data file name",
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
    application.addSingleOption( "-keepFirstMTF",
                                 "Keep MTF result on the first rank for the "
                                 "following rank(s)",
                                 keepFirstMTF,
                                 true );
    application.addSingleOption( "-mtfSliceReference",
                                 "MTF slice reference (default=none)",
                                 mtfSliceReference,
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
             keepFirstMTF,
             mtfSliceReference,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcEpiBoldReconstructionCommand::parse()" );

}


void gkg::GehcEpiBoldReconstructionCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
                                            bool keepFirstMTF,
                                            int32_t mtfSliceReference,
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

    // rounding to greater power of two
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

      std::cout << "reading P-data file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< std::complex< float > > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                       gkg::Volume< std::complex< float > > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "gehc-signa-pfile" )
    {

      throw std::runtime_error( "wrong format, should be \'gehc-signa-pfile\'");

    }

    // check that pulse program is one of the good EPIC EPI pulse sequence
    std::string psdName = "";
    inputVolumeProxy.getHeader().getAttribute(
                      "pool_header.rdb_hdr_image.psd_iname", psdName );
    if ( ( psdName != "EPI2" ) && ( psdName != "EPI" ) )
    {

      throw std::runtime_error( "not a EPIC EPI pulse sequence");

    }

    // getting field of view and slice thickness
    double fovX = 0.0, fovY = 0.0, sliceThickness = 0.0;
    float fov, slthick, scanspacing;
    inputVolumeProxy.getHeader().getAttribute( "pool_header.rdb_hdr_image.dfov",
                                               fov );
    inputVolumeProxy.getHeader().getAttribute(
                                            "pool_header.rdb_hdr_image.slthick",
                                            slthick );
    inputVolumeProxy.getHeader().getAttribute(
                                        "pool_header.rdb_hdr_image.scanspacing",
                                        scanspacing );
    fovX = fovY = ( double )fov;
    sliceThickness = ( double )( slthick + scanspacing );

    // collecting size(s)
    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    if ( verbose )
    {

      std::cout << "raw data geometry: "
                << sizeX << " x "
                << lineCount << " x "
                << sliceCount << " x "
                << timeCount << std::endl;
      std::cout << "field of view: "
                << fovX << " mm x "
                << fovY << " mm"
                << std::endl;
      std::cout << "slice thickness: "
                << slthick << " mm"
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
    if ( reconstructionSizeY == -1 )
    {

      reconstructionSizeY = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                                                    lineCount );

    }
    reconstructionSizeY = std::max( reconstructionSizeY, lineCount );
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
    int32_t xApodizationSize = sizeX;
    int32_t yApodizationSize = lineCount;
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
                                                sliceThickness );
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
                                                         "gehc-signa-pfile" );
      if ( format != "gehc-signa-pfile" )
      {

        throw std::runtime_error(
                                "wrong format, should be \'gehc-signa-pfile\'");

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

      // correction for ramp sampling if ramp sampling on
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

      // estimating modulation transfer function(s)
      if ( !keepFirstMTF || ( keepFirstMTF && ( time == 0 ) ) )
      {

        if ( verbose )
        {

          std::cout << "MTF estimation->" << std::flush;

        }
        gkg::ModulationTransferFunction< float >::getInstance().estimate(
                                                                   recVolume,
                                                                   modulationTF,
                                                                   0.0,
                                                                   verbose );
        if ( mtfSliceReference != -1 )
        {

          if ( ( mtfSliceReference >= sliceCount ) ||
               ( mtfSliceReference < 0 ) )
          {

            throw std::runtime_error( "bad MTF slice reference" );

          }
          int32_t slice = 0;
          for ( slice = 0; slice < sliceCount; slice++ )
          {

            if ( slice != mtfSliceReference )
            {

              modulationTF[ slice ] = modulationTF[ mtfSliceReference ];

            }

          }


        }
        if ( verbose )
        {

          std::cout << gkg::Eraser( 16 ) << std::flush;

        }

      }

      // reconstructing from Full 2D K-space
      if ( verbose )
      {

        std::cout << "Fourier->" << std::flush;

      }
      gkg::FullKSpaceReconstructor2d< float >::getInstance().setRawLineCount(
                                                                    lineCount );
      gkg::FullKSpaceReconstructor2d< float >::getInstance().
                                               setModulationTransferFunction(
                                                                modulationTF );
      gkg::FullKSpaceReconstructor2d< float >::getInstance().setXApodization(
                                                                 xApodization );
      gkg::FullKSpaceReconstructor2d< float >::getInstance().setYApodization(
                                                                 yApodization );
      gkg::FullKSpaceReconstructor2d< float >::getInstance().reconstruct(
                                                                 recVolume,
                                                                 recVolume,
                                                                 verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 9 ) << std::flush;

      }

      // taking magnitude of real or complex part
      if ( verbose )
      {

        std::cout << "Magnitude cplx part" << std::flush;

      }
      magnitudeCplxConverter.convert( recVolume, magnitudeVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 19 ) << std::flush;

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

      // mirroring according to X and Y axis
      if ( verbose )
      {

        std::cout << "Y mirroring" << std::flush;

      }
      mirroring( shortVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 11 ) << std::flush;

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

  }
  GKG_CATCH( "void gkg::GehcEpiBoldReconstructionCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "bool keepFirstMTF, "
             "int32_t mtfSliceReference, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


RegisterCommandCreator(
    GehcEpiBoldReconstructionCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( noTrajectoryCorrection ) +
    DECLARE_STRING_PARAMETER_HELP( phaseCorrectionMethod ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( keepFirstMTF ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
