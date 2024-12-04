// string <=> scalar convertion

#include <gkg-core-io/StringConverter.h>
#include <iostream>
#include <cstdlib>


int main( int /*argc*/, char* /*argv*/[] )
{

  int32_t intValue = 0;
  if ( gkg::StringConverter::toScalar( "18", intValue ) )
  {

    std::cout << "int value (decimal base) = " << intValue << std::endl;

  }
  if ( gkg::StringConverter::toScalar( "18", intValue, 16 ) )
  {

    std::cout << "int value (hexadecimal base) = " << intValue << std::endl;

  }

  std::cout << gkg::StringConverter::toString( 1.086, 'f', 10, 5 ) << std::endl;
  std::cout << gkg::StringConverter::toString( 1.086, 'F', 10, 2 ) << std::endl;
  std::cout << gkg::StringConverter::toString( 1.086, 'g', 10, 2 ) << std::endl;
  std::cout << gkg::StringConverter::toString( 10.086, 'G', 10, 4 )
            << std::endl;
  std::cout << gkg::StringConverter::toString( 10.086, 'e', 10, 2 )
            << std::endl;
  std::cout << gkg::StringConverter::toString( 10.086, 'E', 10, 2 )
            << std::endl;

  return EXIT_SUCCESS;

}
