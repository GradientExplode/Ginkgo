#ifndef _gkg_graphics_X11_core_GeometryAllocation_h_
#define _gkg_graphics_X11_core_GeometryAllocation_h_


#include <gkg-graphics-X11-core/GeometryAllotment.h>


namespace gkg
{


class GeometryAllocation
{

  public:

    GeometryAllocation();
    GeometryAllocation( const GeometryAllocation& other );

    GeometryAllocation& operator = ( const GeometryAllocation& other );

    bool isEqualTo( const GeometryAllocation& other, float epsilon ) const;

    void setAllotment( DimensionName dimName,
                       const GeometryAllotment& allotment );
    void setAllotmentX( const GeometryAllotment& allotment );
    void setAllotmentY( const GeometryAllotment& allotment );

    GeometryAllotment& getAllotment( DimensionName dimName );
    const GeometryAllotment& getAllotment( DimensionName dimName ) const;
    GeometryAllotment& getAllotmentX();
    GeometryAllotment& getAllotmentY();
    const GeometryAllotment& getAllotmentX() const;
    const GeometryAllotment& getAllotmentY() const;

    FloatCoordinate getX() const;
    FloatCoordinate getY() const;
    FloatCoordinate getLeft() const;
    FloatCoordinate getRight() const;
    FloatCoordinate getBottom() const;
    FloatCoordinate getTop() const;

  private:

    GeometryAllotment _allotmentX;
    GeometryAllotment _allotmentY;

};


}


#endif
