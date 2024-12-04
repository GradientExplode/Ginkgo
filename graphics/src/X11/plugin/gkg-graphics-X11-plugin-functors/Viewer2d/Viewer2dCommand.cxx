#include <gkg-graphics-X11-plugin-functors/Viewer2d/Viewer2dCommand.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2d/Viewer2dProcess.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cstdlib>


gkg::Viewer2dCommand::Viewer2dCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin,
                                       bool removeFirst )
                    : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Viewer2dCommand::Viewer2dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Viewer2dCommand::Viewer2dCommand( const std::string& fileNameIn,
                    		       float scale,
                    		       bool verbose,
                    		       bool doubleBuffering )
                    : gkg::Command()
{

  try
  {

    execute( fileNameIn, scale, verbose, doubleBuffering );

  }
  GKG_CATCH( "gkg::Viewer2dCommand::Viewer2dCommand( "
             "const std::string& fileNameIn, float scale, "
             "bool verbose, bool doubleBuffering )" );

}


gkg::Viewer2dCommand::Viewer2dCommand( const gkg::Dictionary& parameters )
                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_FLOATING_PARAMETER( parameters, float, scale );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, doubleBuffering );

    execute( fileNameIn, scale, verbose, doubleBuffering );

  }
  GKG_CATCH( "gkg::Viewer2dCommand::Viewer2dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Viewer2dCommand::~Viewer2dCommand()
{
}


std::string gkg::Viewer2dCommand::getStaticName()
{

  try
  {

    return "Viewer2d";

  }
  GKG_CATCH( "std::string gkg::Viewer2dCommand::getStaticName()" );

}


void gkg::Viewer2dCommand::parse()
{

  try
  {

    std::string fileNameIn;
    float scale = 1.0;
    bool verbose = false;
    bool doubleBuffering = false;

    gkg::Application application( _argc, _argv,
                                  "2D viewer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-scale",
                                 "View scale factor (default=1.0)",
                                 scale,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.addSingleOption( "-dbuf",
                                 "Double buffering",
                                 doubleBuffering, 
                                 true );
    application.initialize();

    execute( fileNameIn, scale, verbose, doubleBuffering );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Viewer2dCommand::parse()" );

}


void gkg::Viewer2dCommand::execute( const std::string& fileNameIn,
                     		    float scale,
                     		    bool verbose,
                     		    bool /* doubleBuffering */ )
{

  try
  {

    int result = EXIT_SUCCESS;

    // initializing Graphical User Interface
    gkg::Session& session = gkg::Session::getInstance();
    session.initialize( "GkgViewer2d", _argc, _argv );

    // launching 2D viewer process
    gkg::Viewer2dProcess viewer2dProcess( scale,
                                          verbose,
                                          result );
    viewer2dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::Viewer2dCommand::execute( "
             "const std::string& fileNameIn, float scale, "
             "bool verbose, bool doubleBuffering )" );

}


RegisterCommandCreator(
    Viewer2dCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( doubleBuffering ) );
