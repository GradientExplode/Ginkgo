#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-mesh/CylinderCollision3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    gkg::Vector3d< float >
      selectedFiberCenter( 0.0178019, 0.0885994, 0.0866025 );
    gkg::Vector3d< float >
      newSelectedAxis( -0.350004, 0.493667, 0.796109 );
    float selectedHalfLength = 0.0866024;
    float selectedRadius = 0.00212908;

    gkg::Vector3d< float >
      currentFiberCenter( 0.0942356, 0.0151407, 0.0866025 );
    gkg::Vector3d< float >
      currentAxis( 0.665487, -0.502657, 0.551782 );
    float currentHalfLength = 0.0866026;
    float currentRadius = 0.00260367;

    std::cout << "collision : "
              << gkg::hasCylinderCollision3d( selectedFiberCenter,
                                              newSelectedAxis,
                                              selectedHalfLength,
                                              selectedRadius,
                                              currentFiberCenter,
                                              currentAxis,
                                              currentHalfLength,
                                              currentRadius )
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
