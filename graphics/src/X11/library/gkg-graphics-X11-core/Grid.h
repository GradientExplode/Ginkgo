#ifndef _gkg_graphics_X11_core_Grid_h_
#define _gkg_graphics_X11_core_Grid_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


template < class T > class ScaleDivider;
template < class T > class WindowingTransform;
class Color;
class Brush;


class Grid : public MonoGlyph
{

  public:

    Grid( ScaleDivider< FloatCoordinate >* scaleDividerX,
          ScaleDivider< FloatCoordinate >* scaleDividerY,
          WindowingTransform< FloatCoordinate >* windowingTransformX,
          WindowingTransform< FloatCoordinate >* windowingTransformY );
    virtual ~Grid();

    virtual void setScaleDividerX(
                               ScaleDivider< FloatCoordinate >* scaleDividerX );
    ScaleDivider< FloatCoordinate >* getScaleDividerX() const;

    virtual void setScaleDividerY(
                               ScaleDivider< FloatCoordinate >* scaleDividerY );
    ScaleDivider< FloatCoordinate >* getScaleDividerY() const;

    virtual void setMajorColor( Color* color );
    virtual Color* getMajorColor() const;

    virtual void setMinorColor( Color* color );
    virtual Color* getMinorColor() const;

    virtual void setMajorBrush( Brush* brush );
    virtual Brush* getMajorBrush() const;

    virtual void setMinorBrush( Brush* brush );
    virtual Brush* getMinorBrush() const;

    virtual void enableX( bool value );
    virtual bool isXEnabled() const;

    virtual void enableY( bool value );
    virtual bool isYEnabled() const;

    virtual void enableMinorX( bool value );
    virtual bool isMinorXEnabled() const;

    virtual void enableMinorY( bool value );
    virtual bool isMinorYEnabled() const;

    virtual void setWindowingTransform(
                    DimensionType d,
                    WindowingTransform< FloatCoordinate >* windowingTransform );
    virtual WindowingTransform< FloatCoordinate >*
                                 getWindowingTransform( DimensionType d ) const;

    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;

  protected:

    ScaleDivider< FloatCoordinate >* _scaleDividerX;
    ScaleDivider< FloatCoordinate >* _scaleDividerY;
    Color* _majorColor;
    Color* _minorColor;
    Brush* _majorBrush;
    Brush* _minorBrush;
    bool _xEnabled;               // x lines enabled
    bool _yEnabled;               // y lines enabled
    bool _minorXEnabled;          // x minor lines enabled
    bool _minorYEnabled;          // y minor lines enabled
    WindowingTransform< FloatCoordinate >* _windowingTransforms[ Dimension2d ];

};


}


#endif
