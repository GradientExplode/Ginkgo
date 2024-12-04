#ifndef _gkg_graphics_X11_core_SliderImplementation_h_
#define _gkg_graphics_X11_core_SliderImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Slider;
class Event;
class Patch;
class Stepper;


class SliderImplementation
{

  public:

    SliderImplementation( Slider* slider, FloatCoordinate minimumThumbSize );
    ~SliderImplementation();

    Slider* getTarget() const;

    void setNormalThumb( Glyph* glyph );
    void setVisibleThumb( Glyph* glyph );
    void setOldThumb( Glyph* glyph );
    FloatCoordinate getMinimumThumbSize() const;

    void draw( Canvas* canvas );
    void undraw();

    void move( const Event& event );
    void press( const Event& event );
    void drag( const Event& event );
    void release( const Event& event );

    void setForwardStepper( Stepper* stepper );
    void setBackwardStepper( Stepper* stepper );

    void redrawThumb();
    void reallocateThumb( Canvas* canvas,
                          const GeometryAllocation& geometryAllocation,
                          GeometryExtension& geometryExtension );

  private:

    int32_t hitThumb( const Event& event );
    void getPosition( const Event& event,
                      FloatCoordinate& x, FloatCoordinate& y );

    Slider* _target;

    Glyph* _normalThumb;
    Glyph* _visibleThumb;
    Patch* _thumbPatch;
    Patch* _oldThumb;
    FloatCoordinate _minimumThumbSize;
    bool _dragging;
    bool _aborted;
    bool _showingOldThumb;
    Stepper* _forward;
    Stepper* _backward;
    Stepper* _stepper;
    FloatCoordinate _offsetX;
    FloatCoordinate _offsetY;

};


}


#endif
