#ifndef _gkg_graphics_video_avi_AviWriter_h_
#define _gkg_graphics_video_avi_AviWriter_h_


#include <gkg-graphics-video-avi/AviMainHeader.h>
#include <gkg-graphics-video-avi/AviStreamHeader.h>
#include <gkg-graphics-video-avi/AviIndex.h>
#include <gkg-graphics-video-avi/Bitmap.h>

#include <vector>
#include <string>


namespace gkg
{


template < class S >
class AviWriter
{

  public:

    AviWriter( int32_t width, 
               int32_t height,
	       int32_t bitsPerPixel,
               int32_t nFrames, 
	       int32_t frameRate,
	       int32_t flags = 0 );
    virtual ~AviWriter();

    void addFrame( const void* dib, uint32_t size );
    void write( std::string fileName );

  protected:

    void initialize( int32_t width, 
                     int32_t height, 
		     int32_t bitsPerPixel,
                     int32_t nFrames, 
	             int32_t frameRate,
                     int32_t flags );
    void writeHeaders();
    void finalize();

    S _aviBuffer;
 
    AviMainHeader _mainHeader;
    AviStreamHeader _streamHeader;
 
    BitmapInfoHeader _bih;
 
    uint32_t _moviSizeOffset;
    uint32_t _moviSize;
    
    std::vector< AviIndexEntry > _index;

};


}


#endif
