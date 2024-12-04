#include <gkg-graphics-X11-core/SliderImplementation.h>
#include <gkg-graphics-X11-core/Slider.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Stepper.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>


gkg::SliderImplementation::SliderImplementation(
                                         gkg::Slider* slider,
                                         gkg::FloatCoordinate minimumThumbSize )
                          : _target( slider ),
                            _normalThumb( 0 ),
                            _visibleThumb( 0 ),
                            _thumbPatch( 0 ),
                            _oldThumb( 0 ),
                            _minimumThumbSize( minimumThumbSize ),
                            _dragging( false ),
                            _aborted( false ),
                            _showingOldThumb( false ),
                            _forward( 0 ),
                            _backward( 0 ),
                            _stepper( 0 ),
                            _offsetX( 0.0 ),
                            _offsetY( 0.0 )
{
}


gkg::SliderImplementation::~SliderImplementation()
{

  if ( _normalThumb )
  {

    _normalThumb->removeReference();

  }
  if ( _visibleThumb )
  {

    _visibleThumb->removeReference();

  }
  if ( _oldThumb )
  {

    _oldThumb->removeReference();

  }
  if ( _thumbPatch )
  {

    _thumbPatch->removeReference();

  }
  if ( _forward )
  {

    _forward->removeReference();

  }
  if ( _backward )
  {

    _backward->removeReference();

  }

}


gkg::Slider* gkg::SliderImplementation::getTarget() const
{

  return _target;

}


void gkg::SliderImplementation::setNormalThumb( gkg::Glyph* glyph )
{

  if ( glyph )
  {

    glyph->addReference();

  }
  if ( _normalThumb )
  {

    _normalThumb->removeReference();

  }
  _normalThumb = glyph;
  if ( _thumbPatch )
  {

    _thumbPatch->removeReference();

  }
  _thumbPatch = new gkg::Patch( glyph );
  if ( _thumbPatch )
  {

    _thumbPatch->addReference();

  }

}


void gkg::SliderImplementation::setVisibleThumb( gkg::Glyph* glyph )
{

  if ( glyph )
  {

    glyph->addReference();

  }
  if ( _visibleThumb )
  {

    _visibleThumb->removeReference();

  }
  _visibleThumb = glyph;

}


void gkg::SliderImplementation::setOldThumb( gkg::Glyph* glyph )
{

  gkg::Patch* patch = new gkg::Patch( glyph );
  if ( patch )
  {

    patch->addReference();

  }
  if ( _oldThumb )
  {

    _oldThumb->removeReference();

  }
  _oldThumb = patch;

}


gkg::FloatCoordinate gkg::SliderImplementation::getMinimumThumbSize() const
{

  return _minimumThumbSize;

}


void  gkg::SliderImplementation::draw( gkg::Canvas* canvas )
{

  if ( _showingOldThumb )
  {

    _oldThumb->draw( canvas, _oldThumb->getGeometryAllocation() );

  }
  _thumbPatch->draw( canvas, _thumbPatch->getGeometryAllocation() );

}


void  gkg::SliderImplementation::undraw()
{

  if ( _thumbPatch )
  {

    _thumbPatch->undraw();

  }
  if ( _oldThumb )
  {

    _oldThumb->undraw();

  }

}


void gkg::SliderImplementation::move( const gkg::Event& event )
{

  if ( _visibleThumb )
  {

    gkg::Glyph* glyph = ( hitThumb( event ) == 0 ) ? 
                        _visibleThumb : _normalThumb;
    if ( _thumbPatch->getBody() != glyph )
    {

      _thumbPatch->setBody( glyph );
      _thumbPatch->reallocate();
      _thumbPatch->redraw();

    }

  }

}


void gkg::SliderImplementation::press( const gkg::Event& event )
{

  gkg::Event::Button button = event.getPointerButton();
  if ( button == gkg::Event::Right )
  {

    return;

  }

  gkg::FloatCoordinate x, y;
  getPosition( event, x, y );
  const gkg::GeometryAllocation& slider = _target->getGeometryAllocation();
  const gkg::GeometryAllocation& a = _thumbPatch->getGeometryAllocation();
  int32_t rel = hitThumb( event );
  if ( rel == 0 )
  {

    _target->applyAdjustment( &gkg::Adjustable::beginAdjustment );
    _offsetX = slider.getLeft() + x - a.getLeft();
    _offsetY = slider.getBottom() + y - a.getBottom();
    _dragging = true;

  }
  else if ( button == gkg::Event::Left )
  {

    _stepper = ( rel == 1 ) ? _forward : _backward;
    if ( _stepper )
    {

      _stepper->startStepping();

    }

  }
  else
  {

    // middle ==> drag
    _target->applyAdjustment( &gkg::Adjustable::beginAdjustment );
    _dragging = true;
    _offsetX = slider.getLeft() + ( a.getRight() - a.getLeft() ) / 2.0;
    _offsetY = slider.getBottom() + ( a.getTop() - a.getBottom() ) / 2.0;
    _target->moveTo( x - _offsetX, y - _offsetY );
    _target->move( event );

  }

}


