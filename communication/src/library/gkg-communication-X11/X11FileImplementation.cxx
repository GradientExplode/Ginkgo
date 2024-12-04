#include <gkg-communication-X11/X11FileImplementation.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-core-cppext/CUniStd.h>
#include <cstdio>


gkg::X11FileImplementation::X11FileImplementation( gkg::File* file,
                                                   const std::string& name )
                           : gkg::FileImplementation( file, name )
{

  update();

}


gkg::X11FileImplementation::~X11FileImplementation()
{
}


std::string gkg::X11FileImplementation::getBaseName() const
{

  std::string separator = gkg::getDirectorySeparator();
  std::string::size_type position = _name.rfind( separator );

  if ( position == std::string::npos )
  {

    if ( _name == "." || _name == ".." )
    {

      return "";

    }
    else
    {

      position = 0U;

    }

  }
  else
  {

    ++ position;

  }
  return _name.substr( position, _name.length() - position ); 

}


std::string gkg::X11FileImplementation::getDirectoryName() const
{

  std::string separator = gkg::getDirectorySeparator();
  std::string::size_type position = _name.rfind( separator );

  if ( position == std::string::npos )
  {

    if ( _name == ".." )
    {

      return std::string( ".." ) + separator;

    }
    else
    {

      return std::string( "." ) + separator;

    }

  }
  if ( position == 0U )
  {

    return separator;

  }
  return _name.substr( 0U, position ) + separator;

}


uint64_t gkg::X11FileImplementation::getSize() const
{

  uint64_t size = 0U;
  if ( _isExisting )
  {

    size = ( uint64_t )_statistics.st_size;

  }
  return size;

}


time_t gkg::X11FileImplementation::getLastAccessTime() const
{

  time_t accessTime = 0;

  if ( _isExisting )
  {

    accessTime = _statistics.st_atime;

  }

  return accessTime;

}


time_t gkg::X11FileImplementation::getLastModificationTime() const
{

  time_t modificationTime = 0;

  if ( _isExisting )
  {

    modificationTime = _statistics.st_mtime;

  }

  return modificationTime;

}


time_t gkg::X11FileImplementation::getLastStateChangeTime() const
{

  time_t stateChangeTime = 0;

  if ( _isExisting )
  {

    stateChangeTime = _statistics.st_ctime;

  }

  return stateChangeTime;

}


bool gkg::X11FileImplementation::isExisting() const
{

  return _isExisting;

}


bool gkg::X11FileImplementation::isRegular() const
{

  return S_ISREG( _statistics.st_mode );

}


bool gkg::X11FileImplementation::isDirectory() const
{

  return S_ISDIR( _statistics.st_mode );

}


bool gkg::X11FileImplementation::isLink() const
{

  return S_ISLNK( _statistics.st_mode );

}


bool gkg::X11FileImplementation::isCharacterDevice() const
{

  return S_ISCHR( _statistics.st_mode );

}


bool gkg::X11FileImplementation::isBlockDevice() const
{

  return S_ISBLK( _statistics.st_mode );

}


bool gkg::X11FileImplementation::isReadable() const
{

  bool readable = false;

  uid_t uid = getuid();
  gid_t gid = getgid();

  // if I am the owner
  if ( _statistics.st_uid == uid )
  {

    readable = this->isUserReadable();

  }
  // else if I belong to the group
  else if ( _statistics.st_gid == gid )
  {

    readable = this->isGroupReadable();

  }
  // else if I am neither the owner nor belongs to the group
  else
  {

    readable = this->isOtherReadable();

  }
  return readable;

}


bool gkg::X11FileImplementation::isWritable() const
{

  bool writable = false;

  uid_t uid = getuid();
  gid_t gid = getgid();

  // if I am the owner
  if ( _statistics.st_uid == uid )
  {

    writable = this->isUserWritable();

  }
  // else if I belong to the group
  else if ( _statistics.st_gid == gid )
  {

    writable = this->isGroupWritable();

  }
  // else if I am neither the owner nor belongs to the group
  else
  {

    writable = this->isOtherWritable();

  }
  return writable;

}


bool gkg::X11FileImplementation::isExecutable() const
{

  bool executable = false;

  uid_t uid = getuid();
  gid_t gid = getgid();

  // if I am the owner
  if ( _statistics.st_uid == uid )
  {

    executable = this->isUserExecutable();

  }
  // else if I belong to the group
  else if ( _statistics.st_gid == gid )
  {

    executable = this->isGroupExecutable();

  }
  // else if I am neither the owner nor belongs to the group
  else
  {

    executable = this->isOtherExecutable();

  }
  return executable;

}


bool gkg::X11FileImplementation::isUserReadable() const
{

  return _statistics.st_mode & S_IRUSR;

}


bool gkg::X11FileImplementation::isUserWritable() const
{

  return _statistics.st_mode & S_IWUSR;

}


bool gkg::X11FileImplementation::isUserExecutable() const
{

  return _statistics.st_mode & S_IXUSR;

}


bool gkg::X11FileImplementation::isGroupReadable() const
{

  return _statistics.st_mode & S_IRGRP;

}


bool gkg::X11FileImplementation::isGroupWritable() const
{

  return _statistics.st_mode & S_IWGRP;

}


bool gkg::X11FileImplementation::isGroupExecutable() const
{

  return _statistics.st_mode & S_IXGRP;

}


bool gkg::X11FileImplementation::isOtherReadable() const
{

  return _statistics.st_mode & S_IROTH;

}


bool gkg::X11FileImplementation::isOtherWritable() const
{

  return _statistics.st_mode & S_IWOTH;

}


bool gkg::X11FileImplementation::isOtherExecutable() const
{

  return _statistics.st_mode & S_IXOTH;

}


void gkg::X11FileImplementation::remove()
{

  if ( _isExisting && isRegular() && !isDirectory() && isUserWritable() )
  {
  
    ::remove( _name.c_str() );
    update();
  
  }

}


void gkg::X11FileImplementation::update()
{

  _isExisting = ( stat( _name.c_str(), &_statistics ) == 0 );


}


gkg::X11FileImplementation*
gkg::X11FileImplementation::getImplementation( gkg::File* file )
{

  return static_cast< gkg::X11FileImplementation* >(
                                                    file->getImplementation() );

}


