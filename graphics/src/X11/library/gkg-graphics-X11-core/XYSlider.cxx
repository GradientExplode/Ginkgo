#include <gkg-graphics-X11-core/XYSlider.h>


gkg::XYSlider::XYSlider( gkg::Style* style,
                         gkg::Adjustable* adjustableX,
                         gkg::Adjustable* adjustableY )
              : gkg::Slider( style ),
                _adjustableX( adjustableX ),
                _adjustableY( adjustableY )
{

  _adjustableX->addObserver( gkg::DimensionX, this );
  _adjustableY->addObserver( gkg::DimensionY, this );

}


gkg::XYSlider::~XYSlider()
{

  if ( _adjustableX )
  {

    _adjustableX->removeObserver( gkg::DimensionX, this );

  }
  if ( _adjustableY )
  {

    _adjustableY->removeObserver( gkg::DimensionY, this );

  }

}


void gkg::XYSlider::moveTo( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _adjustableX->scrollTo( gkg::DimensionX,
                          _adjustableX->getLower( gkg::DimensionX ) + x /
                          _scaleX );
  _adjustableY->scrollTo( gkg::DimensionY,
                          _adjustableY->getLower( gkg::DimensionY ) + y /
                          _scaleY );

}


void gkg::XYSlider::allocateThumb(
                             const gkg::GeometryAllocation& geometryAllocation )
{

  redrawThumb();
  gkg::GeometryAllocation thumbGeometryAllocation;
  getAllotmentThumbMajorAxis( geometryAllocation,
                              gkg::DimensionX,
                              _adjustableX,
                              getMinimumThumbSize(),
                              _scaleX,
                              thumbGeometryAllocation.getAllotmentX() );
  getAllotmentThumbMajorAxis( geometryAllocation,
                              gkg::DimensionY,
                              _adjustableY,
                              getMinimumThumbSize(),
                              _scaleY,
                              thumbGeometryAllocation.getAllotmentY() );
  reallocateThumb( thumbGeometryAllocation );

}


void gkg::XYSlider::unregisterObservable( gkg::Observable* )
{

  _adjustableX = 0;
  _adjustableY = 0;

}


void gkg::XYSlider::applyAdjustment( gkg::Slider::Adjustment adjustment )
{

  doAdjustment( _adjustableX, adjustment, gkg::DimensionX );
  doAdjustment( _adjustableY, adjustment, gkg::DimensionY );

}
