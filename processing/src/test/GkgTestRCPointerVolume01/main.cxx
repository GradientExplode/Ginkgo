#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <iostream>


int main()
{

  gkg::RCPointer< gkg::Volume< uint8_t > >
    volume1( new gkg::Volume< uint8_t >( 4, 4 ) );
  volume1->getHeader().addAttribute( "foo", std::string( "dummy" ) );
  volume1->fill( 0 );
  ( *volume1 )( 1, 1 ) = 255;
  ( *volume1 )( 1, 2 ) = 255;
  ( *volume1 )( 2, 1 ) = 255;
  ( *volume1 )( 2, 2 ) = 255;

  gkg::RCPointer< gkg::Volume< uint8_t > > volume2 = volume1;

  int i, j;
  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

      std::cout << ( int32_t )( *volume2 )( i, j ) << " ";

    }
    std::cout << std::endl;

  }

  std::string aString;
  volume2->getHeader().getAttribute( "foo", aString );
  std::cout << "foo = " << aString << std::endl;

  return EXIT_SUCCESS;

}
