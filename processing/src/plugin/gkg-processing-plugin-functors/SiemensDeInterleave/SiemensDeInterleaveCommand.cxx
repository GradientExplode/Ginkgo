#include <gkg-processing-plugin-functors/SiemensDeInterleave/SiemensDeInterleaveCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


gkg::SiemensDeInterleaveCommand::SiemensDeInterleaveCommand( int32_t argc,
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
  GKG_CATCH( "gkg::SiemensDeInterleaveCommand::SiemensDeInterleaveCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SiemensDeInterleaveCommand::SiemensDeInterleaveCommand(
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
  GKG_CATCH( "gkg::SiemensDeInterleaveCommand::SiemensDeInterleaveCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool verbose )" );

}


gkg::SiemensDeInterleaveCommand::SiemensDeInterleaveCommand(
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
  GKG_CATCH( "gkg::SiemensDeInterleaveCommand::SiemensDeInterleaveCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SiemensDeInterleaveCommand::~SiemensDeInterleaveCommand()
{
}


std::string gkg::SiemensDeInterleaveCommand::getStaticName()
{

  try
  {

    return "SiemensDeInterleave";

  }
  GKG_CATCH( "std::string gkg::SiemensDeInterleaveCommand::getStaticName()" );

}


void gkg::SiemensDeInterleaveCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Siemens Syngo deInterleave tool",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input interleaved image file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output deInterleaved image file name",
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
  GKG_CATCH_FUNCTOR( "void gkg::SiemensDeInterleaveCommand::parse()" );

}


void gkg::SiemensDeInterleaveCommand::execute( const std::string& fileNameIn,
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

      std::cout << "reading interleaved file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::Volume< int16_t > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                   gkg::Volume< int16_t > >( fileNameIn,
                                                             inputVolumeProxy );

    // preparing a bounding box and a volume data for volume reading
    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t sizeY = inputVolumeProxy.getSizeY();
    int32_t sizeZ = inputVolumeProxy.getSizeZ();
    int32_t sizeT = inputVolumeProxy.getSizeT();
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, sizeY - 1,
                                                  0, sizeZ - 1,
                                                  0, 0 );
    gkg::Volume< int16_t > inputSubVolume( sizeX, sizeY, sizeZ );

    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }

    gkg::VolumeProxy< int16_t >
      outputVolumeProxy( sizeX, sizeY, sizeZ, sizeT );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, sizeX - 1,
                                                   0, sizeY - 1,
                                                   0, sizeZ - 1,
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
    gkg::Volume< int16_t > outputSubVolume( sizeX,
                                            sizeY,
                                            sizeZ );

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    // looping over ranks(s)
    if ( verbose )
    {

      std::cout << "deinterleaving: " << std::flush;

    }

    int32_t time, z, y, x;
    for ( time = 0; time < sizeT; time++ )
    {

      if ( verbose )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << sizeT
                  << " ] " << std::flush;

      }

      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      gkg::Reader::getInstance().partialBinaryRead<
                                            gkg::Volume< int16_t > >(
                                                             inputVolumeProxy,
                                                             inputSubVolume,
                                                             inputBoundingBox );

      int32_t k1 = sizeZ % 2;
      int32_t k2 = 1 - k1;
      int32_t k3 = k2 - k1;
      int32_t nOver2 = ( sizeZ + k1 ) / 2;

      // filling output sub-volume
      for ( z = 0; z < sizeZ; z++ )
      {

        int32_t newZ = 2 * ( z % nOver2 ) - k3 * ( z / nOver2 ) + k2;

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            outputSubVolume( x, y, newZ ) = inputSubVolume( x, y, z );

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
  GKG_CATCH( "void gkg::SiemensDeInterleaveCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& outputFormat, bool verbose )" );

}


RegisterCommandCreator( SiemensDeInterleaveCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
