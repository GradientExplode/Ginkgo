#include "GlobalTractographyViewer.h"

#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>
#include <QtGui/QApplication>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    bool singleView = false;
    bool verbose = false;
    gkg::Application application( argc, argv,
                                  "DWI global tractography viewer" );
    application.addSingleOption( "-singleView",
                                 "Viewer only contains a single view instead"
                                 " of 4 axial, sagittal, coronal and 3D views",
                                 singleView,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    QApplication qtApplication( argc, argv );

    gkg::GlobalTractographyViewer globalTractographyViewer( 0,
                                                            singleView,
                                                            verbose );
    globalTractographyViewer.show();

    return qtApplication.exec();
  
  }
  GKG_CATCH_COMMAND( result );

}
