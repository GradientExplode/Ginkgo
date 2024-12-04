#include <gkg-mri-reconstruction-plugin-functors/GehcGre2dReconstructor/GehcGre2dReconstructorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-mri-reconstruction-core/FullKSpaceReconstructor2d.h>
#include <gkg-mri-reconstruction-correction/DcOffsetCorrection.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
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
// void mirroring()
//

static void mirroring( gkg::Volume< std::complex< float > >& recVolume )
{

  int32_t sizeX = recVolume.getSizeX();
  int32_t lineCount = recVolume.getSizeY();
  int32_t sliceCount = recVolume.getSizeZ();
  int32_t halfLineCount = lineCount / 2;

  int32_t x, line, slice;

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
// void addContribution()
//

static void addContribution( const gkg::Volume< float >& magnitudeVolume,
                             gkg::Volume< float >& meanSquareMagnitudeVolume,
                             int32_t coil, bool verbose )
{

  try
  {

    int32_t sizeX = meanSquareMagnitudeVolume.getSizeX();
    int32_t lineCount = meanSquareMagnitudeVolume.getSizeY();
    int32_t sliceCount = meanSquareMagnitudeVolume.getSizeZ();

    int32_t x, line, slice;

    if ( coil == 0 )
    {

      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            meanSquareMagnitudeVolume( x, line, slice ) =
              magnitudeVolume( x, line, slice ) *
              magnitudeVolume( x, line, slice );

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
    else
    {

      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            meanSquareMagnitudeVolume( x, line, slice ) +=
              magnitudeVolume( x, line, slice ) *
              magnitudeVolume( x, line, slice );

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

  }
  catch ( std::exception& e )
  {

    throw std::runtime_error( std::string( "addContribution :" ) + e.what() );

  }

}


//
// void takeRootOf()
//

static void takeRootOf( gkg::Volume< float >& meanSquareMagnitudeVolume,
                        bool verbose )
{

  try
  {

    int32_t sizeX = meanSquareMagnitudeVolume.getSizeX();
    int32_t lineCount = meanSquareMagnitudeVolume.getSizeY();
    int32_t sliceCount = meanSquareMagnitudeVolume.getSizeZ();

    int32_t x, line, slice;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          meanSquareMagnitudeVolume( x, line, slice ) = std::sqrt( 
                                  meanSquareMagnitudeVolume( x, line, slice ) );

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

    throw std::runtime_error( std::string( "takeRootOf :" ) + e.what() );

  }

}


//
// class GehcGre2dReconstructorCommand
//

gkg::GehcGre2dReconstructorCommand::GehcGre2dReconstructorCommand(
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
  GKG_CATCH( "gkg::GehcGre2dReconstructorCommand::"
             "GehcGre2dReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcGre2dReconstructorCommand::GehcGre2dReconstructorCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            bool saveComplex,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
                                            const std::string& outputFormat,
                                            bool multiChannel,
                                            bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             saveComplex,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             outputFormat,
             multiChannel,
             verbose );

  }
  GKG_CATCH( "gkg::GehcGre2dReconstructorCommand::"
             "GehcGre2dReconstructorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool saveComplex, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "const std::string& outputFormat, "
             "bool multiChannel, "
             "bool verbose  )" );

}


gkg::GehcGre2dReconstructorCommand::GehcGre2dReconstructorCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, saveComplex );
    DECLARE_STRING_PARAMETER( parameters, std::string, xApodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, yApodizationName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeY );
    DECLARE_FLOATING_PARAMETER( parameters, float, scale );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, multiChannel );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             saveComplex,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             outputFormat,
             multiChannel,
             verbose );

  }
  GKG_CATCH( "gkg::GehcGre2dReconstructorCommand::"
             "GehcGre2dReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcGre2dReconstructorCommand::~GehcGre2dReconstructorCommand()
{
}


std::string gkg::GehcGre2dReconstructorCommand::getStaticName()
{

  try
  {

    return "GehcGre2dReconstructor";

  }
  GKG_CATCH( "std::string "
             "gkg::GehcGre2dReconstructorCommand::getStaticName()" );

}


void gkg::GehcGre2dReconstructorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool saveComplex = false;
    std::string xApodizationName = "tukey";
    std::string yApodizationName = "tukey";
    int32_t reconstructionSizeX = -1;
    int32_t reconstructionSizeY = -1;
    float scale = 0.1;
    std::string outputFormat = "gis";
    bool multiChannel = false;
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
                                  "Reconstructor for GEHC Signa 1.5T 2D GRE"
                                  " pulse sequence",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input P-data file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Reconstructed GIS image file name",
                                 fileNameOut );
    application.addSingleOption( "-saveComplex",
                                 "Complex value of reconstructed data is"
                                 " saved instead of magnitude",
                                 saveComplex,
                                 true );
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
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-multiChannel",
                                 "Reconstruct all channels (default=false)",
                                 multiChannel,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             saveComplex,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             outputFormat,
             multiChannel,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcGre2dReconstructorCommand::parse()" );

}


