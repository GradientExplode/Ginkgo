#include <gkg-mri-reconstruction-plugin-functors/GehcGradientWarpingCorrection/GehcGradientWarpingCorrectionCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-mri-reconstruction-correction/GehcSignaGradientCoil.h>
#include <gkg-mri-reconstruction-correction/GradientWarping2dCorrection.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-core-exception/Exception.h>

#include <algorithm>
#include <iomanip>
#include <fstream>


gkg::GehcGradientWarpingCorrectionCommand::GehcGradientWarpingCorrectionCommand(
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
  GKG_CATCH( "gkg::GehcGradientWarpingCorrectionCommand::"
             "GehcGradientWarpingCorrectionCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcGradientWarpingCorrectionCommand::GehcGradientWarpingCorrectionCommand(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& outputFormat,
                                            bool keepNegativeValues,
                                            bool displayPlanes,
                                            bool displayCoilInfo,
                                            const std::string& gradientCoilType,
                                            bool verbose )
                                          : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             keepNegativeValues,
             displayPlanes,
             displayCoilInfo,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH( "gkg::GehcGradientWarpingCorrectionCommand::"
             "GehcGradientWarpingCorrectionCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool keepNegativeValues, "
             "bool displayPlanes, "
             "bool displayCoilInfo, "
             "const std::string& gradientCoilType, "
             "bool verbose  )" );

}


gkg::GehcGradientWarpingCorrectionCommand::GehcGradientWarpingCorrectionCommand(
                                             const gkg::Dictionary& parameters )
                                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, keepNegativeValues );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, displayPlanes );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, displayCoilInfo );
    DECLARE_STRING_PARAMETER( parameters, std::string, gradientCoilType );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             keepNegativeValues,
             displayPlanes,
             displayCoilInfo,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH( "gkg::GehcGradientWarpingCorrectionCommand::"
             "GehcGradientWarpingCorrectionCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcGradientWarpingCorrectionCommand::
                                         ~GehcGradientWarpingCorrectionCommand()
{
}


std::string gkg::GehcGradientWarpingCorrectionCommand::getStaticName()
{

  try
  {

    return "GehcGradientWarpingCorrection";

  }
  GKG_CATCH( "std::string gkg::GehcGradientWarpingCorrectionCommand::getStaticName()" );

}


void gkg::GehcGradientWarpingCorrectionCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool keepNegativeValues = false;
    bool displayPlanes = false;
    bool displayCoilInfo = false;
    std::string gradientCoilType = "CRM";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Gradient warping 2d correction for "
                                  "GEHC Signa 1.5T data",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input short GEHC reconstructed file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Corrected GIS image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-keepNegativeValues",
                                 "Do not remove negative values due "
                                 "to resampling",
                                 keepNegativeValues, 
                                 true );
    application.addSingleOption( "-displayCoilInfo",
                                 "Display coil information(s) when verbose",
                                 displayCoilInfo, 
                                 true );
    application.addSingleOption( "-displayPlanes",
                                 "Display plane(s) when verbose",
                                 displayPlanes, 
                                 true );
    application.addSingleOption( "-gradientCoilType",
                                 "GEHC Gradient coil type (default=CRM)\n"
                                 " - BRM\n"
                                 " - CRM\n"
                                 " - TRM.WHOLE\n"
                                 " - TRM.ZOOM\n",
                                 gradientCoilType, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             keepNegativeValues,
             displayPlanes,
             displayCoilInfo,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcGradientWarpingCorrectionCommand::parse()" );

}


