#ifndef _gkg_graphics_X11_core_Slider_h_
#define _gkg_graphics_X11_core_Slider_h_


#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/ActiveHandler.h>
#include <gkg-core-pattern/Observer.h>
#include <gkg-core-pattern/Observable.h>


namespace gkg
{


class SliderImplementation;
class Stepper;
class Style;


// common slider behavior
class Slider : public ActiveHandler, public Observer
{

  public:

    typedef void ( Adjustable::*Adjustment )( DimensionName );

    Slider( Style* style );
    virtual ~Slider();

    virtual void setNormalThumb( Glyph* glyph );
    virtual void setVisibleThumb( Glyph* glyph );
    virtual void setOldThumb( Glyph* glyph );
    virtual FloatCoordinate getMinimumThumbSize() const;

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void undraw();

    virtual void move( const Event& event );
    virtual void press( const Event& event );
    virtual void drag( const Event& event );
    virtual void release( const Event& event );

    virtual void geometryAllocationChanged(
                                 Canvas* canvas,
                                 const GeometryAllocation& geometryAllocation );

    virtual void update( const Observable* observable, void* arg = 0 );

    virtual void setForwardStepper( Stepper* stepper );
    virtual void setBackwardStepper( Stepper* stepper );
    virtual void moveTo( FloatCoordinate x, FloatCoordinate y );
    virtual void allocateThumb( const GeometryAllocation& geometryAllocation );
    virtual void getAllotmentThumbMajorAxis(
                                   const GeometryAllocation& geometryAllocation,
                                   DimensionName dimensionName,
                                   Adjustable* adjustable,
                                   FloatCoordinate minimumThumbSize,
                                   float& scale,
                                   GeometryAllotment& geometryAllotment );
    virtual void getAllotmentThumbMinorAxis(
                                     const GeometryAllotment& geometryAllotment,
                                     GeometryAllotment& newGeometryAllotment );
    virtual void redrawThumb();
    virtual void reallocateThumb( const GeometryAllocation& geometryAllocation);
    virtual void applyAdjustment( Adjustment adjustment );
    virtual void doAdjustment( Adjustable* adjustable,
                               Adjustment adjustment,
                               DimensionName dimensionName );

    SliderImplementation* getImplementation() const;

  private:

    SliderImplementation* _sliderImplementation;

};


}


#endif
