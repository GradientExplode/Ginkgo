// information about account and gkg path(s), configuration file(s)

#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    uint64_t ram = 0;
    uint64_t freeRam = 0;
    uint64_t swap = 0;

    gkg::SystemInfo::getInstance().getMemory( ram, freeRam, swap );

    std::cout << "RAM : "
              << ram
              << std::endl;
    std::cout << "free RAM : "
              << freeRam
              << std::endl;
    std::cout << "swap : "
              << swap
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
