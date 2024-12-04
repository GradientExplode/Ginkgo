#include <gkg-processing-plugin-functors/RescalerWithSlopeAndIntercept/RescalerWithSlopeAndInterceptCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
//   RescalerWithSlopeAndInterceptCommand
//

gkg::RescalerWithSlopeAndInterceptCommand::RescalerWithSlopeAndInterceptCommand(
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
  GKG_CATCH( "gkg::RescalerWithSlopeAndInterceptCommand::"
             "RescalerWithSlopeAndInterceptCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RescalerWithSlopeAndInterceptCommand::RescalerWithSlopeAndInterceptCommand(
                                                 const std::string& fileNameIn,
                        		         const std::string& fileNameOut,
                        		         bool ascii,
                        		         const std::string& format,
                        		         bool verbose )
                                          : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RescalerWithSlopeAndInterceptCommand::"
             "RescalerWithSlopeAndInterceptCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::RescalerWithSlopeAndInterceptCommand::RescalerWithSlopeAndInterceptCommand(
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
    
    execute( fileNameIn, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RescalerWithSlopeAndInterceptCommand::"
             "RescalerWithSlopeAndInterceptCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RescalerWithSlopeAndInterceptCommand::
                                         ~RescalerWithSlopeAndInterceptCommand()
{
}


std::string gkg::RescalerWithSlopeAndInterceptCommand::getStaticName()
{

  try
  {

    return "RescalerWithSlopeAndIntercept";

  }
  GKG_CATCH( "std::string "
             "gkg::RescalerWithSlopeAndInterceptCommand::getStaticName()" );

}


void gkg::RescalerWithSlopeAndInterceptCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Volume rescaler with slope and intercept",
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

    execute( fileNameIn, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::RescalerWithSlopeAndInterceptCommand::parse()" );

}


void gkg::RescalerWithSlopeAndInterceptCommand::execute(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 bool ascii,
                                                 const std::string& format,
                                                 bool verbose )
{

  try
  {

    //
    // reading and converting on the fly to float volume
    //
    if ( verbose )
    {

      std::cout << "reading and rescaling '" << fileNameIn << "' : "
                << std::flush;

    }
    gkg::Volume< float > volume;
    gkg::TypedVolumeReaderProcess< float > typedVolumeReaderProcess( volume );
    typedVolumeReaderProcess.execute( fileNameIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // writing the output float volume
    //
    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      volume,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::RescalerWithSlopeAndInterceptCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( RescalerWithSlopeAndInterceptCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
