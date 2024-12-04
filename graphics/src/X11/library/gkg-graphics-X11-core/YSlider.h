#ifndef _gkg_graphics_X11_core_YSlider_h_
#define _gkg_graphics_X11_core_YSlider_h_


#include <gkg-graphics-X11-core/Slider.h>


namespace gkg
{


class YSlider : public Slider
{

  public:

    YSlider( Style* style, Adjustable* adjustable );
    virtual ~YSlider();

    virtual void moveTo( FloatCoordinate x, FloatCoordinate y);
    virtual void allocateThumb( const GeometryAllocation& geometryAllocation );
    virtual void unregisterObservable( Observable* observable );
    virtual void applyAdjustment( Adjustment adjustment );

  private:

    Adjustable* _adjustable;
    float _scaleY;

};


}


#endif
