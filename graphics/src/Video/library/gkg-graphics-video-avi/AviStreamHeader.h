#ifndef _gkg_graphics_video_avi_AviStreamHeader_h_
#define _gkg_graphics_video_avi_AviStreamHeader_h_


#include <gkg-graphics-video-avi/Rect.h>


namespace gkg
{


struct AviStreamHeader
{

  uint32_t fccType;
  uint32_t fccHandler;
  uint32_t dwFlags;        
  uint16_t wPriority;      
  uint16_t wLanguage;
  uint32_t dwInitialFrames;
  uint32_t dwScale;
  uint32_t dwRate;         
  uint32_t dwStart;
  uint32_t dwLength;       
  uint32_t dwSuggestedBufferSize;
  int32_t  dwQuality;
  uint32_t dwSampleSize;
  Rect     rcFrame;

};


}


#endif
