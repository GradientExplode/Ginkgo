#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <vector>
#include <fstream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::vector< double > u;
    std::vector< double > v;
    std::vector< double > w;

    gkg::Application application( argc, argv,
                                  "Test for Rotation3d functions" );
    application.addSeriesOption( "-u",
                                 "First trieder vector",
                                 u,
                                 3, 3 );
     application.addSeriesOption( "-v",
                                 "Second trieder vector",
                                 v,
                                 3, 3 );
     application.addSeriesOption( "-w",
                                 "Third trieder vector",
                                 w,
                                 3, 3 );
    application.initialize();


    gkg::Vector3d< double > U;
    gkg::Vector3d< double > V;
    gkg::Vector3d< double > W;

    U.x = u[ 0 ];
    U.y = u[ 1 ];
    U.z = u[ 2 ];

    V.x = v[ 0 ];
    V.y = v[ 1 ];
    V.z = v[ 2 ];

    W.x = w[ 0 ];
    W.y = w[ 1 ];
    W.z = w[ 2 ];

    std::cout << "angles in degrees : "
              << gkg::getXYZAnglesFromTrieder( U, V, W, true )
              << std::endl;
    std::cout << "angles in radians : "
              << gkg::getXYZAnglesFromTrieder( U, V, W, false )
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

