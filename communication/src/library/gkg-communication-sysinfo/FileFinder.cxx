#include <gkg-communication-sysinfo/FileFinder.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-string/StringSplitter.h>
#include <iostream>
#include <queue>


gkg::FileFinder::FileFinder() 
{
}


gkg::FileFinder::~FileFinder()
{
}


bool gkg::FileFinder::locateFromPath( const std::string& name )
{

  gkg::File fic( name );

  if ( fic.isExisting() )
  {

    _fullPath = name;
    return true;

  }

  std::string path = gkg::ConfigurationInfo::getInstance().getEnvPath();
  if ( path.empty() )
  {

    std::cerr << "PATH variable is not set" << std::endl;
    return false;

  }

  gkg::StringSplitter splitter( path, gkg::getEnvironmentPathSeparator() );

  gkg::StringSplitter::const_iterator i = splitter.begin(), 
                                      ie = splitter.end();

  while ( i != ie )
  {

    std::string tmpName = *i + gkg::getDirectorySeparator() + name;
    gkg::File tmpFile( tmpName );

    if ( tmpFile.isExisting() )
    {

      _fullPath = tmpName;
      return true;

    }

    ++ i;

  }

  return false;

}
	

bool gkg::FileFinder::locateFromDirectory( const std::string& name,
                                           const std::string& directory )
{

  std::queue< std::string > dirs;

  dirs.push( directory );

  while ( ! dirs.empty() )
  {

    std::string dirElt = dirs.front();
    std::string tmpName = dirElt + gkg::getDirectorySeparator() + name;
    gkg::File tmpFile( tmpName ); 

    if ( tmpFile.isExisting() )
    {

      _fullPath = tmpName;
      return true;

    }

    gkg::Directory tmpDir( dirElt );

    if ( tmpDir.isValid() )
    {

      std::set< std::string > directories = tmpDir.getSubDirectories();
      std::set< std::string >::const_iterator d = directories.begin(),
                                              de = directories.end();

      while ( d != de )
      {

        tmpName = dirElt + gkg::getDirectorySeparator() + *d;
        dirs.push( tmpName );
        ++ d;

      }

    }

    dirs.pop();

  }

  return false;

}


const std::string& gkg::FileFinder::getFullPath() const
{

  return _fullPath;

}
