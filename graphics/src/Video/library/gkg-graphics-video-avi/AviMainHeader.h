#ifndef _gkg_graphics_video_avi_AviMainHeader_h_
#define _gkg_graphics_video_avi_AviMainHeader_h_


#include <gkg-core-cppext/StdInt.h>


/* flags for use in dwFlags */
#define AVIF_HASINDEX       0x00000010
#define AVIF_MUSTUSEINDEX   0x00000020
#define AVIF_ISINTERLEAVED  0x00000100
#define AVIF_TRUSTCKTYPE    0x00000800
#define AVIF_WASCAPTUREFILE 0x00010000
#define AVIF_COPYRIGHTED    0x00020000


namespace gkg
{


struct AviMainHeader
{

  uint32_t dwMicroSecPerFrame;
  uint32_t dwMaxBytesPerSec;
  uint32_t dwPaddingGranularity;
                           
  uint32_t dwFlags;        
  uint32_t dwTotalFrames;  
  uint32_t dwInitialFrames;
  uint32_t dwStreams;
  uint32_t dwSuggestedBufferSize;

  uint32_t dwWidth;
  uint32_t dwHeight;

  uint32_t dwScale;
  uint32_t dwRate;
  uint32_t dwStart;
  uint32_t dwLength;

};


}


#endif
