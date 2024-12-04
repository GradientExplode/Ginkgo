#include <gkg-graphics-X11-core/YSlider.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/ForwardPager.h>
#include <gkg-graphics-X11-core/BackwardPager.h>


gkg::YSlider::YSlider( gkg::Style* style, gkg::Adjustable* adjustable )
             : gkg::Slider( style ),
               _adjustable( adjustable )
{

  _adjustable->addObserver( gkg::DimensionY, this );
  gkg::TellTaleState* tellTaleState = new gkg::TellTaleState;
  setForwardStepper( new gkg::ForwardPager( 0,
                                            style,
                                            tellTaleState,
                                            adjustable,
                                            gkg::DimensionY ) );
  setBackwardStepper( new gkg::BackwardPager( 0,
                                              style,
                                              tellTaleState,
                                              adjustable,
                                              gkg::DimensionY ) );
}


gkg::YSlider::~YSlider()
{

  if ( _adjustable )
  {

    _adjustable->removeObserver( gkg::DimensionY, this );

  }

}


void gkg::YSlider::moveTo( gkg::FloatCoordinate, gkg::FloatCoordinate y )
{

  _adjustable->scrollTo( gkg::DimensionY,
                         _adjustable->getLower( gkg::DimensionY ) + y /
                         _scaleY );

}


void 
gkg::YSlider::allocateThumb( const gkg::GeometryAllocation& geometryAllocation )
{

  redrawThumb();
  gkg::GeometryAllocation thumbGeometryAllocation;
  getAllotmentThumbMajorAxis( geometryAllocation,
                              gkg::DimensionY,
                              _adjustable,
                              getMinimumThumbSize(),
                              _scaleY,
                              thumbGeometryAllocation.getAllotmentY() );
  getAllotmentThumbMinorAxis( geometryAllocation.getAllotmentX(),
                              thumbGeometryAllocation.getAllotmentX() );
  reallocateThumb( thumbGeometryAllocation );

}


void gkg::YSlider::unregisterObservable( gkg::Observable* )
{

  _adjustable = 0;

}


void gkg::YSlider::applyAdjustment( gkg::Slider::Adjustment adjustment )
{

  doAdjustment( _adjustable, adjustment, gkg::DimensionX );

}

