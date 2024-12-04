#ifndef _gkg_graphics_X11_core_XSlider_h_
#define _gkg_graphics_X11_core_XSlider_h_


#include <gkg-graphics-X11-core/Slider.h>


namespace gkg
{


class XSlider : public Slider
{

  public:

    XSlider( Style* style, Adjustable* adjustable );
    virtual ~XSlider();

    virtual void moveTo( FloatCoordinate x, FloatCoordinate y);
    virtual void allocateThumb( const GeometryAllocation& geometryAllocation );
    virtual void unregisterObservable( Observable* observable );
    virtual void applyAdjustment( Adjustment adjustment );

  private:

    Adjustable* _adjustable;
    float _scaleX;

};


}


#endif
