// Directory class

#include <gkg-communication-sysinfo/FileFinder.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::cout << "environment path : "
              << gkg::ConfigurationInfo::getInstance().getEnvPath()
              << std::endl;

    gkg::FileFinder finder;

    if ( finder.locateFromPath( "gnuplot" ) )
    {

      std::cout << "gnuplot was found here : " << finder.getFullPath() 
                << std::endl;

    }
    else
    {

      std::cerr << "gnuplot was not found in the PATH" << std::endl;

    }


    if ( finder.locateFromDirectory( "gnuplot_x11", "/usr/libexec/gnuplot" ) )
    {

      std::cout << "gnuplot_x11 was found here : " << finder.getFullPath() 
                << std::endl;
    }
    else
    {

      std::cerr << "gnuplot_x11 was not found from /usr/libexec/gnuplot" 
                << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
