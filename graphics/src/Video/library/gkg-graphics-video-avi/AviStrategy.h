#ifndef _gkg_graphics_video_avi_AviStrategy_h_
#define _gkg_graphics_video_avi_AviStrategy_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class AviStrategy
{

  public:
  
    AviStrategy();
    virtual ~AviStrategy();
    
    virtual void initialize( int32_t width, 
                             int32_t height, 
			     int32_t bitsPerPixel,
			     int32_t nFrames ) = 0;
			     
    virtual off_t lseek( off_t offset, int32_t whence ) = 0;

    virtual void write( std::string fileName ) = 0;
    virtual ssize_t write( const void* buffer, uint32_t count ) = 0;
    virtual void write_le32( int32_t value ) = 0;

    virtual void close();

    virtual uint32_t getBufferSize();
    virtual char* getBuffer();

  protected:

    uint32_t _pos;
    uint32_t _used;

};


}


#endif
