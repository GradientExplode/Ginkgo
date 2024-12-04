#ifndef _gkg_graphics_X11_progressive_ProgressiveModel_h_
#define _gkg_graphics_X11_progressive_ProgressiveModel_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-base/Color.h>


namespace gkg
{


class ProgressiveModel : public RCGUIObject
{

  public:

    ProgressiveModel( PixelCoordinate pixelWidth,
                      PixelCoordinate pixelHeight );
    virtual ~ProgressiveModel();

    PixelCoordinate getPixelWidth() const;
    PixelCoordinate getPixelHeight() const;

    virtual bool getValueAt( PixelCoordinate x,
                             PixelCoordinate y,
                             FloatColorIntensity& red,
                             FloatColorIntensity& green,
                             FloatColorIntensity& blue,
                             float& alpha ) const = 0;

  protected:

    PixelCoordinate _pixelWidth;
    PixelCoordinate _pixelHeight;

};


}


#endif
