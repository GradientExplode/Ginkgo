#include <gkg-mri-reconstruction-plugin-functors/GehcB0MapReconstructor/GehcB0MapReconstructorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-mri-reconstruction-core/FullKSpaceReconstructor2d.h>
#include <gkg-mri-reconstruction-correction/DcOffsetCorrection.h>
#include <gkg-mri-reconstruction-correction/GehcSignaGradientCoil.h>
#include <gkg-mri-reconstruction-correction/GradientWarping2dCorrection.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
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
// class GehcB0MapReconstructorCommand
//

gkg::GehcB0MapReconstructorCommand::GehcB0MapReconstructorCommand(
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
  GKG_CATCH( "gkg::GehcB0MapReconstructorCommand::"
             "GehcB0MapReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcB0MapReconstructorCommand::GehcB0MapReconstructorCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            const std::string& outputFormat,
                                            bool gradwarp,
                                            const std::string& gradientCoilType,
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
             outputFormat,
             gradwarp,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH( "gkg::GehcB0MapReconstructorCommand::"
             "GehcB0MapReconstructorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "const std::string& outputFormat, "
             "bool gradwarp, "
             "const std::string& gradientCoilType, "
             "bool verbose  )" );

}


gkg::GehcB0MapReconstructorCommand::GehcB0MapReconstructorCommand(
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
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, gradwarp );
    DECLARE_STRING_PARAMETER( parameters, std::string, gradientCoilType );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             fileNameOut,
             xApodizationName,
             yApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             outputFormat,
             gradwarp,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH( "gkg::GehcB0MapReconstructorCommand::"
             "GehcB0MapReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcB0MapReconstructorCommand::~GehcB0MapReconstructorCommand()
{
}


std::string gkg::GehcB0MapReconstructorCommand::getStaticName()
{

  try
  {

    return "GehcB0MapReconstructor";

  }
  GKG_CATCH( "std::string gkg::GehcB0MapReconstructorCommand::getStaticName()" );

}


void gkg::GehcB0MapReconstructorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string xApodizationName = "tukey";
    std::string yApodizationName = "tukey";
    int32_t reconstructionSizeX = -1;
    int32_t reconstructionSizeY = -1;
    std::string outputFormat = "gis";
    bool gradwarp = false;
    std::string gradientCoilType = "CRM";
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
                                  " double-echo B0 map pulse sequence",
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
    application.addSingleOption( "-gw",
                                 "Apply gradient warping correction"
                                 " (default=false)",
                                 gradwarp, 
                                 true );
    application.addSingleOption( "-gradientCoilType",
                                 "GEHC Gradient coil type (default=CRM)\n"
                                 " - BRM\n"
                                 " - CRM\n"
                                 " - TRM.WHOLE\n"
                                 " - TRM.ZOOM\n",
                                 gradientCoilType, 
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
             outputFormat,
             gradwarp,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcB0MapReconstructorCommand::parse()" );

}


void gkg::GehcB0MapReconstructorCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& xApodizationName,
                                            const std::string& yApodizationName,
                                            int32_t reconstructionSizeX,
                                            int32_t reconstructionSizeY,
                                            const std::string& outputFormat,
                                            bool gradwarp,
                                            const std::string& gradientCoilType,
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
    if ( ( gradientCoilType != "BRM" ) &&
         ( gradientCoilType != "CRM" ) &&
         ( gradientCoilType != "TRM.WHOLE" ) &&
         ( gradientCoilType != "TRM.ZOOM" ) )
    {

      throw std::runtime_error( "not a valid gradient coil type" );

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
    if ( ( psdName != "mcdgeb0map" ) &&
         ( psdName != "GkgB0Map" ) &&
         ( psdName != "NmrB0Map" ) )
    {

      throw std::runtime_error( "not a McKinnon Double Gradient Echo "
                                "B0 map pulse sequence");

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
                << sliceThickness << " mm"
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
                                                            sliceCount,
                                                            timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "item_type" ] =
      gkg::TypeOf< std::complex< float > >::getName();
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

    // estimating gradient warping correction 
    std::vector< gkg::GradientWarping2dCorrection< float >::Wedge2d >
        gradientWarpingWedge2ds;
    bool swapXY = false;
    if ( gradwarp )
    {

      if ( verbose )
      {

        std::cout << "preparing gradient warping correction : " << std::flush;

      }

      std::string gradientCoilName =
        gkg::ConfigurationInfo::getInstance().getGkgPath() +
        gkg::getDirectorySeparator() + "gehc" +
        gkg::getDirectorySeparator() + "gradwarp" +
        gkg::getDirectorySeparator() + "gw_coils.dat." + gradientCoilType;
      gkg::GehcSignaGradientCoil< float > gradientCoil( gradientCoilName );

      float start_loc, end_loc;
      inputVolumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      inputVolumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );

      std::vector< gkg::GradientWarping2dCorrection< float >::Plane >
        planes( sliceCount );
      int32_t s, trueSlice;
      for ( s = 0; s < sliceCount; s++ )
      {

        trueSlice = ( reverseZ ? sliceCount - 1 - s : s );

        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point1[0]",
            planes[ trueSlice ].point1.x );
        planes[ trueSlice ].point1.x /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point1[1]",
            planes[ trueSlice ].point1.y );
        planes[ trueSlice ].point1.y /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point1[2]",
            planes[ trueSlice ].point1.z );
        planes[ trueSlice ].point1.z /= 10.0;

        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point2[0]",
            planes[ trueSlice ].point2.x );
        planes[ trueSlice ].point2.x /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point2[1]",
            planes[ trueSlice ].point2.y );
        planes[ trueSlice ].point2.y /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point2[2]",
            planes[ trueSlice ].point2.z );
        planes[ trueSlice ].point2.z /= 10.0;

        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point3[0]",
            planes[ trueSlice ].point3.x );
        planes[ trueSlice ].point3.x /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point3[1]",
            planes[ trueSlice ].point3.y );
        planes[ trueSlice ].point3.y /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point3[2]",
            planes[ trueSlice ].point3.z );
        planes[ trueSlice ].point3.z /= 10.0;

      }

      // detecting swap between X and Y
      if ( inputVolumeProxy.getHeader().hasAttribute(
             "pool_header.rdb_hdr_image.freq_dir" ) )
      {

        int16_t freq_dir;
        inputVolumeProxy.getHeader().getAttribute(
                                           "pool_header.rdb_hdr_image.freq_dir",
                                           freq_dir );
        swapXY = ( freq_dir == 1 ? true : false );

      }

      gkg::GradientWarping2dCorrection< float >::getInstance().estimate(
                                                        gradientCoil,
                                                        planes,
                                                        gradientWarpingWedge2ds,
                                                        reconstructionSizeX,
                                                        reconstructionSizeY,
                                                        verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

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

      // applying 2d gradient warping
      if ( gradwarp )
      {

        if ( verbose )
        {

          std::cout << "Gradient warping correction" << std::flush;

        }
        gkg::GradientWarping2dCorrection< float >::getInstance().apply(
                                                        recVolume,
                                                        gradientWarpingWedge2ds,
                                                        swapXY,
                                                        verbose );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 27 ) << std::flush;

        }

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

  }
  GKG_CATCH( "void gkg::GehcB0MapReconstructorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "const std::string& outputFormat, "
             "bool gradwarp, "
             "const std::string& gradientCoilType, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    GehcB0MapReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( gradwarp ) +
    DECLARE_STRING_PARAMETER_HELP( gradientCoilType ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
