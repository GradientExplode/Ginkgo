#include <gkg-graphics-X11-core/GeometryAllocation.h>


static gkg::GeometryAllotment* emptyAllotment = 0;


gkg::GeometryAllocation::GeometryAllocation()
                        : _allotmentX(),
                          _allotmentY( 0, 0, 0 )
{
}


gkg::GeometryAllocation::GeometryAllocation( 
                                          const gkg::GeometryAllocation& other )
                        : _allotmentX( other._allotmentX ),
                          _allotmentY( other._allotmentY )
{
}


gkg::GeometryAllocation& gkg::GeometryAllocation::operator = (
                                          const gkg::GeometryAllocation& other )
{

  _allotmentX = other._allotmentX;
  _allotmentY = other._allotmentY;

  return *this;

}


bool gkg::GeometryAllocation::isEqualTo( const GeometryAllocation& other,
                                         float epsilon ) const
{

  return _allotmentX.isEqualTo( other._allotmentX, epsilon ) &&
         _allotmentY.isEqualTo( other._allotmentY, epsilon );

}


void gkg::GeometryAllocation::setAllotment(
                                       gkg::DimensionName dimName,
                                       const gkg::GeometryAllotment& allotment )
{

  if ( dimName == gkg::DimensionX )
  {

    _allotmentX = allotment;

  }
  else if ( dimName == gkg::DimensionY )
  {

    _allotmentY = allotment;

  }

}


void 
gkg::GeometryAllocation::setAllotmentX( 
                                       const gkg::GeometryAllotment& allotment )
{

  _allotmentX = allotment;

}


void 
gkg::GeometryAllocation::setAllotmentY( 
                                       const gkg::GeometryAllotment& allotment )
{

  _allotmentY = allotment;

}


gkg::GeometryAllotment& 
gkg::GeometryAllocation::getAllotment( gkg::DimensionName dimName )
{

  switch ( dimName )
  {

    case gkg::DimensionX:

      return _allotmentX;
      break;

    case gkg::DimensionY:

      return _allotmentY;
      break;

    default:

      if ( !emptyAllotment )
      {

        emptyAllotment = new gkg::GeometryAllotment;

      }
      return *emptyAllotment;

  }

}


const gkg::GeometryAllotment& 
gkg::GeometryAllocation::getAllotment( gkg::DimensionName dimName ) const
{

  switch ( dimName )
  {

    case gkg::DimensionX:

      return _allotmentX;
      break;

    case gkg::DimensionY:

      return _allotmentY;
      break;

    default:

      if ( !emptyAllotment )
      {

        emptyAllotment = new gkg::GeometryAllotment;

      }
      return *emptyAllotment;

  }

}


gkg::GeometryAllotment& gkg::GeometryAllocation::getAllotmentX()
{

  return _allotmentX;

}


gkg::GeometryAllotment& gkg::GeometryAllocation::getAllotmentY()
{

  return _allotmentY;

}


const gkg::GeometryAllotment& 
gkg::GeometryAllocation::getAllotmentX() const
{

  return _allotmentX;

}


const gkg::GeometryAllotment& 
gkg::GeometryAllocation::getAllotmentY() const
{

  return _allotmentY;

}


gkg::FloatCoordinate gkg::GeometryAllocation::getX() const
{

  return _allotmentX.getOrigin();

}


gkg::FloatCoordinate gkg::GeometryAllocation::getY() const
{

  return _allotmentY.getOrigin();

}


gkg::FloatCoordinate gkg::GeometryAllocation::getLeft() const
{

  return _allotmentX.getBegin();

}


gkg::FloatCoordinate gkg::GeometryAllocation::getRight() const
{

  return _allotmentX.getEnd();

}


gkg::FloatCoordinate gkg::GeometryAllocation::getBottom() const
{

  return _allotmentY.getBegin();

}


gkg::FloatCoordinate gkg::GeometryAllocation::getTop() const
{

  return _allotmentY.getEnd();

}


