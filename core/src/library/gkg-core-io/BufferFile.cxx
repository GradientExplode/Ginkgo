#include <gkg-core-io/BufferFile.h>
#include <cstdio>
#include <gkg-core-cppext/CUniStd.h>


//
// struct BufferFileInfo
//

gkg::BufferFileInfo::BufferFileInfo( const std::string& theName, int32_t theFd )
                    : name( theName ),
                      fd( theFd ),
                      position( 0 ),
                      buffer( 0 ),
                      limit( 0 )
{
}


//
// class File
//


gkg::BufferFile::BufferFile()
                : _bufferFileInfo( 0 )
{
}


gkg::BufferFile::~BufferFile()
{

  close();
  if ( _bufferFileInfo )
  {

    delete _bufferFileInfo;

  }

}


const std::string& gkg::BufferFile::getName() const
{

  return _bufferFileInfo->name;

}


int64_t gkg::BufferFile::getLength() const
{

  return _bufferFileInfo->info.st_size;

}


void gkg::BufferFile::close()
{

  if ( _bufferFileInfo->fd >= 0 )
  {

    if ( _bufferFileInfo->buffer )
    {

      delete [] _bufferFileInfo->buffer;

    }
    ::close( _bufferFileInfo->fd );
    _bufferFileInfo->fd = -1;

  }

}


void gkg::BufferFile::setLimit( uint32_t bufferSize )
{

  _bufferFileInfo->limit = bufferSize;

}


gkg::BufferFile::operator bool()
{

  return _bufferFileInfo != 0;

}
