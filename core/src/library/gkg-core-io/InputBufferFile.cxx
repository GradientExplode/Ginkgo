#include <gkg-core-io/InputBufferFile.h>
#include <cstdio>
#include <gkg-core-cppext/CUniStd.h>
#include <fcntl.h>
#include <sys/stat.h>


gkg::InputBufferFile::InputBufferFile()
                     : gkg::BufferFile()
{
}


gkg::InputBufferFile::InputBufferFile( const std::string& theName )
                     : gkg::BufferFile()
{

  open( theName );

}


gkg::InputBufferFile::~InputBufferFile()
{
}


void gkg::InputBufferFile::open( const std::string& theName )
{

  if ( _bufferFileInfo )
  {

    close();
    delete _bufferFileInfo;
    _bufferFileInfo = 0;

  }
  int32_t fd = ::open( theName.c_str(), O_RDONLY );
  if ( fd < 0 )
  {

    _bufferFileInfo = 0;
    return;

  }
  _bufferFileInfo = new gkg::BufferFileInfo( theName, fd );
  if ( fstat( fd, &_bufferFileInfo->info ) < 0 )
  {

    delete _bufferFileInfo;
    _bufferFileInfo = 0;

  }

}


int32_t gkg::InputBufferFile::read( const char*& start )
{

  if ( _bufferFileInfo )
  {

    int32_t length = ( int32_t )( _bufferFileInfo->info.st_size );
    if ( _bufferFileInfo->position >= length )
    {

      return 0;

    }
    if ( ( _bufferFileInfo->limit != 0 ) &&
         ( length > ( int32_t )_bufferFileInfo->limit ) )
    {

      length = ( int32_t )( _bufferFileInfo->limit );

    }
    if ( !_bufferFileInfo->buffer )
    {

      _bufferFileInfo->buffer = new char[ length ];

    }
    start = _bufferFileInfo->buffer;
    length = ::read( _bufferFileInfo->fd, _bufferFileInfo->buffer, length );
    _bufferFileInfo->position += length;
    return length;

  }
  return -1;

}
