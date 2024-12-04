#ifndef _gkg_graphics_video_avi_Bitmap_h_
#define _gkg_graphics_video_avi_Bitmap_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


struct BitmapInfoHeader
{

  uint32_t biSize;
  uint32_t biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  uint32_t biXPelsPerMeter;
  uint32_t biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;

};


}


#endif
