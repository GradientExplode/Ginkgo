#include <gkg-graphics-video-avi/AviMemory.h>
#include <gkg-core-exception/Exception.h>

#include <cstring>
#include <cstdio>


gkg::AviMemory::AviMemory()
              : gkg::AviStrategy(),
	        _buffer( 0 )
{
}


gkg::AviMemory::~AviMemory()
{

  delete[] _buffer;

}


void gkg::AviMemory::initialize( int32_t width, 
                                 int32_t height, 
				 int32_t bitsPerPixel,
				 int32_t nFrames )
{

  try
  {
  
    uint32_t nBytes = bitsPerPixel >> 3;
    uint32_t mem_size = nBytes * width * height * nFrames + 6144;
  
    _buffer = new char[ mem_size ];

  }
  GKG_CATCH( "void gkg::AviMemory::initialize( int32_t width, "
             "int32_t height, int32_t nFrames )" );

}

   		 
off_t gkg::AviMemory::lseek( off_t offset, int32_t whence )
{

  try
  {

    if ( _buffer )
    {

      switch ( whence )
      {

    	case SEEK_SET:
    	  _pos = offset;
    	  break;

    	case SEEK_CUR:
    	  _pos += offset;
    	  break;

    	case SEEK_END:
    	  _pos = _used;
    	  break;

      }

      return _pos;
  
    }
  
    return 0;
  
  }
  GKG_CATCH( "off_t gkg::AviMemory::lseek( off_t offset, int32_t whence )" );

}


void gkg::AviMemory::write( std::string fileName )
{

  try
  {

    if ( !fileName.empty() )
    {
  
      FILE* file = fopen( fileName.c_str(), "wb" );
      
      if ( file )
      {
      
    	fwrite( _buffer, sizeof( char ), _used, file );
    	fclose( file );
      
      }
  
    }
  
  }
  GKG_CATCH( "void gkg::AviMemory::write( std::string fileName )" );

}


ssize_t gkg::AviMemory::write( const void* buffer, uint32_t count )
{

  try
  {

    if ( _buffer )
    {
  
      memcpy( _buffer + _pos, buffer, count );
      
      _pos += count;

      if ( _pos > _used )
      {

    	_used = _pos;

      }

      return count;
      
    }
  
    return 0;

  }
  GKG_CATCH( "ssize_t gkg::AviMemory::write( "
             "const void* buffer, uint32_t count )" );

}


void gkg::AviMemory::write_le32( int32_t value )
{

  try
  {
  
    if ( _buffer )
    {

      _buffer[ _pos++ ] = value & 0xFF;
      value >>= 8;
      _buffer[ _pos++ ] = value & 0xFF;
      value >>= 8;
      _buffer[ _pos++ ] = value & 0xFF;
      value >>= 8;
      _buffer[ _pos++ ] = value & 0xFF;

      if ( _pos > _used )
      {

    	_used = _pos;

      }
  
    }
  
  }
  GKG_CATCH( "void gkg::AviMemory::write_le32( int32_t value )" );

}


char* gkg::AviMemory::getBuffer()
{

  return _buffer;

}
