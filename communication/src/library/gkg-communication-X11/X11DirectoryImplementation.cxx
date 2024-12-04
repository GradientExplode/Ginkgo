#include <gkg-communication-X11/X11DirectoryImplementation.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-cppext/CUniStd.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>


// these hide in mysterious places on various systems; for now, it seems safest
// just to declare them explicitly.

extern "C"
{

  extern uid_t getuid();
#ifdef __DECCXX

  extern struct passwd* getpwent();
  extern struct passwd* getpwnam( const char* );
  extern struct passwd* getpwuid( uid_t );

#endif

}


#ifndef S_ISDIR
#define S_ISDIR( mode ) ( ( ( mode ) & S_IFMT ) == S_IFDIR )
#endif


//
// return system-dependant canonical directory form
//

std::string gkg::getCanonicalDirectory( const std::string& path )
{

  return gkg::X11DirectoryImplementation::getCanonical( path );

}


//
// return system-dependant directory separator
//

std::string gkg::getDirectorySeparator()
{

  return "/";

}


//
// return current directory
//

std::string gkg::getCurrentDirectory()
{

  char buffer[ 1024U ];
  char* currentDirectory = getcwd( buffer, 1024U );
  
  if ( !currentDirectory )
  {
  
    return std::string( "." );
  
  }
  
  return std::string( buffer );

}


//
// struct DirectoryImplementation::Entry
//

bool gkg::X11DirectoryImplementation::Entry::operator < ( 
                     const gkg::X11DirectoryImplementation::Entry& other ) const
{

  return ( name.compare( other.name ) < 0 );

}


//
// class X11DirectoryImplementation
//


gkg::X11DirectoryImplementation::X11DirectoryImplementation(
                                                      gkg::Directory* directory,
                                                      const std::string& name )
                                : DirectoryImplementation( directory,
                        gkg::X11DirectoryImplementation::getCanonical( name ) ),
                                  _filled( false )
{

  try
  {

    // cast is to workaround bug in some opendir prototypes
    _dir = opendir( ( char* )_name.c_str() );

  }
  GKG_CATCH( "gkg::X11DirectoryImplementation::X11DirectoryImplementation( "
             "gkg::Directory* directory, "
             "const std::string& name )" );

}



gkg::X11DirectoryImplementation::~X11DirectoryImplementation()
{

  doClear();

}


std::string gkg::X11DirectoryImplementation::getName( int32_t index )
{

  try
  {

    if ( !_filled )
    {

      doFill();

    }
    if ( ( index < 0 ) || ( index >= int32_t( _entries.size() ) ) )
    {

      throw std::runtime_error( "index out of range" );
      return "";

    }
    return _entries[ index ].name;

  }
  GKG_CATCH( "std::string gkg::X11DirectoryImplementation::getName( "
             "int index )" );

}


int32_t gkg::X11DirectoryImplementation::getIndex( const std::string& name )
{

  try
  {

    if ( !_filled )
    {

      doFill();

    }

    int32_t i = 0, j = int32_t( _entries.size() ) - 1;
    while ( i <= j )
    {

      int32_t k = ( i + j ) / 2;
      int32_t cmp = name.compare( _entries[ k ].name );
      if ( cmp == 0 )
      {

        return k;

      }
      if ( cmp > 0 )
      {

        i = k + 1;

      }
      else
      {

        j = k - 1;

      }

    }
    return -1;

  }
  GKG_CATCH( "int gkg::X11DirectoryImplementation::getIndex( "
             "const std::string& name )" );

}


int32_t gkg::X11DirectoryImplementation::getCount()
{

  try
  {

    if ( !_filled )
    {

      doFill();

    }
    return int32_t( _entries.size() );

  }
  GKG_CATCH( "int gkg::X11DirectoryImplementation::getCount()" );

}


void gkg::X11DirectoryImplementation::chdir( const std::string& name )
{

  try
  {

    // first, we remove the existing tree
    doClear();

    // creating canonical directory name
    _name = gkg::X11DirectoryImplementation::getCanonical( name );

    // opening the directory
    _dir = opendir( ( char* )_name.c_str() );

  }
  GKG_CATCH( "void gkg::X11DirectoryImplementation::chdir( "
             "const std::string& name )" );

}


