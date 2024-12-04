#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-graphics-X11-core/Session.h>

#include "Viewer2dProcess.h"


//
// main function
//

int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameIn;
    float scale = 1.0;
    bool verbose = false;
    bool doubleBuffering = false;
    //
    // command line arguments
    //
    gkg::Application application( argc, argv,
                                "2D viewer" );
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

    // initializing Graphical User Interface
    gkg::Session& session = gkg::Session::getInstance();
    session.initialize( "GkgViewer2d", argc, argv );

    // launching 2D viewer process
    gkg::Viewer2dProcess viewer2dProcess( scale,
                                          verbose,
                                          result );
    viewer2dProcess.execute( fileNameIn );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
