#include <gkg-processing-plugin-functors/RgbIntensityMapper/RgbIntensityMapperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>



//
// class RgbIntensityMapperCommand
//


gkg::RgbIntensityMapperCommand::RgbIntensityMapperCommand( int32_t argc,
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
  GKG_CATCH( "gkg::RgbIntensityMapperCommand::RgbIntensityMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RgbIntensityMapperCommand::RgbIntensityMapperCommand( 
                                                 const std::string& fileNameIn,
                      			         const std::string& fileNameOut,
                      	       	                 bool ascii,
                                                 const std::string& format,
                                                 bool verbose )
{

  try
  {

    execute( fileNameIn, fileNameOut,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RgbIntensityMapperCommand::RgbIntensityMapperCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::RgbIntensityMapperCommand::RgbIntensityMapperCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RgbIntensityMapperCommand::RgbIntensityMapperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RgbIntensityMapperCommand::~RgbIntensityMapperCommand()
{
}


std::string gkg::RgbIntensityMapperCommand::getStaticName()
{

  try
  {

    return "RgbIntensityMapper";

  }
  GKG_CATCH( "std::string gkg::RgbIntensityMapperCommand::getStaticName()" );

}


void gkg::RgbIntensityMapperCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "RGB intensity mapper for RGB maps",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameIn, fileNameOut,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RgbIntensityMapperCommand::parse()" );

}


void gkg::RgbIntensityMapperCommand::execute( const std::string& fileNameIn,
                      		              const std::string& fileNameOut,
                                              bool ascii,
                                              const std::string& format,
                                              bool verbose )
{

  try
  {

    // reading data
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< gkg::RGBComponent > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    gkg::Vector3d< int32_t > inputSize( 1, 1, 1 );
    int32_t inputSizeT = 1;
    volumeIn.getSize( inputSize, inputSizeT );

    gkg::Vector3d< double > inputResolution( 1.0, 1.0, 1.0 );
    double inputResolutionT = 1.0;
    volumeIn.getResolution( inputResolution, inputResolutionT );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // computing intensity
    if ( verbose )
    {

      std::cout << "computing intensity : " << std::flush;

    }
    gkg::Volume< float > volumeOut( inputSize, inputSizeT );
    volumeOut.setResolution( inputResolution, inputResolutionT );

    gkg::Volume< gkg::RGBComponent >::const_iterator
      i = volumeIn.begin(),
      ie = volumeIn.end();
    gkg::Volume< float >::iterator
      o = volumeOut.begin();
    while ( i != ie )
    {

      *o = std::sqrt( ( ( ( float )i->r / 255.0 ) *
                        ( ( float )i->r / 255.0 ) ) +
                      ( ( ( float )i->g / 255.0 ) *
                        ( ( float )i->g / 255.0 ) ) +
                      ( ( ( float )i->b / 255.0 ) *
                        ( ( float )i->b / 255.0 ) ) );
               
      ++ i;
      ++ o;

    }
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    // writing data
    if ( verbose )
    {

      std::cout << "writing '"
                << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      volumeOut,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::RgbIntensityMapperCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outType, "
             "const std::string& mode, "
             "double threshold1, double threshold2, "
             "double foreground, double background, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( RgbIntensityMapperCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
