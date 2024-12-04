#include <gkg-mri-reconstruction-plugin-functors/BrukerGre2dReconstructor/BrukerGre2dReconstructorCommand.h>
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
// void reorderingSlices()
//

static void reorderingSlices( gkg::Volume< int16_t >& shortVolume )
{

  int32_t sizeX = shortVolume.getSizeX();
  int32_t lineCount = shortVolume.getSizeY();
  int32_t sliceCount = shortVolume.getSizeZ();
  int32_t halfSliceCount = sliceCount / 2;
  int32_t x, line, slice;

  //  slices have to be Z mirrored
  for ( line = 0; line < lineCount; line ++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      for ( slice = 0; slice < halfSliceCount; slice++ )
      {

        std::swap( shortVolume( x, line, slice ),
                   shortVolume( x, line, sliceCount - 1 - slice ) );

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
// class BrukerGre2dReconstructorCommand
//

gkg::BrukerGre2dReconstructorCommand::BrukerGre2dReconstructorCommand(
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
  GKG_CATCH( "gkg::BrukerGre2dReconstructorCommand::"
             "BrukerGre2dReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrukerGre2dReconstructorCommand::BrukerGre2dReconstructorCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
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
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerGre2dReconstructorCommand::"
             "BrukerGre2dReconstructorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


gkg::BrukerGre2dReconstructorCommand::BrukerGre2dReconstructorCommand(
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
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             fileNameOut,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             scale,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerGre2dReconstructorCommand::"
             "BrukerGre2dReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrukerGre2dReconstructorCommand::~BrukerGre2dReconstructorCommand()
{
}


std::string gkg::BrukerGre2dReconstructorCommand::getStaticName()
{

  try
  {

    return "BrukerGre2dReconstructor";

  }
  GKG_CATCH( "std::string gkg::BrukerGre2dReconstructorCommand::getStaticName()" );

}


void gkg::BrukerGre2dReconstructorCommand::parse()
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
                                  "Reconstructor for Bruker 3T 2D GRE"
                                  " 2 echoes phase map pulse sequence",
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
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrukerGre2dReconstructorCommand::parse()" );

}


void gkg::BrukerGre2dReconstructorCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            float scale,
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

    // check that pulse program is one of the good EPI pulse program
    // and managing EPI attributes
    std::string pulseProgramName = "";
    inputVolumeProxy.getHeader().getAttribute( "PULPROG", pulseProgramName );

    double fovX = 0.0, fovY = 0.0, slthick = 0.0;
    if ( pulseProgramName == "fieldmap.ppg" )
    {

      // getting field of view and slice thickness
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "ACQ_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_thick", slthick );

    }
    else
    {

      throw std::runtime_error( "not a Bruker GRE 2D pulse program");

    }


    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    // collecting ramp sampling information if ramp sampling on
    int32_t effectiveSizeX = sizeX;

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
                                                               effectiveSizeX );

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
    int32_t regridingSizeX = ( sizeX <= reconstructionSizeX ? sizeX :
                                                          reconstructionSizeX );
    int32_t xApodizationSize = regridingSizeX;
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
    gkg::Volume< short > shortVolume( reconstructionSizeX,
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


      // mirroring according to Y axis
      if ( verbose )
      {

        std::cout << "Y mirroring" << std::flush;

      }
      mirroring( shortVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 11 ) << std::flush;

      }


      // reordering slices
      if ( verbose )
      {

        std::cout << "Reordering slices" << std::flush;

      }
      reorderingSlices( shortVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 17 ) << std::flush;

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
  GKG_CATCH( "void gkg::BrukerGre2dReconstructorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "float scale, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    BrukerGre2dReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
