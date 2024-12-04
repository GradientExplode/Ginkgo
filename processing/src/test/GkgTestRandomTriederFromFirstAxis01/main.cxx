#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::vector< double > firstAxisVector;

    gkg::Application application( argc, argv,
                                  "Unit test for random trieder "
                                  "from first axis" );
    application.addSeriesOption( "-i",
                                 "Input 3D vector corresponding to first axis",
                                 firstAxisVector,
                                 3, 3 );
    application.initialize();

    gkg::Vector3d< double > firstAxis( firstAxisVector[ 0 ],
                                       firstAxisVector[ 1 ],
                                       firstAxisVector[ 2 ] );
    firstAxis.normalize();
    gkg::Vector3d< double > secondAxis;
    gkg::Vector3d< double > thirdAxis;

    int32_t t = 0;
    for ( t = 0; t < 10; t++ )
    {

      gkg::getRandomOrthonormalTriederFromFirstAxis( firstAxis,
                                                     secondAxis,
                                                     thirdAxis );
      std::cout << "---------------------------------------" << std::endl;
      std::cout << "first axis  : " << firstAxis << std::endl;
      std::cout << "second axis : " << secondAxis << std::endl;
      std::cout << "third axis  : " << thirdAxis << std::endl;
      

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
