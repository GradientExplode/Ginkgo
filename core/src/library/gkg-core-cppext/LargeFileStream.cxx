#include <gkg-core-cppext/LargeFileStream_i.h>
#include <gkg-core-exception/Exception.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>


#if ( defined(__APPLE__) && defined(__MACH__) )

// note: need some tests for the following definitions
#define lseek64        lseek
#define open64         open
#define ftruncate64    ftruncate
#define O_LARGEFILE    0

#endif


// #include <cstdio>
// #include <fcntl.h>
// #include <string>
// #include <iostream>
// 
// #undef _FILE_OFFSET_BITS
// #define _FILE_OFFSET_BITS 64
// #ifndef _LARGEFILE_SOURCE
// #define _LARGEFILE_SOURCE
// #endif
// #ifndef __USE_FILE_OFFSET64
// #define __USE_FILE_OFFSET64
// #endif
// 
// int main()
// {
//   FILE* fp = ::fopen64( "foo.bin", "wb" );
//   ::truncate64( "toto", 5000000000LL );
//   ::fseeko64( fp, 4000000000LL, SEEK_SET );
//   int buffer = 100;
//   ::fwrite( &buffer, sizeof( int ), 1, fp );
//   std::cout << ::ftello64( fp ) << std::endl;
//   ::fclose( fp );
//   return 1;
// }



//
// class BasicLargeFileIFStream
//


gkg::BasicLargeFileIFStream::BasicLargeFileIFStream() 
                            : _fileDescriptor( 0 )
{
}


gkg::BasicLargeFileIFStream::BasicLargeFileIFStream( const std::string& name )
                            : _fileDescriptor( 0 )
{

  try
  {

    open( name );

  }
  GKG_CATCH( "gkg::BasicLargeFileIFStream::BasicLargeFileIFStream( "
             "const std::string& name )" );

}


gkg::BasicLargeFileIFStream::~BasicLargeFileIFStream()
{

  close();

}


