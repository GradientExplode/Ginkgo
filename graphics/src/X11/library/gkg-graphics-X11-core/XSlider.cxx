#include <gkg-graphics-X11-core/XSlider.h>
#include <gkg-graphics-X11-core/TellTale.h>
#include <gkg-graphics-X11-core/ForwardPager.h>
#include <gkg-graphics-X11-core/BackwardPager.h>


gkg::XSlider::XSlider( gkg::Style* style, gkg::Adjustable* adjustable )
             : gkg::Slider( style ),
               _adjustable( adjustable )
{

  _adjustable->addObserver( gkg::DimensionX, this );
  gkg::TellTaleState* tellTaleState = new gkg::TellTaleState;
  setForwardStepper( new gkg::ForwardPager( 0,
                                            style,
                                            tellTaleState,
                                            adjustable,
                                            gkg::DimensionX ) );
  setBackwardStepper( new gkg::BackwardPager( 0,
                                              style,
                                              tellTaleState,
                                              adjustable,
                                              gkg::DimensionX ) );
}


gkg::XSlider::~XSlider()
{

  if ( _adjustable )
  {

    _adjustable->removeObserver( gkg::DimensionX, this );

  }

}


void gkg::XSlider::moveTo( gkg::FloatCoordinate x, gkg::FloatCoordinate )
{

  _adjustable->scrollTo( gkg::DimensionX,
                         _adjustable->getLower( gkg::DimensionX ) + x /
                         _scaleX );

}


void
gkg::XSlider::allocateThumb( const gkg::GeometryAllocation& geometryAllocation )
{

  redrawThumb();
  gkg::GeometryAllocation thumbGeometryAllocation;
  getAllotmentThumbMajorAxis( geometryAllocation,
                              gkg::DimensionX,
                              _adjustable,
                              getMinimumThumbSize(),
                              _scaleX,
                              thumbGeometryAllocation.getAllotmentX() );
  getAllotmentThumbMinorAxis( geometryAllocation.getAllotmentY(),
                              thumbGeometryAllocation.getAllotmentY() );
  reallocateThumb( thumbGeometryAllocation );

}


void gkg::XSlider::unregisterObservable( gkg::Observable* )
{

  _adjustable = 0;

}


void gkg::XSlider::applyAdjustment( gkg::Slider::Adjustment adjustment )
{

  doAdjustment( _adjustable, adjustment, gkg::DimensionX );

}

