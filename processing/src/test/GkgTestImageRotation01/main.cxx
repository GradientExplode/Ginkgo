#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-processing-transform/RotationY3d.h>
#include <gkg-processing-transform/RotationZ3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>




int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    float coronalAngle = -16.7;
    float sagittalAngle = -0.3;
    bool verbose = false;

    gkg::Application application( argc, argv,
                                  "Image rotation tester" );
    application.addSingleOption( "-c",
                                 "Coronal angle in degrees",
                                 coronalAngle,
                                 true );
    application.addSingleOption( "-s",
                                 "Sagittal angle in degrees",
                                 sagittalAngle,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();


    gkg::Vector3d< float > inputOrientation( 0.164557, -0.272883, -0.947869 );
    gkg::Vector3d< float > outputOrientation( 0.169461, -0.006982, -0.985512 );


    //////////////////////////// homogeneous
    gkg::HomogeneousTransform3d< float >
      h( 0.999711, 0.0212093, -0.0113299, 0,
          -0.0235701, 0.957598, -0.287142, 0,
         0.00475944, 0.287326, 0.957821, 0,
          0, 0, 0, 1 );


    gkg::Vector3d< float > processedOutputOrientation;
    h.getDirect( inputOrientation, processedOutputOrientation );
    std::cout << "processed output( direct ): " << processedOutputOrientation
              << std::endl;


    std::cout << std::endl << "theoretical output: " << outputOrientation
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

