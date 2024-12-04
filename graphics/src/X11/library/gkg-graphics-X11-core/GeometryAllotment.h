#ifndef _gkg_graphics_X11_core_GeometryAllotment_h_
#define _gkg_graphics_X11_core_GeometryAllotment_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class GeometryAllotment
{

  public:

    GeometryAllotment();
    GeometryAllotment( const GeometryAllotment& other );
    GeometryAllotment( FloatCoordinate origin,
                       FloatCoordinate span,
                       float alignment );
                       
    GeometryAllotment& operator = ( const GeometryAllotment& other );

    bool isEqualTo( const GeometryAllotment& other, float epsilon ) const;

    void setOrigin( FloatCoordinate origin );
    void addOffset( FloatCoordinate offset );
    FloatCoordinate getOrigin() const;

    void setSpan( FloatCoordinate span );
    FloatCoordinate getSpan() const;

    void setAlignment( float alignment );
    float getAlignment() const;

    FloatCoordinate getBegin() const;
    FloatCoordinate getEnd() const;

  private:

    FloatCoordinate _origin;
    FloatCoordinate _span;
    float _alignment;

};


}


#endif