void gkg::GehcGradientWarpingCorrectionCommand::execute(
                                            const std::string& fileNameIn,
                                            const std::string& fileNameOut,
                                            const std::string& outputFormat,
                                            bool keepNegativeValues,
                                            bool displayPlanes,
                                            bool displayCoilInfo,
                                            const std::string& gradientCoilType,
                                            bool verbose )
{

  try
  {

    // sanity checks
    if ( ( gradientCoilType != "BRM" ) &&
         ( gradientCoilType != "CRM" ) &&
         ( gradientCoilType != "TRM.WHOLE" ) &&
         ( gradientCoilType != "TRM.ZOOM" ) )
    {

      throw std::runtime_error( "not a valid gradient coil type" );

    }
         

    // checking that data is readable and collecting sizes
    if ( verbose )
    {

      std::cout << "reading GEHC data file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< int16_t > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                       gkg::Volume< int16_t > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "gis" )
    {

      throw std::runtime_error( "wrong format, should be \'gis\'");

    }

    // collecting size(s)
    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;

    if ( inputVolumeProxy.getHeader().hasAttribute( "resolutionX" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "resolutionX",
                                                 resolutionX );

    }
    if ( inputVolumeProxy.getHeader().hasAttribute( "resolutionY" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "resolutionY",
                                                 resolutionY );

    }
    if ( inputVolumeProxy.getHeader().hasAttribute( "resolutionZ" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "resolutionZ",
                                                 resolutionZ );

    }


    if ( verbose )
    {

      std::cout << "data geometry: "
                << sizeX << " x "
                << lineCount << " x "
                << sliceCount << " x "
                << timeCount << std::endl;
      std::cout << "data resolution: "
                << resolutionX << "mm x "
                << resolutionY << "mm x "
                << resolutionZ << "mm" << std::endl;

    }

    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int32_t > boundingBox( 0, sizeX - 1,
                                             0, lineCount - 1,
                                             0, sliceCount - 1,
                                             0, 0 );
    gkg::Volume< int16_t > shortVolume;

    gkg::Volume< float > floatVolume;


    floatVolume.getHeader().addAttribute( "resolutionX",
                                          resolutionX );
    floatVolume.getHeader().addAttribute( "resolutionY",
                                          resolutionY );
    floatVolume.getHeader().addAttribute( "resolutionZ",
                                          resolutionZ );
    floatVolume.getHeader().addAttribute( "resolutionT",
                                        ( double )1.0 );


    // preparing converter(s)
    gkg::Converter< gkg::Volume< int16_t >, gkg::Volume< float > >
      shortToFloatConverter;
    gkg::Converter< gkg::Volume< float >, gkg::Volume< int16_t > >
      floatToShortConverter;

    // preparing thresholder
    gkg::LowerOrEqualToFunction< float > lowerOrEqualToFunction( 32767.0 );
    gkg::GreaterOrEqualToFunction< float > greaterOrEqualToFunction( 0.0 );
    gkg::Thresholder< gkg::Volume< float > > lowerThresholder(
                                                       lowerOrEqualToFunction,
                                                       32767.0 );
    gkg::Thresholder< gkg::Volume< float > > greaterThresholder(
                                                       greaterOrEqualToFunction,
                                                       0.0 );


    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }


    gkg::VolumeProxy< int16_t > outputVolumeProxy( sizeX,
                                                 lineCount,
                                                 sliceCount,
                                                 timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
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

    // estimating gradient warping correction 
    std::vector< gkg::GradientWarping2dCorrection< float >::Wedge2d >
        gradientWarpingWedge2ds;

    std::string gradientCoilName =
      gkg::ConfigurationInfo::getInstance().getGkgPath() +
        gkg::getDirectorySeparator() + "gehc" +
        gkg::getDirectorySeparator() + "gradwarp" +
        gkg::getDirectorySeparator() + "gw_coils.dat." + gradientCoilType;

    if ( verbose )
    {

      std::cout << "gradient warping file: " << gradientCoilName << std::endl;

    }

    gkg::GehcSignaGradientCoil< float > gradientCoil( gradientCoilName );

    if ( displayCoilInfo && verbose )
    {

     int32_t i;
     for ( i = 0; i < 10; i++ )
     {

       std::cout << "scaleX[ " << i << " ] = " << gradientCoil.getScaleX( i )
                 << std::endl;

     }
     for ( i = 0; i < 10; i++ )
     {

       std::cout << "scaleY[ " << i << " ] = " << gradientCoil.getScaleY( i )
                 << std::endl;

     }
     for ( i = 0; i < 10; i++ )
     {

       std::cout << "scaleZ[ " << i << " ] = " << gradientCoil.getScaleZ( i )
                 << std::endl;

     }
     std::cout << "delta = " << gradientCoil.getDelta() << std::endl;

     if ( verbose )
     {

       std::cout << "preparing gradient warping correction : " << std::flush;

     }

    }

    double start_loc, end_loc;
    inputVolumeProxy.getHeader().getAttribute(
                          "pool_header.rdb_hdr_series.start_loc", start_loc );
    inputVolumeProxy.getHeader().getAttribute(
                          "pool_header.rdb_hdr_series.end_loc", end_loc );
    bool reverseZ = ( start_loc < end_loc );

    std::vector< gkg::GradientWarping2dCorrection< float >::Plane >
      planes( sliceCount );
    int32_t s, trueSlice;
    double tmp = 0.0;
    for ( s = 0; s < sliceCount; s++ )
    {

      trueSlice = ( reverseZ ? sliceCount - 1 - s : s );
     
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point1[0]",
          tmp );
      planes[ trueSlice  ].point1.x = ( float )tmp / 10.0;
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point1[1]",
          tmp );
      planes[ trueSlice ].point1.y = ( float )tmp / 10.0;
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point1[2]",
          tmp );
      planes[ trueSlice ].point1.z = ( float )tmp / 10.0;

      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point2[0]",
          tmp );
      planes[ trueSlice ].point2.x = ( float )tmp / 10.0;
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point2[1]",
          tmp );
      planes[ trueSlice ].point2.y = ( float )tmp / 10.0;
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point2[2]",
          tmp );
      planes[ trueSlice ].point2.z = ( float )tmp / 10.0;

      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point3[0]",
          tmp );
      planes[ trueSlice ].point3.x = ( float )tmp / 10.0;
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point3[1]",
          tmp );
      planes[ trueSlice ].point3.y = ( float )tmp / 10.0;
      inputVolumeProxy.getHeader().getAttribute(
          std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
          gkg::StringConverter::toString( s ) +
          "].gw_point3[2]",
          tmp );
      planes[ trueSlice ].point3.z = ( float )tmp / 10.0;

      if ( displayPlanes && verbose )
      {

        std::cout << "plane[ " << trueSlice  << "].point1 = "
                  << planes[ trueSlice ].point1 << std::endl;
        std::cout << "plane[ " << trueSlice  << "].point2 = "
                  << planes[ trueSlice ].point2 << std::endl;
        std::cout << "plane[ " << trueSlice  << "].point3 = "
                  << planes[ trueSlice ].point3 << std::endl;

      }
              
    }

    // detecting swap between X and Y
    bool swapXY = false;
    if ( inputVolumeProxy.getHeader().hasAttribute(
           "pool_header.rdb_hdr_image.freq_dir" ) )
    {

      double freq_dir;
      inputVolumeProxy.getHeader().getAttribute(
                                           "pool_header.rdb_hdr_image.freq_dir",
                                           freq_dir );
      swapXY = ( ( int32_t )( freq_dir + 0.5 ) == 1 ? true : false );

    }

    // estimating correction
    if ( verbose )
    {

      std::cout << "estimating correction: " << std::flush;

    }
    gkg::GradientWarping2dCorrection< float >::getInstance().estimate(
                                                        gradientCoil,
                                                        planes,
                                                        gradientWarpingWedge2ds,
                                                        sizeX,
                                                        lineCount,
                                                        verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // looping over ranks(s)
    if ( verbose )
    {

      std::cout << "correcting: "
                << std::flush;

    }

    int32_t time;
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
      boundingBox.setLowerT( time );
      boundingBox.setUpperT( time );
      format =
        gkg::Reader::getInstance().partialBinaryRead<
                                     gkg::Volume< int16_t > >( inputVolumeProxy,
                                                               shortVolume,
                                                               boundingBox,
                                                               "gis" );
      if ( format != "gis" )
      {

        throw std::runtime_error( "wrong format, should be \'gis\'");

      }

      // convert short to float
      if ( verbose )
      {

        std::cout << "Converting->" << std::flush;

      }
      shortToFloatConverter.convert( shortVolume, floatVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 12 ) << std::flush;

      }

      // applying 2d gradient warping
      if ( verbose )
      {

         std::cout << "Gradient warping correction" << std::flush;

      }
      gkg::GradientWarping2dCorrection< float >::getInstance().apply(
                                                      floatVolume,
                                                      gradientWarpingWedge2ds,
                                                      swapXY,
                                                      verbose );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 27 ) << std::flush;

      }

      // removing negatives values
      if ( keepNegativeValues == false )
      {

        if ( verbose )
        {

          std::cout << "Thresholding->" << std::flush;

        }
        lowerThresholder.threshold( floatVolume,
                                    floatVolume );
        greaterThresholder.threshold( floatVolume,
                                      floatVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 14 ) << std::flush;

        }

      }


      // convert float to short
      if ( verbose )
      {

        std::cout << "Converting->" << std::flush;

      }
      floatToShortConverter.convert( floatVolume, shortVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 12 ) << std::flush;

      }

      // saving reconstructed slice to disk
      if ( verbose )
      {

        std::cout << "Saving to disk" << std::flush;

      }
      gkg::Writer::getInstance().partialBinaryWrite< gkg::Volume< int16_t > >(
                                                            outputVolumeProxy,
                                                            shortVolume,
                                                            boundingBox,
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

  }
  GKG_CATCH( "void gkg::GehcGradientWarpingCorrectionCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool keepNegativeValues, "
             "bool displayPlanes, "
             "bool displayCoilInfo, "
             "const std::string& gradientCoilType, "
             "bool verbose  )" );

}


RegisterCommandCreator( 
    GehcGradientWarpingCorrectionCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( keepNegativeValues ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( displayPlanes ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( displayCoilInfo ) +
    DECLARE_STRING_PARAMETER_HELP( gradientCoilType ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