void gkg::SliderImplementation::drag( const gkg::Event& event )
{

  if ( !_aborted && _dragging )
  {

    if ( !_showingOldThumb && _oldThumb )
    {

      _showingOldThumb = true;
      gkg::Canvas* canvas = _target->getCanvas();
      canvas->pushTransform();
      canvas->setTransformer( _target->getTransformer() );
      gkg::GeometryExtension geometryExtension;
      geometryExtension.clear();
      _oldThumb->allocate( canvas,
                           _thumbPatch->getGeometryAllocation(),
                           geometryExtension );
      canvas->popTransform();

    }
    gkg::FloatCoordinate x, y;
    getPosition( event, x, y );
    _target->moveTo( x - _offsetX, y - _offsetY );

  }

}


void gkg::SliderImplementation::release( const gkg::Event& event )
{

  if ( _dragging )
  {

    if ( _showingOldThumb )
    {

      _showingOldThumb = false;
      _oldThumb->redraw();

    }
    _dragging = false;
    if ( _aborted )
    {

      _aborted = false;
      return;

    }
    gkg::FloatCoordinate x, y;
    getPosition( event, x, y );
    _target->moveTo( x - _offsetX, y - _offsetY );
    _target->redrawThumb();
    _target->move( event );
    _target->applyAdjustment( &gkg::Adjustable::commitAdjustment );

  }
  else if ( _stepper )
  {

    _stepper->stopStepping();
    _stepper = 0;
    _target->move( event );

  }

}


void 
gkg::SliderImplementation::setForwardStepper( gkg::Stepper* stepper )
{

  if ( stepper )
  {

    stepper->addReference();

  }
  if ( _forward )
  {

    _forward->removeReference();

  }
  _forward = stepper;

}


void 
gkg::SliderImplementation::setBackwardStepper( gkg::Stepper* stepper )
{

  if ( stepper )
  {

    stepper->addReference();

  }
  if ( _backward )
  {

    _backward->removeReference();

  }
  _backward = stepper;

}


void gkg::SliderImplementation::redrawThumb()
{

  _thumbPatch->redraw();

}


void gkg::SliderImplementation::reallocateThumb(
                              gkg::Canvas* canvas,
                              const  gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  _thumbPatch->allocate( canvas,
                         geometryAllocation,
                         geometryExtension );

}


int32_t gkg::SliderImplementation::hitThumb( const gkg::Event& event )
{

  gkg::FloatCoordinate x = event.getPointerX();
  gkg::FloatCoordinate y = event.getPointerY();
  const gkg::GeometryExtension& 
    geometryExtension = _thumbPatch->getGeometryExtension();
  if ( ( x >= geometryExtension.getLeft() ) &&
       ( x < geometryExtension.getRight() ) &&
       ( y >= geometryExtension.getBottom() ) &&
       ( y < geometryExtension.getTop() ) )
  {

    gkg::Canvas* canvas = _target->getCanvas();
    const gkg::Transformer& transformer = _target->getTransformer();
    gkg::Hit hit( &event );
    hit.setTransform( transformer );
    canvas->pushTransform();
    canvas->setTransformer( transformer );
    _thumbPatch->pick( canvas, _thumbPatch->getGeometryAllocation(), 0, hit );
    canvas->popTransform();
    return hit.hasAny() ? 0 : 1;

  }
  if ( ( x < geometryExtension.getLeft() ) ||
       ( y < geometryExtension.getBottom() ) )
  {

    return -1;

  }
  return 1;

}


void gkg::SliderImplementation::getPosition( const gkg::Event& event,
                                             gkg::FloatCoordinate& x,
                                             gkg::FloatCoordinate& y )
{

  x = event.getPointerX();
  y = event.getPointerY();
  _target->getTransformer().inverseTransform( x, y );

}

