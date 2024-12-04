#include <gkg-graphics-X11-core/GeometryExtension.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/Transformer.h>
#include <gkg-processing-algobase/Math.h>


gkg::GeometryExtension::GeometryExtension()
                          : _beginX( 0 ),
                            _endX( 0 ),
                            _beginY( 0 ),
                            _endY( 0 )
{
}


gkg::GeometryExtension::GeometryExtension( const gkg::GeometryExtension& other )
                       : _beginX( other._beginX ),
                         _endX( other._endX ),
                         _beginY( other._beginY ),
                         _endY( other._endY )
{
}


gkg::GeometryExtension& gkg::GeometryExtension::operator = (
                                           const gkg::GeometryExtension& other )
{

  _beginX = other._beginX;
  _endX = other._endX;
  _beginY = other._beginY;
  _endY = other._endY;

  return *this;

}


void gkg::GeometryExtension::set( gkg::Canvas* canvas,
                                  const gkg::GeometryAllocation& allocation )
{

  setXY( canvas,
         allocation.getLeft(),
         allocation.getBottom(),
         allocation.getRight(),
         allocation.getTop() );

}


void gkg::GeometryExtension::setXY( gkg::Canvas* canvas,
                                    gkg::FloatCoordinate left,
                                    gkg::FloatCoordinate bottom,
                                    gkg::FloatCoordinate right,
                                    gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate l = left;
  gkg::FloatCoordinate b = bottom;
  gkg::FloatCoordinate r = right;
  gkg::FloatCoordinate t = top;
  transformXY( canvas, l, b, r, t );
  _beginX = l;
  _endX = r;
  _beginY = b;
  _endY = t;

}


void gkg::GeometryExtension::clear()
{

  _beginX = fil;
  _endX = -fil;
  _beginY = fil;
  _endY = -fil;

}


void gkg::GeometryExtension::merge( const gkg::GeometryExtension& other )
{

  _beginX = std::min( _beginX, other._beginX );
  _endX = std::max( _endX, other._endX );
  _beginY = std::min( _beginY, other._beginY );
  _endY = std::max( _endY, other._endY );

}


void gkg::GeometryExtension::merge( gkg::Canvas* canvas,
                                    const gkg::GeometryAllocation& allocation )
{

  mergeXY( canvas,
           allocation.getLeft(),
           allocation.getBottom(),
           allocation.getRight(),
           allocation.getTop() );

}


void gkg::GeometryExtension::mergeXY( gkg::Canvas* canvas,
                                      gkg::FloatCoordinate left,
                                      gkg::FloatCoordinate bottom,
                                      gkg::FloatCoordinate right,
                                      gkg::FloatCoordinate top )
{

  gkg::FloatCoordinate l = left;
  gkg::FloatCoordinate b = bottom;
  gkg::FloatCoordinate r = right;
  gkg::FloatCoordinate t = top;
  transformXY( canvas, l, b, r, t );
  _beginX = std::min( _beginX, l );
  _endX = std::max( _endX, r );
  _beginY = std::min( _beginY, b );
  _endY = std::max( _endY, t );

}


gkg::FloatCoordinate gkg::GeometryExtension::getLeft() const
{

  return _beginX;

}


gkg::FloatCoordinate gkg::GeometryExtension::getBottom() const
{

  return _beginY;

}


gkg::FloatCoordinate gkg::GeometryExtension::getRight() const
{

  return _endX;

}


gkg::FloatCoordinate gkg::GeometryExtension::getTop() const
{

  return _endY;

}




void gkg::GeometryExtension::transformXY( gkg::Canvas* canvas,
                                          gkg::FloatCoordinate& left,
                                          gkg::FloatCoordinate& bottom,
                                          gkg::FloatCoordinate& right,
                                          gkg::FloatCoordinate& top )
{

  if ( canvas )
  {

    const gkg::Transformer& transformer = canvas->getTransformer();
    if ( !transformer.isIdentity() )
    {

      gkg::FloatCoordinate x1, y1, x2, y2, x3, y3, x4, y4;
      transformer.transform( left, bottom, x1, y1 );
      transformer.transform( left, top, x2, y2 );
      transformer.transform( right, top, x3, y3 );
      transformer.transform( right, bottom, x4, y4 );
      left = gkg::min( x1, x2, x3, x4 );
      bottom = gkg::min( y1, y2, y3, y4 );
      right = gkg::max( x1, x2, x3, x4 );
      top = gkg::max( y1, y2, y3, y4 );

    }

  }

}

