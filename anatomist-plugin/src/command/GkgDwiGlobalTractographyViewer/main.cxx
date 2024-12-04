#include "GlobalTractographyViewer.h"

#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>

#include <QApplication>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    int32_t openGLRefreshPeriod = 1000;
    bool verbose = false;
    gkg::Application application( argc, argv,
                                  "DWI global tractography viewer" );
    application.addSingleOption( "-openGLRefreshPeriod",
                                 "Refresh really OpenGL scene every XXXX GL"
                                 " events (default=1000)",
                                 openGLRefreshPeriod,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    QApplication qtApplication( argc, argv );

    gkg::GlobalTractographyViewer
      globalTractographyViewer( 0, openGLRefreshPeriod, verbose );
    globalTractographyViewer.show();

    result = qtApplication.exec();
  
    return result;

  }
  GKG_CATCH_COMMAND( result );

}
