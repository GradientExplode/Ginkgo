#ifndef _gkg_graphics_X11_core_ColorImplementation_h_
#define _gkg_graphics_X11_core_ColorImplementation_h_


#include <gkg-graphics-X11-base/Color.h>


namespace gkg
{


class Color;
class Visual;


class ColorImplementation
{

  public:

    virtual ~ColorImplementation();

    virtual Visual* getVisual() const;
    virtual bool isMasking() const;

    virtual bool distinguished( const Color* color ) const = 0;
    virtual void getIntensities( FloatColorIntensity& r,
                                 FloatColorIntensity& g,
                                 FloatColorIntensity& b ) const = 0;

  protected:

    ColorImplementation( Visual* visual );

    Visual* _visual;
    bool _masking;

};


}


#endif
