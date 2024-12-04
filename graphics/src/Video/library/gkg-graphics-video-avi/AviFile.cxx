#include <gkg-graphics-video-avi/AviFile.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-cppext/CUniStd.h>

#include <string>
#include <cstdlib>


gkg::AviFile::AviFile()
            : _file( 0 ),
	      _tempName( 0 )
{

  try
  {

#if ( defined(__APPLE__) && defined(__MACH__) )

    // Note: This modification causes errors while running GkgTestAvi01

    char directory[ FILENAME_MAX ];
    if ( !getcwd( directory, sizeof( directory ) ) )
    {

      throw std::runtime_error( "fail to get directory" );

    }
    _tempName = tempnam( directory, "avi" );
    _file = fopen( _tempName, "wb" );
    free( directory );

#else

    std::string tmpFileStr = std::string( get_current_dir_name() ) +
                             gkg::getDirectorySeparator() +
                             "aviXXXXXX";
    _tempName = const_cast< char* >( tmpFileStr.c_str() );
    _file = fdopen( mkstemp( _tempName ), "wb" );

#endif
    
  }
  GKG_CATCH( "gkg::AviFile::AviFile()" );

}


gkg::AviFile::~AviFile()
{

  close();
  free( _tempName );

}


void gkg::AviFile::initialize( int32_t /* width */, 
                               int32_t /* height */, 
			       int32_t /* bitsPerPixel */,
			       int32_t /* nFrames */ )
{
}

    		 
off_t gkg::AviFile::lseek( off_t offset, int32_t whence )
{

  try
  {

    if ( _file )
    {

      fseek( _file, offset, whence );

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
  GKG_CATCH( "off_t gkg::AviFile::lseek( off_t offset, int32_t whence )" );

}


void gkg::AviFile::write( std::string fileName )
{

  try
  {
  
    close();
    rename( _tempName, fileName.c_str() );

  }
  GKG_CATCH( "void gkg::AviFile::write( std::string fileName )" )

}


ssize_t gkg::AviFile::write( const void* buffer, uint32_t count )
{

  try
  {
  
    if ( _file )
    {

      fwrite( buffer, sizeof( char ), count, _file );
  
      _pos += count;

      if ( _pos > _used )
      {

    	_used = _pos;

      }

      return count;
  
    }
  
    return 0;
  
  }
  GKG_CATCH( "ssize_t gkg::AviFile::write( "
             "const void* buffer, uint32_t count )" );

}


void gkg::AviFile::write_le32( int32_t value )
{

  try
  {
  
    if ( _file )
    {

      char val = value & 0xFF;

      fwrite( &val, sizeof( char ), 1, _file );
      value >>= 8;
      val = value & 0xFF;
      fwrite( &val, sizeof( char ), 1, _file );
      value >>= 8;
      val = value & 0xFF;
      fwrite( &val, sizeof( char ), 1, _file );
      value >>= 8;
      val = value & 0xFF;
      fwrite( &val, sizeof( char ), 1, _file );
      _pos += 4;
      
      if ( _pos > _used )
      {

    	_used = _pos;

      }

    }
  
  }
  GKG_CATCH( "void gkg::AviFile::write_le32( int32_t value )" );

}


void gkg::AviFile::close()
{

  try
  {
  
    if ( _file )
    {
  
      fclose( _file );
      _file = 0;
  
    }
  
  }
  GKG_CATCH( "void gkg::AviFile::close()" );

}
