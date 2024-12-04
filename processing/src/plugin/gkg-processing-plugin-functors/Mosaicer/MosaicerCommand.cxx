#include <gkg-processing-plugin-functors/Mosaicer/MosaicerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


gkg::MosaicerCommand::MosaicerCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin,
                                       bool removeFirst )
                    : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::MosaicerCommand::MosaicerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MosaicerCommand::MosaicerCommand( const std::string& fileNameIn,
                     		       const std::string& fileNameOut,
                     		       const std::string& outputFormat,
                     		       bool writeSiemensTags,
                     		       bool verbose )
                    : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outputFormat, writeSiemensTags, verbose );

  }
  GKG_CATCH( "gkg::MosaicerCommand::MosaicerCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool writeSiemensTags, "
             "bool verbose )" );

}


gkg::MosaicerCommand::MosaicerCommand( const gkg::Dictionary& parameters )
                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, writeSiemensTags );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn, fileNameOut, outputFormat, writeSiemensTags, verbose );

  }
  GKG_CATCH( "gkg::MosaicerCommand::MosaicerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MosaicerCommand::~MosaicerCommand()
{
}


std::string gkg::MosaicerCommand::getStaticName()
{

  try
  {

    return "Mosaicer";

  }
  GKG_CATCH( "std::string gkg::MosaicerCommand::getStaticName()" );

}


void gkg::MosaicerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool writeSiemensTags = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "MOSAICer tool",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input image file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output MOSAIC image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-siemens",
                                 "Write Siemens private Dicom tags"
                                 " (default=false)",
                                 writeSiemensTags,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, outputFormat, writeSiemensTags, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MosaicerCommand::parse()" );

}


void gkg::MosaicerCommand::execute( const std::string& fileNameIn,
                     		    const std::string& fileNameOut,
                     		    const std::string& outputFormat,
                     		    bool writeSiemensTags,
                     		    bool verbose )
{

  try
  {

    // sanity checks
    if ( fileNameOut == fileNameIn )
    {

      throw std::runtime_error(
                               "input and ouput file names must be different" );

    }

    if ( outputFormat == "dicom" )
    {

      throw std::runtime_error( "Dicom output format not supported" );

    }

    // reading input proxy
    if ( verbose )
    {

      std::cout << "reading file: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::Volume< int16_t > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                   gkg::Volume< int16_t > >( fileNameIn,
                                                             inputVolumeProxy );

    // preparing a bounding box and a volume data for volume reading
    int32_t inSizeX = inputVolumeProxy.getSizeX();
    int32_t inLineCount = inputVolumeProxy.getSizeY();
    int32_t inSliceCount = inputVolumeProxy.getSizeZ();
    int32_t inTimeCount = inputVolumeProxy.getSizeT();
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, inSizeX - 1,
                                                  0, inLineCount - 1,
                                                  0, inSliceCount - 1,
                                                  0, 0 );
    gkg::Volume< int16_t > inputSubVolume( inSizeX, 
                                           inLineCount,
                                           inSliceCount );

    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }

    int32_t divider = 1;
    while ( divider * divider < inSliceCount )
    {

      ++ divider;

    }

    int32_t outSizeX = inSizeX * divider;
    int32_t outLineCount = inLineCount * divider;
    int32_t outTimeCount = inTimeCount;

    gkg::VolumeProxy< int16_t >
      outputVolumeProxy( outSizeX, outLineCount, 1, outTimeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "sizeX" ] = outSizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = outLineCount;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = 1;
    outputVolumeProxy.getHeader()[ "sizeT" ] = outTimeCount;

    if ( writeSiemensTags )
    {

      gkg::Dictionary dicomDictionary;

      if ( outputVolumeProxy.getHeader().hasAttribute( "dicom" ) )
      {

        outputVolumeProxy.getHeader().getAttribute( "dicom", dicomDictionary );

      }

      std::string imageType = "MOSAIC";
      if ( dicomDictionary.find( "(0008,0008)" ) != dicomDictionary.end() )
      {

        imageType = dicomDictionary[ "(0008,0008)" ]->getValue< std::string >();

        if ( imageType.find( "MOSAIC" ) == std::string::npos )
        {

          imageType += "MOSAIC";

        }

      }
      
      dicomDictionary[ "(0008,0008)" ] = imageType;
      dicomDictionary[ "(0008,0070)" ] = std::string( "SIEMENS" );
      dicomDictionary[ "(0019,100a)" ] = inSliceCount;
      outputVolumeProxy.getHeader().addAttribute( "dicom", dicomDictionary );

    }

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, outSizeX - 1,
                                                   0, outLineCount - 1,
                                                   0, 0,
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
    gkg::Volume< int16_t > outputSubVolume( outSizeX,
                                            outLineCount );

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    // looping over ranks(s)
    if ( verbose )
    {

      std::cout << "mosaicing: " << std::flush;

    }

    int32_t time, slice, line, x;
    for ( time = 0; time < outTimeCount; time++ )
    {

      if ( verbose )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << outTimeCount
                  << " ] " << std::flush;

      }

      // reading data from input file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      gkg::Reader::getInstance().partialBinaryRead<
                                            gkg::Volume< int16_t > >(
                                                             inputVolumeProxy,
                                                             inputSubVolume,
                                                             inputBoundingBox );

      // filling output sub-volume
      for ( slice = 0; slice < inSliceCount; slice++ )
      {

        int32_t offsetX = ( slice % divider ) * inSizeX;
        int32_t offsetLine = ( slice / divider ) * inLineCount;

        for ( line = 0; line < inLineCount; line++ )
        {

          for ( x = 0; x < inSizeX; x++ )
          {

            outputSubVolume( offsetX + x, offsetLine + line ) =
              inputSubVolume( x, line, slice );

          }

        }

      }

      // saving sub-volume(s) to disk
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );
      gkg::Writer::getInstance().partialBinaryWrite<
                                            gkg::Volume< int16_t > >(
                                                              outputVolumeProxy,
                                                              outputSubVolume,
                                                              outputBoundingBox,
                                                              outputFormat );

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "void gkg::MosaicerCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool writeSiemensTags, "
             "bool verbose )" );

}


RegisterCommandCreator( MosaicerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( writeSiemensTags ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