void gkg::BasicLargeFileIFStream::open( const std::string& name )
{

  try
  {

    if ( _fileDescriptor != 0 )
    {

      throw std::runtime_error( "stream is already opened" );

    }

    if ( name.empty() )
    {

      throw std::runtime_error( "file name is empty" );

    }

    _fileDescriptor = ::open( name.c_str(), O_RDONLY | O_LARGEFILE );

    if ( _fileDescriptor == -1 )
    {

      throw std::runtime_error( std::string( "cannot open '" ) + name +
                                "' file" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileIFStream::open( "
             "const std::string& name )" );     

}


bool gkg::BasicLargeFileIFStream::isOpened() const
{

  return ( _fileDescriptor != 0 ) && ( _fileDescriptor != -1 );

}


void gkg::BasicLargeFileIFStream::close()
{

  try
  {

    if ( isOpened() )
    {

      ::close( _fileDescriptor );
      _fileDescriptor = 0;

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileIFStream::close()" );

}


void gkg::BasicLargeFileIFStream::read( char* buffer, size_t count )
{

  try
  {

    if ( ::read( _fileDescriptor, buffer, count ) != ( ssize_t )count )
    {

      throw std::runtime_error( "read failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileIFStream::read( "
             "char* buffer, size_t count )" );

}


void gkg::BasicLargeFileIFStream::seek(
                                   gkg::BasicLargeFileIFStream::off_type offset,
                                   gkg::IosBase::seekdir direction )
{

  try
  {

    int whence = 0;
    if ( direction == gkg::IosBase::beg )
    {

      whence = SEEK_SET;

    }
    else if ( direction == gkg::IosBase::cur )
    {

      whence = SEEK_CUR;

    }
    else if ( direction == gkg::IosBase::end )
    {

      whence = SEEK_END;

    }
    if ( ::lseek64( _fileDescriptor, offset, whence ) == 
         ( gkg::BasicLargeFileIFStream::off_type )-1 )
    {

      throw std::runtime_error( "seek failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileIFStream::seek( "
             "gkg::BasicLargeFileIFStream::off_type offset, "
             "gkg::IosBase::seekdir direction" );

}


//
// class BasicLargeFileOFStream
//


gkg::BasicLargeFileOFStream::BasicLargeFileOFStream() 
                            : _fileDescriptor( 0 )
{
}


gkg::BasicLargeFileOFStream::BasicLargeFileOFStream( const std::string& name )
                            : _fileDescriptor( 0 )
{

  try
  {

    open( name );

  }
  GKG_CATCH( "gkg::BasicLargeFileOFStream::BasicLargeFileOFStream( "
             "const std::string& name )" );

}


gkg::BasicLargeFileOFStream::~BasicLargeFileOFStream()
{

  close();

}


void gkg::BasicLargeFileOFStream::open( const std::string& name )
{

  try
  {

    if ( _fileDescriptor != 0 )
    {

      throw std::runtime_error( "stream is already opened" );

    }

    if ( name.empty() )
    {

      throw std::runtime_error( "file name is empty" );

    }

    _fileDescriptor = ::open( name.c_str(),
                              O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE,
                              0644 );

    if ( _fileDescriptor == -1 )
    {

      throw std::runtime_error( std::string( "cannot open '" ) + name +
                                "' file" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileOFStream::open( "
             "const std::string& name )" );     

}


bool gkg::BasicLargeFileOFStream::isOpened() const
{

  return ( _fileDescriptor != 0 ) && ( _fileDescriptor != -1 );

}


void gkg::BasicLargeFileOFStream::close()
{

  try
  {

    if ( isOpened() )
    {

      ::close( _fileDescriptor );
      _fileDescriptor = 0;

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileOFStream::close()" );

}


void gkg::BasicLargeFileOFStream::write( const char* buffer, size_t count )
{

  try
  {

    if ( ::write( _fileDescriptor, buffer, count ) != ( ssize_t )count )
    {

      throw std::runtime_error( "write failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileOFStream::write( "
             "const char* buffer, size_t count )" );

}


void gkg::BasicLargeFileOFStream::seek(
                                   gkg::BasicLargeFileOFStream::off_type offset,
                                   gkg::IosBase::seekdir direction )
{

  try
  {

    int whence = 0;
    if ( direction == gkg::IosBase::beg )
    {

      whence = SEEK_SET;

    }
    else if ( direction == gkg::IosBase::cur )
    {

      whence = SEEK_CUR;

    }
    else if ( direction == gkg::IosBase::end )
    {

      whence = SEEK_END;

    }
    if ( ::lseek64( _fileDescriptor, offset, whence ) ==
         ( gkg::BasicLargeFileOFStream::off_type )-1 )
    {

      if ( ::ftruncate64( _fileDescriptor, offset ) )
      {

        throw std::runtime_error( "seek failure" );

      }

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileOFStream::seek( "
             "gkg::BasicLargeFileOFStream::off_type offset, "
             "gkg::IosBase::seekdir direction" );

}


void gkg::BasicLargeFileOFStream::truncate(
                                  gkg::BasicLargeFileOFStream::off_type length )
{

  try
  {

    if ( ::ftruncate64( _fileDescriptor, length ) )
    {

      throw std::runtime_error( "truncate failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileOFStream::truncate( "
             "gkg::BasicLargeFileOFStream::off_type length" );

}


//
// class BasicLargeFileFStream
//


gkg::BasicLargeFileFStream::BasicLargeFileFStream() 
                           : _fileDescriptor( 0 )
{
}


gkg::BasicLargeFileFStream::BasicLargeFileFStream( const std::string& name )
                           : _fileDescriptor( 0 )
{

  try
  {

    open( name );

  }
  GKG_CATCH( "gkg::BasicLargeFileFStream::BasicLargeFileFStream( "
             "const std::string& name )" );

}


gkg::BasicLargeFileFStream::~BasicLargeFileFStream()
{

  close();

}


void gkg::BasicLargeFileFStream::open( const std::string& name )
{

  try
  {

    if ( _fileDescriptor != 0 )
    {

      throw std::runtime_error( "stream is already opened" );

    }

    if ( name.empty() )
    {

      throw std::runtime_error( "file name is empty" );

    }

    _fileDescriptor = ::open( name.c_str(),
                              O_RDWR | O_CREAT | O_LARGEFILE, 0644 );

    if ( _fileDescriptor == -1 )
    {

      throw std::runtime_error( std::string( "cannot open '" ) + name +
                                "' file" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileFStream::open( "
             "const std::string& name )" );     

}


bool gkg::BasicLargeFileFStream::isOpened() const
{

  return ( _fileDescriptor != 0 ) && ( _fileDescriptor != -1 );

}


void gkg::BasicLargeFileFStream::close()
{

  try
  {

    if ( isOpened() )
    {

      ::close( _fileDescriptor );
      _fileDescriptor = 0;

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileFStream::close()" );

}


void gkg::BasicLargeFileFStream::read( char* buffer, size_t count )
{

  try
  {

    if ( ::read( _fileDescriptor, buffer, count ) != ( ssize_t )count )
    {

      throw std::runtime_error( "read failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileFStream::read( "
             "char* buffer, size_t count )" );

}


void gkg::BasicLargeFileFStream::write( const char* buffer, size_t count )
{

  try
  {

    if ( ::write( _fileDescriptor, buffer, count ) != ( ssize_t )count )
    {

      throw std::runtime_error( "write failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileFStream::write( "
             "const char* buffer, size_t count )" );

}


void gkg::BasicLargeFileFStream::seek(
                                    gkg::BasicLargeFileFStream::off_type offset,
                                    gkg::IosBase::seekdir direction )
{

  try
  {

    int whence = 0;
    if ( direction == gkg::IosBase::beg )
    {

      whence = SEEK_SET;

    }
    else if ( direction == gkg::IosBase::cur )
    {

      whence = SEEK_CUR;

    }
    else if ( direction == gkg::IosBase::end )
    {

      whence = SEEK_END;

    }
    if ( ::lseek64( _fileDescriptor, offset, whence ) ==
         ( gkg::BasicLargeFileFStream::off_type )-1 )
    {

      if ( ::ftruncate64( _fileDescriptor, offset ) )
      {

        throw std::runtime_error( "seek failure" );

      }

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileFStream::seek( "
             "gkg::BasicLargeFileFStream::off_type offset, "
             "gkg::IosBase::seekdir direction" );

}


void gkg::BasicLargeFileFStream::truncate(
                                   gkg::BasicLargeFileFStream::off_type length )
{

  try
  {

    if ( ::ftruncate64( _fileDescriptor, length ) )
    {

      throw std::runtime_error( "truncate failure" );

    }

  }
  GKG_CATCH( "void gkg::BasicLargeFileFStream::truncate( "
             "gkg::BasicLargeFileFStream::off_type length" );

}