void gkg::GehcGre2dReconstructorCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            bool saveComplex,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
                                            const std::string& outputFormat,
                                            bool multiChannel,
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

    // forcing some states
    if ( saveComplex )
    {

      multiChannel = true;

    }

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

      int powerOfTwo = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
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

      int powerOfTwo = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
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

      std::cout << "reading GEHC Signa P-file header: "
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

    // check that pulse program is one of the good McKinnon EPI pulse sequence
    std::string psdName = "";
    inputVolumeProxy.getHeader().getAttribute(
                      "pool_header.rdb_hdr_image.psd_iname", psdName );
    if ( ( psdName != "GkgGe" ) &&
         ( psdName != "NmrGe" ) )
    {

      throw std::runtime_error( "not a McKinnon Gradient Echo pulse sequence");

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

    // collecting coil information
    int16_t start_rcv, stop_rcv;
    inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
    inputVolumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
    int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );

    // collecting size(s)
    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT() /
                    ( multiChannel ? 1 :coilCount );

    // collecting parallel reduction factor
    float parallelReductionFactor = 1.0;
    inputVolumeProxy.getHeader().getAttribute(
                   "pool_header.rdb_hdr_image.user0", parallelReductionFactor );
    fovY /= parallelReductionFactor;

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
                << sliceThickness << " mm"
                << std::endl;
      std::cout << "coil channel count: "
                << coilCount
                << std::endl;
      std::cout << "parallel reduction factor: "
                << parallelReductionFactor
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
    gkg::Volume< float > realPartVolume;
    gkg::Volume< float > magnitudeVolume;
    gkg::Volume< float > meanSquareMagnitudeVolume;
    gkg::Volume< int16_t > shortVolume;

    recVolume.getHeader().addAttribute( "resolutionX",
                                        fovX / reconstructionSizeX );
    recVolume.getHeader().addAttribute( "resolutionY",
                                        fovY / reconstructionSizeY );
    recVolume.getHeader().addAttribute( "resolutionZ",
                                        sliceThickness );
    recVolume.getHeader().addAttribute( "resolutionT",
                                        ( double )1.0 );

    if ( !saveComplex )
    {

      realPartVolume = gkg::Volume< float >( reconstructionSizeX,
                                             reconstructionSizeY,
                                             sliceCount );
      magnitudeVolume = gkg::Volume< float >( reconstructionSizeX,
                                              reconstructionSizeY,
                                              sliceCount );
      meanSquareMagnitudeVolume = gkg::Volume< float >( reconstructionSizeX,
                                                        reconstructionSizeY,
                                                        sliceCount );
      shortVolume = gkg::Volume< int16_t >( reconstructionSizeX,
                                            reconstructionSizeY,
                                            sliceCount );

    }

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

    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }
    gkg::BoundingBox< int32_t > outputBoundingBox( 0, reconstructionSizeX - 1,
                                                   0, reconstructionSizeY - 1,
                                                   0, sliceCount - 1,
                                                   0, 0 );
    gkg::VolumeProxy< int16_t > outputVolumeProxy;
    gkg::VolumeProxy< std::complex< float > > outputCplxVolumeProxy;


    if ( saveComplex )
    {

      outputCplxVolumeProxy = gkg::VolumeProxy< std::complex< float > > (
                                                          reconstructionSizeX,
                                                          reconstructionSizeY,
                                                          sliceCount,
                                                          timeCount );
      outputCplxVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
      outputCplxVolumeProxy.getHeader()[ "item_type" ] =
        gkg::TypeOf< std::complex< float > >::getName();
      outputCplxVolumeProxy.getHeader()[ "sizeX" ] = reconstructionSizeX;
      outputCplxVolumeProxy.getHeader()[ "sizeY" ] = reconstructionSizeY;
      outputCplxVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
      outputCplxVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionX",
                                                   fovX / reconstructionSizeX );
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionY",
                                                   fovY / reconstructionSizeY );
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                                      sliceThickness );
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionT",
                                                      ( double )1.0 );
      if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                  gkg::Volume< std::complex< float > > >( fileNameOut,
                                                          outputCplxVolumeProxy,
                                                          outputFormat ) !=
           outputFormat )
      {

        throw std::runtime_error(
              std::string( "failed to prepare partial write with format \' " ) +
              outputFormat + "\'" );

      }

    }
    else
    {

      outputVolumeProxy = gkg::VolumeProxy< int16_t > ( reconstructionSizeX,
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

    int32_t time, coil;
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

      for ( coil = 0; coil < ( multiChannel ? 1 : coilCount ); coil++ )
      {

        // reading raw data from P-file
        inputBoundingBox.setLowerT( time * ( multiChannel ? 1 : coilCount ) +
                                    coil );
        inputBoundingBox.setUpperT( time * ( multiChannel ? 1 : coilCount ) +
                                    coil );
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

        // copying raw to rec volume
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

        // mirroring according to Y axis
        if ( verbose )
        {

          std::cout << "Y mirroring" << std::flush;

        }
        mirroring( recVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 11 ) << std::flush;

        }

        if ( !saveComplex )
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
          addContribution( magnitudeVolume, meanSquareMagnitudeVolume, coil,
                           verbose );

        }

      }

      // further things to do if not complex 
      if ( !saveComplex )
      {

        // taking mean square root
        takeRootOf( meanSquareMagnitudeVolume, verbose );

        // rescaling and converting to short
        if ( verbose )
        {

          std::cout << "Rescaling+short convertion" << std::flush;

        }
        scaler.scale( meanSquareMagnitudeVolume );
        lowerThresholder.threshold( meanSquareMagnitudeVolume,
                                    meanSquareMagnitudeVolume );
        greaterThresholder.threshold( meanSquareMagnitudeVolume,
                                      meanSquareMagnitudeVolume );
        converter.convert( meanSquareMagnitudeVolume, shortVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 26 ) << std::flush;

        }

      }

      // saving reconstructed slice to disk
      if ( verbose )
      {

        std::cout << "Saving to disk" << std::flush;

      }
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );

      if ( saveComplex )
      {

        gkg::Writer::getInstance().partialBinaryWrite<
                  gkg::Volume< std::complex< float > > >( outputCplxVolumeProxy,
                                                          recVolume,
                                                          outputBoundingBox,
                                                          outputFormat );

      }
      else
      {

        gkg::Writer::getInstance().partialBinaryWrite< 
                                                      gkg::Volume< int16_t > >(
                                                            outputVolumeProxy,
                                                            shortVolume,
                                                            outputBoundingBox,
                                                            outputFormat );

      }
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
  GKG_CATCH( "void gkg::GehcGre2dReconstructorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool saveComplex, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "const std::string& outputFormat, "
             "bool multiChannel, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    GehcGre2dReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( saveComplex ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( multiChannel ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