void gkg::X11DirectoryImplementation::mkdir() const
{

  try
  {

    if ( !_dir )
    {

      if ( ::mkdir( _name.c_str(),
                    S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) )
      {

        throw std::runtime_error( std::string( "couldn't create directory" ) +
                                  _name );

      }

    }

  }
  GKG_CATCH( "void gkg::X11DirectoryImplementation::mkdir() const" );

}


void gkg::X11DirectoryImplementation::remove()
{

  try
  {

    if ( _dir )
    {

      if ( ::remove( _name.c_str() ) )
      {

        throw std::runtime_error( std::string( "couldn't remove directory" ) +
                                  _name );

      }

    }

  }
  GKG_CATCH( "void gkg::X11DirectoryImplementation::remove()" );

}


bool gkg::X11DirectoryImplementation::isValid() const
{

  try
  {

    return _dir != 0;

  }
  GKG_CATCH( "bool gkg::X11DirectoryImplementation::isValid() const" );

}


bool gkg::X11DirectoryImplementation::isDirectory( int32_t index )
{

  try
  {

    if ( !_filled )
    {

      doFill();

    }

    if ( ( index < 0 ) || ( index >= int32_t( _entries.size() ) ) )
    {

      throw std::runtime_error( "index out of range" );
      return false;

    }
    gkg::X11DirectoryImplementation::Entry& entry = _entries[ index ];
    if ( !entry.info.st_size )
    {

      std::string tmp = _name;
      tmp += "/";
      tmp += entry.name;
      stat( tmp.c_str(), &entry.info );

    }
    return S_ISDIR( entry.info.st_mode );

  }
  GKG_CATCH( "bool gkg::X11DirectoryImplementation::isDirectory( int index )" );

}


std::string 
gkg::X11DirectoryImplementation::getCanonical( const std::string& name )
{

  try
  {

    std::string path;

    path = interpretSlashSlash( name );
    path = eliminateDot( path );
    path = eliminateDotDot( path );
    path = interpretTilde( path );

    if ( path[ 0 ] == '\0' )
    {

      path = "./";

    }
    else if ( !hasDotSlash( path, 0 ) &&
              !hasDotDotSlash( path, 0 ) &&
              ( path[ 0 ] != '/' ) )
    {

      path = ( std::string )"./" + path;

    }
    else if ( ifDirectory( path ) && ( path[ path.length() - 1 ] != '/' ) )
    {

      path += "/";

    }

    if ( path[ path.length() - 1 ] != '/' )
    {

      path += "/";

    }
    return path;

  }
  GKG_CATCH( "std::string gkg::X11DirectoryImplementation::getCanonical( "
             "const std::string& name )" );

}


bool gkg::X11DirectoryImplementation::hasDotSlash( const std::string& path,
                                                   int32_t position )
{

  return ( path[ 0 + position ] == '.' ) &&
         ( ( path[ 1 + position ] == '/' ) ||
           ( path[ 1 + position ] == '\0' ) );

}

bool 
gkg::X11DirectoryImplementation::hasDotDotSlash( const std::string& path,
                                                 int32_t position )
{

    return ( path[ 0 + position ] == '.' ) &&
           ( path[ 1 + position ] == '.' ) &&
           ( ( path[ 2 + position ] == '/' ) || 
             ( ( position + 2 ) == ( int32_t )path.length() ) );

}


std::string 
gkg::X11DirectoryImplementation::getHome( const std::string& name )
{

  struct passwd* pw;
  if ( name.empty() )
  {

    pw = getpwuid( getuid() );

  }
  else
  {

    pw = getpwnam( name.c_str() );

  }
  return pw ? std::string( pw->pw_dir ) : "";

}


std::string 
gkg::X11DirectoryImplementation::eliminateDot( const std::string& path )
{

  std::string newPath;
  int32_t destination = 0, len = path.length();
  for ( int32_t position = 0; position < len; position++ )
  {

    if ( hasDotSlash( path, position ) &&
         ( destination > 0 ) &&
         ( newPath[ destination - 1 ] == '/' ) )
    {

      ++ position;

    }
    else
    {

      newPath += path[ position ];
      ++ destination;

    }

  }
  return newPath;

}


