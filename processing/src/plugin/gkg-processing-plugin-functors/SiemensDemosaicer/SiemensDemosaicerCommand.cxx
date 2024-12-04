#include <gkg-processing-plugin-functors/SiemensDemosaicer/SiemensDemosaicerCommand.h>
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


gkg::SiemensDemosaicerCommand::SiemensDemosaicerCommand( int32_t argc,
                                 			 char* argv[],
                                 			 bool loadPlugin,
                                 			 bool removeFirst )
                             : gkg::Command( argc, argv,
                                             loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SiemensDemosaicerCommand::SiemensDemosaicerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SiemensDemosaicerCommand::SiemensDemosaicerCommand(
                              			const std::string& fileNameIn,
                              			const std::string& fileNameOut,
                              			const std::string& outputFormat,
                              			bool verbose )
                             : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::SiemensDemosaicerCommand::SiemensDemosaicerCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool verbose )" );

}


gkg::SiemensDemosaicerCommand::SiemensDemosaicerCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outputFormat, verbose );

  }
  GKG_CATCH( "gkg::SiemensDemosaicerCommand::SiemensDemosaicerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SiemensDemosaicerCommand::~SiemensDemosaicerCommand()
{
}


std::string gkg::SiemensDemosaicerCommand::getStaticName()
{

  try
  {

    return "SiemensDemosaicer";

  }
  GKG_CATCH( "std::string gkg::SiemensDemosaicerCommand::getStaticName()" );

}


void gkg::SiemensDemosaicerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Siemens Syngo deMOSAICer tool",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input MOSAIC image file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output deMOSAIC image file name"
                                 " (default=input file name)",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn, fileNameOut, outputFormat, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SiemensDemosaicerCommand::parse()" );

}


void gkg::SiemensDemosaicerCommand::execute( const std::string& fileNameIn,
                              		     const std::string& fileNameOut,
                              		     const std::string& outputFormat,
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

    // reading input proxy
    if ( verbose )
    {

      std::cout << "reading MOSAIC file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::Volume< int16_t > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                   gkg::Volume< int16_t > >( fileNameIn,
                                                             inputVolumeProxy );

    gkg::Dictionary dicomDictionary;
    if ( inputVolumeProxy.getHeader().hasAttribute( "dicom" ) )
    {

      inputVolumeProxy.getHeader().getAttribute( "dicom", dicomDictionary );


    }
    else
    {

      throw std::runtime_error( "no DICOM attribute found" );

    }
    
    // sanity checks
    if ( dicomDictionary.find( "(0008,0070)" ) != dicomDictionary.end() )
    {

      std::string Manufacturer = dicomDictionary[ "(0008,0070)" ]->getValue<
                                                                std::string >();
      if ( Manufacturer != "SIEMENS" )
      {

        throw std::runtime_error( "not a SIEMENS data" );

      }

    }

    if ( dicomDictionary.find( "(0008,0008)" ) != dicomDictionary.end() )
    {

      std::string ImageType = dicomDictionary[ "(0008,0008)" ]->getValue<
                                                                std::string >();
      if ( ImageType.find( "MOSAIC" ) == std::string::npos )
      {

        throw std::runtime_error( "not a MOSAIC data" );

      }

    }
    else
    {

      throw std::runtime_error( "tag (0008,0008) missing" );

    }

    if ( inputVolumeProxy.getSizeZ() != 1 )
    {

      throw std::runtime_error( "MOSAIC slice count is not equal to 1" );

    }


    // preparing a bounding box and a slice data for slice reading
    int32_t inSizeX = inputVolumeProxy.getSizeX();
    int32_t inLineCount = inputVolumeProxy.getSizeY();
    int32_t inTimeCount = inputVolumeProxy.getSizeT();
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, inSizeX - 1,
                                                  0, inLineCount - 1,
                                                  0, 0,
                                                  0, 0 );
    gkg::Volume< int16_t > inputSubVolume( inSizeX, inLineCount );


    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }

    int32_t outSliceCount = 0;
    if ( dicomDictionary.find( "(0019,100a)" ) != dicomDictionary.end() )
    {

       try
       {

         outSliceCount = ( int32_t )(
                         dicomDictionary[ "(0019,100a)" ]->getValue< double >()+
                         0.5 );

       }
       catch ( std::exception& )
       {

         try
         {

           outSliceCount = ( int32_t )( dicomDictionary[ "(0019,100a)" ]->
                                                            getScalar() + 0.5 );

         }
         catch ( std::exception& )
         {

           throw std::runtime_error( "failed to read tag (0019,100a)" );

         }

       }

    }
    else
    {

      throw std::runtime_error( "tag (0019,100a) missing" );

    }
    int32_t divider = 1;
    while ( divider * divider < outSliceCount )
    {

      ++ divider;

    }

    int32_t outSizeX = inSizeX / divider;
    int32_t outLineCount = inLineCount / divider;
    int32_t outTimeCount = inTimeCount;

    gkg::VolumeProxy< int16_t >
      outputVolumeProxy( outSizeX, outLineCount, outSliceCount, outTimeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "sizeX" ] = outSizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = outLineCount;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = outSliceCount;
    outputVolumeProxy.getHeader()[ "sizeT" ] = outTimeCount;
    //outputVolumeProxy.getHeader()[ "resolutionX" ] = resolutionX;
    //outputVolumeProxy.getHeader()[ "resolutionY" ] = resolutionY;
    //outputVolumeProxy.getHeader()[ "resolutionZ" ] = resolutionZ;
    //outputVolumeProxy.getHeader()[ "resolutionT" ] = resolutionT;

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, outSizeX - 1,
                                                   0, outLineCount - 1,
                                                   0, outSliceCount - 1,
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
                                            outLineCount,
                                            outSliceCount );

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    // looping over ranks(s)
    if ( verbose )
    {

      std::cout << "demosaicing: " << std::flush;

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

      // reading reco data from 2dsequ Bruker file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      gkg::Reader::getInstance().partialBinaryRead<
                                            gkg::Volume< int16_t > >(
                                                             inputVolumeProxy,
                                                             inputSubVolume,
                                                             inputBoundingBox );

      // filling output sub-volume
      for ( slice = 0; slice < outSliceCount; slice++ )
      {

        int32_t offsetX = ( slice % divider ) * outSizeX;
        int32_t offsetLine = ( slice / divider ) * outLineCount;

        for ( line = 0; line < outLineCount; line++ )
        {

          for ( x = 0; x < outSizeX; x++ )
          {

            outputSubVolume( x, line, slice ) =
              inputSubVolume( offsetX + x, offsetLine + line );

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
  GKG_CATCH( "void gkg::SiemensDemosaicerCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool verbose )" );

}


RegisterCommandCreator( SiemensDemosaicerCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
