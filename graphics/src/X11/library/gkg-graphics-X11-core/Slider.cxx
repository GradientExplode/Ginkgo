#include <gkg-graphics-X11-core/Slider.h>
#include <gkg-graphics-X11-core/SliderImplementation.h>
#include <gkg-graphics-X11-core/Stepper.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/Adjustable.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-core/Hit.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-processing-algobase/Math.h>


#define DEFAULT_MINIMUM_THUMB_SIZE      28.0


gkg::Slider::Slider( gkg::Style* style )
            : gkg::ActiveHandler( 0, style )
{

  gkg::FloatCoordinate minimumThumbSize = DEFAULT_MINIMUM_THUMB_SIZE;
  style->findAttribute( "minimumThumbSize", minimumThumbSize );
  _sliderImplementation = new gkg::SliderImplementation( this,
                                                         minimumThumbSize );

}


gkg::Slider::~Slider() 
{

  delete _sliderImplementation;

}


void gkg::Slider::setNormalThumb( gkg::Glyph* glyph )
{

  _sliderImplementation->setNormalThumb( glyph );

}


void gkg::Slider::setVisibleThumb( gkg::Glyph* glyph )
{

  _sliderImplementation->setVisibleThumb( glyph );

}


void gkg::Slider::setOldThumb( gkg::Glyph* glyph )
{

  _sliderImplementation->setOldThumb( glyph );

}


gkg::FloatCoordinate gkg::Slider::getMinimumThumbSize() const
{

  return _sliderImplementation->getMinimumThumbSize();

}


void gkg::Slider::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  gkg::GeometryRequirement defaultSize( 22.0, fil, 22.0, 0.0 );
  geometryRequisition.setRequirement( gkg::DimensionX, defaultSize );
  geometryRequisition.setRequirement( gkg::DimensionY, defaultSize );

}


void gkg::Slider::allocate( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::GeometryExtension& geometryExtension )
{

  geometryExtension.merge( canvas, geometryAllocation );
  gkg::ActiveHandler::allocate( canvas,
                                geometryAllocation,
                                geometryExtension );

}


void
gkg::Slider::draw( gkg::Canvas* canvas,
                   const gkg::GeometryAllocation& geometryAllocation ) const
{

  canvas->pushClipping();
  canvas->clipRectangle( geometryAllocation.getLeft(),
                         geometryAllocation.getBottom(),
                         geometryAllocation.getRight(),
                         geometryAllocation.getTop() );
  _sliderImplementation->draw( canvas );
  canvas->popClipping();

}


void gkg::Slider::undraw()
{

  _sliderImplementation->undraw();
  this->gkg::ActiveHandler::undraw();

}


void gkg::Slider::move( const gkg::Event& event )
{

  _sliderImplementation->move( event );
  this->gkg::ActiveHandler::move( event );

}


void gkg::Slider::press( const gkg::Event& event )
{

  _sliderImplementation->press( event );

}


void gkg::Slider::drag( const gkg::Event& event )
{

  _sliderImplementation->drag( event );

}


void gkg::Slider::release( const gkg::Event& event )
{

  _sliderImplementation->release( event );

}


void gkg::Slider::geometryAllocationChanged(
                             gkg::Canvas*,
                             const gkg::GeometryAllocation& geometryAllocation )
{

  allocateThumb( geometryAllocation );

}


void gkg::Slider::update( const gkg::Observable*, void* )
{

  if ( getCanvas() )
  {

    allocateThumb( getGeometryAllocation() );

  }

}


void gkg::Slider::setForwardStepper( gkg::Stepper* stepper )
{

  _sliderImplementation->setForwardStepper( stepper );

}


void gkg::Slider::setBackwardStepper( gkg::Stepper* stepper )
{

  _sliderImplementation->setBackwardStepper( stepper );

}


void gkg::Slider::moveTo( gkg::FloatCoordinate, gkg::FloatCoordinate )
{
}


void gkg::Slider::allocateThumb( const gkg::GeometryAllocation& )
{
}


void gkg::Slider::getAllotmentThumbMajorAxis(
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::DimensionName dimensionName,
                              gkg::Adjustable* adjustable,
                              gkg::FloatCoordinate minimumThumbSize,
                              float& scale,
                              gkg::GeometryAllotment& geometryAllotment )
{

  const gkg::GeometryAllotment&
    a = geometryAllocation.getAllotment( dimensionName );
  gkg::FloatCoordinate length = adjustable->getLength( dimensionName );
  gkg::FloatCoordinate currentLength = adjustable->getCurrentLength(
                                                                dimensionName );
  gkg::FloatCoordinate sliderSize = a.getSpan();
  gkg::FloatCoordinate thumbSize;
  gkg::FloatCoordinate thumbStart;
  if ( gkg::equal( length, 0.0f, 1e-3f ) ||
       gkg::equal( length, currentLength, 1e-3f ) )
  {

    thumbSize = sliderSize;
    thumbStart = 0.0;
    scale = 1.0;

  }
  else 
  {

    thumbSize = sliderSize * currentLength / length;
    if ( thumbSize > sliderSize )
    {

      thumbSize = sliderSize;
      thumbStart = 0.0;
      scale = 1.0;

    }
    else
    {

      if ( thumbSize < minimumThumbSize )
      {

        thumbSize = minimumThumbSize;

      }
      scale = ( sliderSize - thumbSize ) / ( length - currentLength );
      thumbStart = scale * ( adjustable->getCurrentLower( dimensionName ) -
                             adjustable->getLower( dimensionName ) );

    }

  }
  geometryAllotment.setOrigin( a.getBegin() + thumbStart );
  geometryAllotment.setSpan( thumbSize );
  geometryAllotment.setAlignment( 0.0 );

}


void gkg::Slider::getAllotmentThumbMinorAxis(
                                const gkg::GeometryAllotment& geometryAllotment,
                                gkg::GeometryAllotment& newGeometryAllotment )
{

  newGeometryAllotment.setOrigin( geometryAllotment.getBegin() );
  newGeometryAllotment.setSpan( geometryAllotment.getSpan() );
  newGeometryAllotment.setAlignment( 0.0 );

}


void gkg::Slider::redrawThumb()
{

  _sliderImplementation->redrawThumb();

}


void gkg::Slider::reallocateThumb(
                             const gkg::GeometryAllocation& geometryAllocation )
{


  gkg::Canvas* canvas = getCanvas();
  canvas->pushTransform();
  canvas->setTransformer( getTransformer() );
  gkg::GeometryExtension geometryExtension;
  geometryExtension.clear();
  _sliderImplementation->reallocateThumb( canvas,
                                          geometryAllocation,
                                          geometryExtension );
  canvas->popTransform();
  _sliderImplementation->redrawThumb();

}


void gkg::Slider::applyAdjustment( gkg::Slider::Adjustment )
{
}


void gkg::Slider::doAdjustment( gkg::Adjustable* adjustable,
                                gkg::Slider::Adjustment adjustment,
                                gkg::DimensionName dimensionName )
{

  if ( adjustable )
  {

    ( adjustable->*adjustment )( dimensionName );

  }

}


gkg::SliderImplementation* gkg::Slider::getImplementation() const
{

  return _sliderImplementation;

}


