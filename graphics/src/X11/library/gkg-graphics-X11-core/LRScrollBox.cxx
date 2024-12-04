#include <gkg-graphics-X11-core/LRScrollBox.h>
#include <gkg-graphics-X11-core/LRScrollBoxImplementation.h>
#include <gkg-processing-algobase/Math.h>


gkg::LRScrollBox::LRScrollBox( bool shrinkable )
                 : gkg::ScrollBox()
{

  _lrScrollBoxImplementation = new gkg::LRScrollBoxImplementation( this,
                                                                   shrinkable );

}


gkg::LRScrollBox::~LRScrollBox()
{

  delete _lrScrollBoxImplementation;

}


void 
gkg::LRScrollBox::request( gkg::GeometryRequisition& geometryRequisition ) const
{

  _lrScrollBoxImplementation->request( geometryRequisition );

}


void 
gkg::LRScrollBox::allocate( gkg::Canvas* canvas,
                            const gkg::GeometryAllocation& geometryAllocation,
                            gkg::GeometryExtension& geometryExtension )
{

  _lrScrollBoxImplementation->allocate( canvas,
                                        geometryAllocation,
                                        geometryExtension );

}


void gkg::LRScrollBox::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  _lrScrollBoxImplementation->draw( canvas, geometryAllocation );

}


void gkg::LRScrollBox::pick( gkg::Canvas* canvas,
                             const gkg::GeometryAllocation& geometryAllocation,
                             int32_t depth,
                             gkg::Hit& hit )
{

  _lrScrollBoxImplementation->pick( canvas, geometryAllocation, depth, hit );

}


void gkg::LRScrollBox::undraw()
{

  _lrScrollBoxImplementation->undraw();
  this->gkg::ScrollBox::undraw();

}


void gkg::LRScrollBox::modified( gkg::Glyph::Index index )
{

  _lrScrollBoxImplementation->modified( index );

}


bool gkg::LRScrollBox::isShown( gkg::Glyph::Index index ) const
{

  return _lrScrollBoxImplementation->isShown( index );

}


gkg::Glyph::Index gkg::LRScrollBox::getFirstShown() const
{

  return _lrScrollBoxImplementation->getFirstShown();

}


gkg::Glyph::Index gkg::LRScrollBox::getLastShown() const
{

  return _lrScrollBoxImplementation->getLastShown();

}


void gkg::LRScrollBox::getGeometryAllotment(
                               gkg::Glyph::Index index,
                               gkg::DimensionName dimensionName,
                               gkg::GeometryAllotment& geometryAllotment ) const
{

  _lrScrollBoxImplementation->getGeometryAllotment( index,
                                                    dimensionName,
                                                    geometryAllotment );

}


gkg::FloatCoordinate gkg::LRScrollBox::getLower( gkg::DimensionName ) const
{

  return ( gkg::FloatCoordinate )0;

}


gkg::FloatCoordinate gkg::LRScrollBox::getUpper( gkg::DimensionName ) const
{

  return ( gkg::FloatCoordinate )( getCount() - 1 );

}


gkg::FloatCoordinate gkg::LRScrollBox::getLength( gkg::DimensionName ) const
{

  return ( gkg::FloatCoordinate )getCount();

}


gkg::FloatCoordinate
gkg::LRScrollBox::getCurrentLower( gkg::DimensionName ) const
{

  _lrScrollBoxImplementation->doRefresh();
  return ( gkg::FloatCoordinate )_lrScrollBoxImplementation->getStart();

}


gkg::FloatCoordinate
gkg::LRScrollBox::getCurrentUpper( gkg::DimensionName ) const
{

  _lrScrollBoxImplementation->doRefresh();
  return ( gkg::FloatCoordinate )_lrScrollBoxImplementation->getEnd();

}


gkg::FloatCoordinate
gkg::LRScrollBox::getCurrentLength( gkg::DimensionName ) const
{

  _lrScrollBoxImplementation->doRefresh();
  return ( gkg::FloatCoordinate )( _lrScrollBoxImplementation->getEnd() -
                                   _lrScrollBoxImplementation->getStart() );

}


void gkg::LRScrollBox::scrollForward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, ( int32_t )getSmallScroll( dimensionName ) );

}


void gkg::LRScrollBox::scrollBackward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, -( int32_t )getSmallScroll( dimensionName ) );

}


void gkg::LRScrollBox::pageForward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, ( int32_t )getLargeScroll( dimensionName ) );

}


void gkg::LRScrollBox::pageBackward( gkg::DimensionName dimensionName )
{

  scrollBy( dimensionName, -( int32_t )getLargeScroll( dimensionName ) );

}


void gkg::LRScrollBox::scrollTo( gkg::DimensionName dimensionName,
                                 gkg::FloatCoordinate lower )
{

  _lrScrollBoxImplementation->doRefresh();
  gkg::Glyph::Index newStart = gkg::round( lower );
  gkg::Glyph::Index newEnd = newStart + _lrScrollBoxImplementation->getEnd() -
                                        _lrScrollBoxImplementation->getStart();
  doScroll( dimensionName, newStart, newEnd );

}


gkg::LRScrollBoxImplementation* gkg::LRScrollBox::getImplementation() const
{

  return _lrScrollBoxImplementation;

}


void gkg::LRScrollBox::scrollBy( gkg::DimensionName dimensionName, 
                                 int32_t offset )
{

  _lrScrollBoxImplementation->doRefresh();
  doScroll( dimensionName,
            _lrScrollBoxImplementation->getStart() + offset,
            _lrScrollBoxImplementation->getEnd() + offset );

}


void gkg::LRScrollBox::doScroll( gkg::DimensionName dimensionName,
                                 gkg::Glyph::Index newStart,
                                 gkg::Glyph::Index newEnd )
{

  _lrScrollBoxImplementation->doScroll( dimensionName, newStart, newEnd );

}
