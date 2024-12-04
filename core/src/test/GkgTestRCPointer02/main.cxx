#include <gkg-core-pattern/RCPointer.h>
#include <cassert>
#include <iostream>


int main() 
{

  gkg::RCPointer< int32_t > i1( new int32_t );

  *i1 = 2;

  gkg::RCPointer< int32_t > i2;

  i2 = i1;


  std::cout << *i2 << std::endl;

  *i2 = 3;

  std::cout << *i1 << std::endl;

  return EXIT_SUCCESS;

}
