#ifndef _gkg_graphics_X11_core_XYMarker_h_
#define _gkg_graphics_X11_core_XYMarker_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Color;


// mark rectangular region
class XYMarker : public MonoGlyph
{

  public:

    XYMarker( Glyph* body, const Color* overlay, const Color* underlay );
    virtual ~XYMarker();

    void mark( FloatCoordinate left,
               FloatCoordinate bottom,
               FloatCoordinate right,
               FloatCoordinate top );
    void unmark();

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void undraw();

  private:

    Canvas* _canvas;
    const Color* _overlay;
    const Color* _underlay;
    bool _marked;
    FloatCoordinate _left;
    FloatCoordinate _right;
    FloatCoordinate _top;
    FloatCoordinate _bottom;

};


}


#endif
