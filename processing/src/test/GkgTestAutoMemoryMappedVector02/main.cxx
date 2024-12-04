#include "NewAutoMemoryMappedVector_i.h"
#include "NewVolume_i.h"
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <vector>



int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::cout << "--------------------------->new volume 1" << std::endl;
    gkg::NewVolume< gkg::Vector3d< float > > volume1( 100, 1024, 1024 );

    std::cout << "volume1 is memory mapped : " << volume1.isMemoryMapped()
              << std::endl;

    std::cout << "--------------------------->filling volume 1" << std::endl;
    volume1.fill( gkg::Vector3d< float >( 1, 2, 3 ) );

    std::cout << "--------------------------->new volume 2" << std::endl;
    gkg::NewVolume< gkg::Vector3d< float > > volume2;

    std::cout << "--------------------------->reallocating volume 2" << std::endl;
    volume2.reallocate( 100, 1024, 1024 );

    std::cout << "volume2 is memory mapped : " << volume2.isMemoryMapped()
              << std::endl;

    std::cout << "--------------------------->filling volume 2" << std::endl;

    volume2.fill( gkg::Vector3d< float >( 1, 2, 3 ) );

    std::cout << "--------------------------->new volume 3" << std::endl;
    gkg::NewVolume< gkg::Vector3d< float > > volume3;

    std::cout << "--------------------------->reallocating volume 3" << std::endl;
    volume3.reallocate( 100, 1024, 1024 );

    std::cout << "volume3 is memory mapped : " << volume3.isMemoryMapped()
              << std::endl;

    std::cout << "volume3( 0, 0, 0 ) = " << volume3( 0, 0, 0 ) << std::endl;

    std::cout << "--------------------------->filling volume 3" << std::endl;

    volume3.setAutoMemoryVerbose();

    volume3.fill( gkg::Vector3d< float >( 1, 2, 3 ), true );

    volume1( 0, 0, 0 ).x = 0;
    volume2( 0, 0, 0 ).x = 0;
    volume3( 0, 0, 0 ).x = 0;


    std::cout << "--------------------------->end of main" << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
