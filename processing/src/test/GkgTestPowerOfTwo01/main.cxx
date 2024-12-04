#include <gkg-processing-algobase/PowerOfTwo.h>
#include <iostream>
#include <cstdlib>


int main()
{

  std::cout << gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 33 )
            << std::endl;
  std::cout << gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 32 )
            << std::endl;
  std::cout << gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 21.3 )
            << std::endl;

  std::cout << gkg::PowerOfTwo::getInstance().getStrictlyGreater( 32 )
            << std::endl;

  std::cout << gkg::PowerOfTwo::getInstance().getStrictlyLower( 32 )
            << std::endl;

  std::cout << gkg::PowerOfTwo::getInstance().getStrictlyLower( 1 )
            << std::endl;

  std::cout << gkg::PowerOfTwo::getInstance().getClosest( ( int16_t )51 )
            << std::endl;
  std::cout << gkg::PowerOfTwo::getInstance().getClosest( ( int16_t )40 )
            << std::endl;

  return EXIT_SUCCESS;

}
