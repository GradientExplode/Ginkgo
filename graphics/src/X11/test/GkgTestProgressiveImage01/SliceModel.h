#ifndef _gkg_graphics_X11_GkgTestProgressiveImage01_SliceModel_h_
#define _gkg_graphics_X11_GkgTestProgressiveImage01_SliceModel_h_


#include <gkg-graphics-X11-progressive/ProgressiveModel.h>


class SliceModel : public gkg::ProgressiveModel
{

  public:

    SliceModel();
    ~SliceModel();

    bool getValueAt( gkg::PixelCoordinate x,
                     gkg::PixelCoordinate y,
                     gkg::FloatColorIntensity& red,
                     gkg::FloatColorIntensity& green,
                     gkg::FloatColorIntensity& blue,
                     float& alpha ) const;

  private:

    float* _data;

};


#endif
