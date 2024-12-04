// Directory class

#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
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

    gkg::Directory directory(
      gkg::ConfigurationInfo::getInstance().getHomePath() );

    std::cout << "Files:" << std::endl;
    std::set< std::string > files = directory.getFiles();
    std::set< std::string >::const_iterator f = files.begin(), fe = files.end();
    while ( f != fe )
    {

      std::cout << "  " << *f << std::endl;
      ++ f;

    }

    std::cout << "Directories:" << std::endl;
    std::set< std::string > directories = directory.getSubDirectories();
    std::set< std::string >::const_iterator d = directories.begin(),
                                            de = directories.end();
    while ( d != de )
    {

      std::cout << "  " << *d << std::endl;
      ++ d;

    }

    directory.chdir( gkg::ConfigurationInfo::getInstance().getHomePath() +
                     "foo" );
    if ( !directory.isValid() )
    {

      directory.mkdir();

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
