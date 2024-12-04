#ifndef _gkg_graphics_X11_core_LRMarker_h_
#define _gkg_graphics_X11_core_LRMarker_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Color;


// mark text region
class LRMarker : public MonoGlyph
{

  public:

    LRMarker( Glyph* body, const Color* overlay, const Color* underlay );
    virtual ~LRMarker();

    void setBound( FloatCoordinate left, FloatCoordinate bottom,
                   FloatCoordinate right, FloatCoordinate top );
    void mark( FloatCoordinate x1, FloatCoordinate y1,
               FloatCoordinate h1, FloatCoordinate x2,
               FloatCoordinate y2, FloatCoordinate h2 );
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
    bool _bounded;
    FloatCoordinate _left, _right, _bottom, _top;
    FloatCoordinate _x1, _y1, _h1;
    FloatCoordinate _x2, _y2, _h2;

};


}


#endif
