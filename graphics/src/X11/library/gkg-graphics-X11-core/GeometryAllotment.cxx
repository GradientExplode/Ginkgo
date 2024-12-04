#include <gkg-graphics-X11-core/GeometryAllotment.h>
#include <gkg-processing-algobase/Math.h>


gkg::GeometryAllotment::GeometryAllotment()
                          : _origin( 0 ),
                            _span( 0 ),
                            _alignment( 0 )
{
}


gkg::GeometryAllotment::GeometryAllotment( const gkg::GeometryAllotment& other )
                          : _origin( other._origin ),
                            _span( other._span ),
                            _alignment( other._alignment )
{
}


gkg::GeometryAllotment::GeometryAllotment( gkg::FloatCoordinate origin,
                                           gkg::FloatCoordinate span,
                                           float alignment )
                       : _origin( origin ),
                         _span( span ),
                         _alignment( alignment )
{
}


gkg::GeometryAllotment& gkg::GeometryAllotment::operator = (
                                           const gkg::GeometryAllotment& other )
{

  _origin = other._origin;
  _span = other._span;
  _alignment = other._alignment;
  
  return *this;

}


bool gkg::GeometryAllotment::isEqualTo( const gkg::GeometryAllotment& other,
                                        float epsilon ) const
{

  if ( !gkg::equal( _origin, other._origin, epsilon ) )
  {

    return false;

  }
  else if ( !gkg::equal( _span, other._span, epsilon ) )
  {

    return false;

  }
  else if ( !gkg::equal( _alignment, other._alignment, epsilon ) )
  {

    return false;

  }
  return true;

}


void gkg::GeometryAllotment::setOrigin( gkg::FloatCoordinate origin )
{

  _origin = origin;

}


void gkg::GeometryAllotment::addOffset( gkg::FloatCoordinate offset )
{

  _origin += offset;

}


gkg::FloatCoordinate gkg::GeometryAllotment::getOrigin() const
{

  return _origin;

}


void gkg::GeometryAllotment::setSpan( gkg::FloatCoordinate span )
{

  _span = span;

}


gkg::FloatCoordinate gkg::GeometryAllotment::getSpan() const
{

  return _span;

}


void gkg::GeometryAllotment::setAlignment( float alignment )
{

  _alignment = alignment;

}


float gkg::GeometryAllotment::getAlignment() const
{

  return _alignment;

}


gkg::FloatCoordinate gkg::GeometryAllotment::getBegin() const
{

  return _origin - ( gkg::FloatCoordinate )( _alignment * _span );

}


gkg::FloatCoordinate gkg::GeometryAllotment::getEnd() const
{

  return _origin - ( gkg::FloatCoordinate )( _alignment * _span ) + _span;

}


