#ifndef _gkg_graphics_video_avi_AviFile_h_
#define _gkg_graphics_video_avi_AviFile_h_


#include <gkg-graphics-video-avi/AviStrategy.h>

#include <cstdio>


namespace gkg
{


class AviFile : public AviStrategy
{

  public:
  
    AviFile();
    ~AviFile();
    
    void initialize( int32_t width, 
                     int32_t height,
		     int32_t bitsPerPixel,
		     int32_t nFrames );
		     
    off_t lseek( off_t offset, int32_t whence );
    
    void write( std::string fileName );
    ssize_t write( const void* buffer, uint32_t count );
    void write_le32( int32_t value );

    void close();
    
  protected:

    FILE* _file;
    char* _tempName;

};


}


#endif
