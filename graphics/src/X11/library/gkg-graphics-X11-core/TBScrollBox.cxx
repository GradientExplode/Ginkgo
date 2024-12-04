#include <gkg-graphics-X11-core/TBScrollBox.h>
#include <gkg-graphics-X11-core/TBScrollBoxImplementation.h>
#include <gkg-processing-algobase/Math.h>


gkg::TBScrollBox::TBScrollBox( bool shrinkable )
                 : gkg::ScrollBox()
{

  _tbScrollBoxImplementation = new gkg::TBScrollBoxImplementation( this,
                                                                   shrinkable );

}


gkg::TBScrollBox::~TBScrollBox()
{

  delete _tbScrollBoxImplementation;

}


void 
gkg::TBScrollBox::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  _tbScrollBoxImplementation->request( geometryRequisition );

}


void gkg::TBScrollBox::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  _tbScrollBoxImplementation->allocate( canvas,
                                        geometryAllocation,
                                        geometryExtension );

}


void gkg::TBScrollBox::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  _tbScrollBoxImplementation->draw( canvas, geometryAllocation );

}


void gkg::TBScrollBox::pick( gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation,
                             int32_t depth,
                             gkg::Hit& hit )
{

  _tbScrollBoxImplementation->pick( canvas, geometryAllocation, depth, hit );

}


void gkg::TBScrollBox::undraw()
{

  _tbScrollBoxImplementation->undraw();
  this->gkg::ScrollBox::undraw();

}


void gkg::TBScrollBox::modified( gkg::Glyph::Index index )
{

  _tbScrollBoxImplementation->modified( index );

}


bool gkg::TBScrollBox::isShown( gkg::Glyph::Index index ) const
{

  return _tbScrollBoxImplementation->isShown( index );

}


gkg::Glyph::Index gkg::TBScrollBox::getFirstShown() const
{

  return _tbScrollBoxImplementation->getFirstShown();

}


gkg::Glyph::Index gkg::TBScrollBox::getLastShown() const
{

  return _tbScrollBoxImplementation->getLastShown();

}


void gkg::TBScrollBox::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  _tbScrollBoxImplementation->getGeometryAllotment( index,
                                                    dimensionName,
                                                    geometryAllotment );

}


gkg::FloatCoordinate gkg::TBScrollBox::getLower( gkg::DimensionName ) const
{

  return ( gkg::FloatCoordinate )0;

}


gkg::FloatCoordinate gkg::TBScrollBox::getUpper( gkg::DimensionName ) const
{

  return ( gkg::FloatCoordinate )( getCount() - 1 );

}


gkg::FloatCoordinate gkg::TBScrollBox::getLength( gkg::DimensionName ) const
{

  return ( gkg::FloatCoordinate )getCount();

}


gkg::FloatCoordinate 
gkg::TBScrollBox::getCurrentLower( gkg::DimensionName ) const
{

  _tbScrollBoxImplementation->doRefresh();
  return ( gkg::FloatCoordinate )( getCount() -
                                   _tbScrollBoxImplementation->getEnd() );

}


gkg::FloatCoordinate
gkg::TBScrollBox::getCurrentUpper( gkg::DimensionName ) const
{

  _tbScrollBoxImplementation->doRefresh();
  return ( gkg::FloatCoordinate )( getCount() - 1 -
                                   _tbScrollBoxImplementation->getStart() );

}


gkg::FloatCoordinate
gkg::TBScrollBox::getCurrentLength( gkg::DimensionName ) const
{

  _tbScrollBoxImplementation->doRefresh();
  return ( gkg::FloatCoordinate )( _tbScrollBoxImplementation->getEnd() -
                                   _tbScrollBoxImplementation->getStart() );

}


void gkg::TBScrollBox::scrollForward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, -( int32_t )getSmallScroll( dimensionName ) );

}


void gkg::TBScrollBox::scrollBackward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, ( int32_t )getSmallScroll( dimensionName ) );

}


void gkg::TBScrollBox::pageForward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, -( int32_t )getLargeScroll( dimensionName ) );

}


void gkg::TBScrollBox::pageBackward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, ( int32_t )getLargeScroll( dimensionName ) );

}


void gkg::TBScrollBox::scrollTo( gkg::DimensionName dimensionName,
                                 gkg::FloatCoordinate lower )
{

  _tbScrollBoxImplementation->doRefresh();
  gkg::Glyph::Index newEnd = getCount() - gkg::round( lower );
  gkg::Glyph::Index newStart = newEnd - _tbScrollBoxImplementation->getEnd() +
                                        _tbScrollBoxImplementation->getStart();
  doScroll( dimensionName, newStart, newEnd );

}


gkg::TBScrollBoxImplementation* gkg::TBScrollBox::getImplementation() const
{

  return _tbScrollBoxImplementation;

}


void gkg::TBScrollBox::scrollBy( gkg::DimensionName dimensionName, 
                                 int32_t offset )
{

  _tbScrollBoxImplementation->doRefresh();
  doScroll( dimensionName,
            _tbScrollBoxImplementation->getStart() + offset,
            _tbScrollBoxImplementation->getEnd() + offset );

}


void gkg::TBScrollBox::doScroll( gkg::DimensionName dimensionName,
                                 gkg::Glyph::Index newStart,
                                 gkg::Glyph::Index newEnd )
{

  _tbScrollBoxImplementation->doScroll( dimensionName, newStart, newEnd );

}
