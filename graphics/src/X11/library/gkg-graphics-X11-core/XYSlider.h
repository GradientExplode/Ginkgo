#ifndef _gkg_graphics_X11_core_XYSlider_h_
#define _gkg_graphics_X11_core_XYSlider_h_


#include <gkg-graphics-X11-core/Slider.h>


namespace gkg
{


class XYSlider : public Slider
{

  public:

    XYSlider( Style* style,
              Adjustable* adjustableX,
              Adjustable* adjustableY );
    virtual ~XYSlider();

    virtual void moveTo( FloatCoordinate x, FloatCoordinate y);
    virtual void allocateThumb( const GeometryAllocation& geometryAllocation );
    virtual void unregisterObservable( Observable* observable );
    virtual void applyAdjustment( Adjustment adjustment );

  private:

    Adjustable* _adjustableX;
    Adjustable* _adjustableY;
    float _scaleX;
    float _scaleY;

};


}


#endif
