#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/DirectoryImplementation.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>


gkg::Directory::Directory( const std::string& name )
{

  try
  {

    _directoryImplementation =
      gkg::getCommunicationImplementationFactory().
                                          createDirectoryImplementation( this,
                                                                         name );

  }
  GKG_CATCH( "gkg::Directory::Directory( const std::string& name )" );

}


gkg::Directory::~Directory()
{

  delete _directoryImplementation;

}


bool gkg::Directory::isValid() const
{

  try
  {

    return _directoryImplementation->isValid();

  }
  GKG_CATCH( "bool gkg::Directory::isValid() const" );

}


bool gkg::Directory::isDirectory( int index ) const
{

  try
  {

    return _directoryImplementation->isDirectory( index );

  }
  GKG_CATCH( "bool gkg::Directory::isDirectory( int index ) const" );

}


const std::string& gkg::Directory::getPath() const
{

  try
  {

    return _directoryImplementation->getPath();

  }
  GKG_CATCH( "const std::string& gkg::Directory::getPath() const" );

}


int32_t gkg::Directory::getCount() const
{

  try
  {

    return _directoryImplementation->getCount();

  }
  GKG_CATCH( "int gkg::Directory::getCount() const" );

}


std::string gkg::Directory::getName( int32_t index ) const
{

  try
  {

    return _directoryImplementation->getName( index );

  }
  GKG_CATCH( "std::string gkg::Directory::getName( int index ) const" );

}


int32_t gkg::Directory::getIndex( const std::string& name ) const
{

  try
  {

    return _directoryImplementation->getIndex( name ); 

  }
  GKG_CATCH( "int gkg::Directory::getIndex( const std::string& name ) const" );

}


std::set< std::string > gkg::Directory::getFiles() const
{

  try
  {

    std::set< std::string > files;

    int32_t index;
    for ( index = 0; index < getCount(); index++ )
    {

      if ( !isDirectory( index ) )
      {

        files.insert( getName( index ) );

      }

    }
    return files;

  }
  GKG_CATCH( "std::set< std::string > gkg::Directory::getFiles() const" );

}


// Pattern must be of the form  */*/*.ext ou ../../*/*.ext
std::set< std::string > gkg::Directory::getMatchingFiles(
                                              const std::string& pattern ) const
{

  try
  {

    std::set< std::string > files;

    if ( !pattern.empty() )
    {

      int32_t index;
      std::string wildcard = std::string( "*" ) + gkg::getDirectorySeparator();
      std::string upperDir = std::string( ".." ) + gkg::getDirectorySeparator();

      if ( !pattern.compare( 0, 2, wildcard ) )
      {

        std::string subPattern = pattern.substr( 2 );
        std::string baseDir = getPath() + gkg::getDirectorySeparator();
        std::set< std::string > subDirs = getSubDirectories();
        std::set< std::string >::iterator
          d = subDirs.begin(),
          de = subDirs.end();

        while ( d != de )
        {

          if ( ( *d != "." ) && ( *d != ".." ) )
          {

            gkg::Directory subDirectory( baseDir + *d );
            std::set< std::string > fileList = subDirectory.getMatchingFiles( 
                                                                   subPattern );
            std::set< std::string >::iterator
              f = fileList.begin(),
              fe = fileList.end();

            while ( f != fe )
            {

              files.insert( *d + gkg::getDirectorySeparator() + *f );

              ++f;

            }

          }

          ++d;

        }

      }
      else if ( !pattern.compare( 0, 3, upperDir ) )
      {

        std::string subPattern = pattern.substr( 3 );
        gkg::Directory upperDirectory( upperDir );
        std::set< std::string > fileList = upperDirectory.getMatchingFiles( 
                                                                   subPattern );
        std::set< std::string >::iterator
          f = fileList.begin(),
          fe = fileList.end();

        while ( f != fe )
        {

          files.insert( upperDir + *f );

          ++f;

        }

      }
      else
      {

        for ( index = 0; index < getCount(); index++ )
        {

          if ( !isDirectory( index ) )
          {

            std::string fileName = getName( index );

            if ( match( fileName, pattern ) )
            {

              files.insert( fileName );

            }

          }

        }

      }
  
    }
    return files;

  }
  GKG_CATCH( "std::set< std::string > gkg::Directory::getMatchingFiles( "
             "const std::string& pattern ) const" );

}


std::set< std::string > gkg::Directory::getSubDirectories(
                      const std::set< std::string >* unwantedDirectories ) const
{

  try
  {

    std::set< std::string > directories;

    int32_t index;
    std::string directoryName;
    for ( index = 0; index < getCount(); index++ )
    {

      if ( isDirectory( index ) )
      {

        directoryName = getName( index );
        if ( unwantedDirectories )
        {

          if ( unwantedDirectories->find( directoryName ) == 
               unwantedDirectories->end() )
          {

            directories.insert( directoryName );

          }

        }
        else
        {

          directories.insert( directoryName );

        }

      }

    }
    return directories;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::Directory::getSubDirectories() const" );

}


void gkg::Directory::chdir( const std::string& name )
{

  try
  {

    _directoryImplementation->chdir( name );

  }
  GKG_CATCH( "void gkg::Directory::chdir( const std::string& name )" );

}


void gkg::Directory::mkdir() const
{

  try
  {

    _directoryImplementation->mkdir();

  }
  GKG_CATCH( "void gkg::Directory::mkdir() const" );

}


void gkg::Directory::remove()
{

  try
  {

    // removing sub-directories
    std::set< std::string >
      subDirectories = gkg::Directory::getSubDirectories();
    std::set< std::string >::const_iterator s = subDirectories.begin(),
                                            se = subDirectories.end();
    while ( s != se )
    {

      if ( ( *s != "." ) && ( *s != ".." ) )
      {
      
        gkg::Directory subDirectory( getPath() +
                                     gkg::getDirectorySeparator() +
                                     *s );
        subDirectory.remove();
	
      }
      ++ s;

    }

    // removing files
    std::set< std::string >
      files = gkg::Directory::getFiles();
    std::set< std::string >::const_iterator f = files.begin(),
                                            fe = files.end();
    while ( f != fe )
    {

      gkg::File file( getPath() +
                      gkg::getDirectorySeparator() +
                      *f );
      file.remove();
      ++ f;

    }

    _directoryImplementation->remove();

    std::string name = _directoryImplementation->getPath();
    delete _directoryImplementation;

    _directoryImplementation =
      gkg::getCommunicationImplementationFactory().
                                    createDirectoryImplementation( this, name );

  }
  GKG_CATCH( "void gkg::Directory::remove()" );

}


bool gkg::Directory::match( const std::string& name,
                            const std::string& pattern )
{

  const char* s = name.c_str();
  const char* endS = s + name.length();
  const char* p = pattern.c_str();
  const char* endP = p + pattern.length();
  for ( ; p < endP; p++, s++ )
  {

    if ( *p == '*' )
    {

      const char* pp = p + 1;
      if ( pp == endP )
      {

        return true;

      }
      for ( ; s < endS && *s != *pp; s++ ) ;
      p = pp;

    }
    else if ( ( s >= endS ) || ( *p != *s ) )
    {

      return false;

    }

  }
  return s == endS;

}


gkg::DirectoryImplementation* gkg::Directory::getImplementation() const
{

  return _directoryImplementation;

}
