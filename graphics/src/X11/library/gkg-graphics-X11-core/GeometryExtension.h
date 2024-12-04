#ifndef _gkg_graphics_X11_core_GeometryExtension_h_
#define _gkg_graphics_X11_core_GeometryExtension_h_


#include <gkg-graphics-X11-core/GeometryAllocation.h>


namespace gkg
{


class Canvas;


class GeometryExtension
{

  public:

    GeometryExtension();
    GeometryExtension( const GeometryExtension& other );
    
    GeometryExtension& operator = ( const GeometryExtension& other );

    void set( Canvas* canvas, const GeometryAllocation& allocation );
    void setXY( Canvas*,
                FloatCoordinate left,
                FloatCoordinate bottom,
                FloatCoordinate right,
                FloatCoordinate top );
    void clear();

    void merge( const GeometryExtension& other );
    void merge( Canvas*, const GeometryAllocation& allocation );
    void mergeXY( Canvas*,
                  FloatCoordinate left,
                  FloatCoordinate bottom,
                  FloatCoordinate right,
                  FloatCoordinate top );

    FloatCoordinate getLeft() const;
    FloatCoordinate getBottom() const;
    FloatCoordinate getRight() const;
    FloatCoordinate getTop() const;

    static void transformXY( Canvas* canvas,
                             FloatCoordinate& left,
                             FloatCoordinate& bottom,
                             FloatCoordinate& right,
                             FloatCoordinate& top );

  private:

    FloatCoordinate _beginX;
    FloatCoordinate _endX;
    FloatCoordinate _beginY;
    FloatCoordinate _endY;

};


}


#endif