bool gkg::X11DirectoryImplementation::collapsedDotDotSlash(
                                                        const std::string& path,
                                                        int32_t& start )
{


  if ( ( start == 0 ) || ( path[ start - 1 ] != '/' ) )
  {

    return false;

  }

  if ( ( start == 1 ) && ( path[ 0 ] == '/' ) )
  {

    return true;

  }

  if ( start == 2 )
  {

    // doesn't handle double-slash correctly
    start = 0;
   return path[ 0 ] != '.';

  }

  if ( ( start > 2 ) && !hasDotDotSlash( path, start - 3 ) )
  {

    for ( start -= 2; start >= 0; start-- )
    {

      if ( path[ start ] == '/' )
      {

        ++ start;
        return true;

      }
	
    }
    start = 0;
    return true;

  }
  return false;

}


std::string 
gkg::X11DirectoryImplementation::eliminateDotDot( const std::string& path )
{

  std::string newPath;
  int32_t destination = 0, len = path.length();
  for ( int32_t position = 0; position < len; position++ )
  {

    if ( hasDotDotSlash( path, position ) &&
         collapsedDotDotSlash( path, destination ) )
    {

      newPath = newPath.substr( 0, destination );
      position += 2;

    }
    else
    {

      newPath += path[ position ];
      ++ destination;

    }

  }
  return newPath;

}


std::string 
gkg::X11DirectoryImplementation::interpretSlashSlash( const std::string& path )
{

  for ( int32_t position = path.length() - 1; position > 0; position-- )
  {

    if ( ( path[ position ] == '/' ) && ( path[ position - 1 ] == '/' ) )
    {

      return path.substr( position, path.length() - position );

    }

  }
  return path;

}


std::string 
gkg::X11DirectoryImplementation::interpretTilde( const std::string& path )
{

  std::string realPath;

  size_t beg = path.rfind( '~' );
  bool valid = ( ( beg != std::string::npos ) &&
                 ( ( beg == 0 ) || ( path[ beg - 1 ] == '/' ) ) );
  if ( valid )
  {

    size_t end = path.find( '/', beg );
    size_t length = ( end == std::string::npos ) ? path.length() - beg :
                                                   ( end - beg );
    std::string subPath = path.substr( beg, length );

    std::string expanded = expandTilde( subPath );
    if ( expanded.empty() )
    {

      valid = false;

    }
    else
    {

      realPath = expanded;
      if ( end != std::string::npos )
      {

        realPath += path.substr( end, path.length() - end );

      }

    }

  }
  return valid ? realPath : path;

}


std::string 
gkg::X11DirectoryImplementation::expandTilde( const std::string& tilde )
{

  std::string name;

  if ( tilde.length() > 1 )
  {

    name = tilde.substr( 1, tilde.length() - 1 );

  }
  return getHome( name );

}


std::string gkg::X11DirectoryImplementation::realPath( const std::string& path )
{

  if ( path[ 0 ] == '\0' )
  {

    return "./";

  }
  return interpretTilde( interpretSlashSlash( path ) );

}


bool gkg::X11DirectoryImplementation::ifDirectory( const std::string& path )
{

  struct stat st;
  return ( stat( path.c_str(), &st ) == 0 ) && S_ISDIR( st.st_mode );

}


gkg::X11DirectoryImplementation* 
gkg::X11DirectoryImplementation::getImplementation( gkg::Directory* directory )
{

  return static_cast< gkg::X11DirectoryImplementation* >(
                                               directory->getImplementation() );

}


void gkg::X11DirectoryImplementation::doFill()
{

  try
  {

#ifdef apollo

    for ( struct direct* d = readdir( _dir ); d != 0; d = readdir( _dir ) )

#else

    for ( struct dirent* d = readdir( _dir ); d != 0; d = readdir( _dir ) )

#endif
    {

      Entry entry;
      entry.name = std::string( d->d_name );
      entry.info.st_size = 0;
      _entries.push_back( entry );

    }

    std::sort( _entries.begin(), _entries.end() );
    _filled = true;

  }
  GKG_CATCH( "void gkg::X11DirectoryImplementation::doFill()" );

}


void gkg::X11DirectoryImplementation::doClear()
{

  try
  {

    if ( _dir )
    {

      closedir( _dir );

    }

    _entries.clear();
    _filled = false;

  }
  GKG_CATCH( "void gkg::X11DirectoryImplementation::doClear()" );

}
