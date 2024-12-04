// information about account and gkg path(s), configuration file(s)

#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::cout << "home path : "
              << gkg::ConfigurationInfo::getInstance().getHomePath()
              << std::endl;
    std::cout << "gkg path : "
              << gkg::ConfigurationInfo::getInstance().getGkgPath()
              << std::endl;
    std::cout << "tmp path : "
              << gkg::ConfigurationInfo::getInstance().getTmpPath()
              << std::endl;
    std::cout << "plugin configuration file : "
              << gkg::ConfigurationInfo::getInstance().
                                                    getGkgPluginConfigFileName()
              << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
