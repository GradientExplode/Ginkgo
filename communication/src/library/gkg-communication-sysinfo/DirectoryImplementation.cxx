#include <gkg-communication-sysinfo/DirectoryImplementation.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <cstdio>
#include <cstring>


gkg::DirectoryImplementation::DirectoryImplementation( 
                                                      gkg::Directory* directory,
                                                      const std::string& name )
                             : _target( directory ),
                               _name( name )
{

  if ( ( name.length() > 1U ) && 
       ( name.substr( name.length() - 1U, 1 ) ==
         gkg::getDirectorySeparator() ) )
  {

    _name = name.substr( 0U, name.length() - 1U );

  }
  else
  {

    _name = name;

  }

}


gkg::DirectoryImplementation::~DirectoryImplementation()
{
}


gkg::Directory* gkg::DirectoryImplementation::getTarget() const
{

  return _target;

}


const std::string& gkg::DirectoryImplementation::getPath() const
{

  return _name;

}
