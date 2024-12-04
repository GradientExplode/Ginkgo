#ifndef _gkg_graphics_X11_GeometryRequirement_h_
#define _gkg_graphics_X11_GeometryRequirement_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class GeometryRequirement
{

  public:

    GeometryRequirement();
    GeometryRequirement( const GeometryRequirement& other );
    GeometryRequirement( FloatCoordinate natural );
    GeometryRequirement( FloatCoordinate natural,
                         FloatCoordinate stretch,
                         FloatCoordinate shrink,
                         float alignment );
    GeometryRequirement( FloatCoordinate naturalLead,
                         FloatCoordinate maximumLead,
                         FloatCoordinate minimumLead,
                         FloatCoordinate naturalTrail,
                         FloatCoordinate maximumTrail,
                         FloatCoordinate minimumTrail );
                         
    GeometryRequirement& operator = ( const GeometryRequirement& other );

    bool isEqualTo( const GeometryRequirement& other, float epsilon ) const;
    bool isDefined() const;

    void setNatural( FloatCoordinate natural );
    FloatCoordinate getNatural() const;

    void setStretch( FloatCoordinate stretch );
    FloatCoordinate getStretch() const;

    void setShrink( FloatCoordinate shrink );
    FloatCoordinate getShrink() const;

    void setAlignment( float alignment );
    float getAlignment() const;

  private:

    FloatCoordinate _natural;
    FloatCoordinate _stretch;
    FloatCoordinate _shrink;
    float _alignment;

};


}


#endif
