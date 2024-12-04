#ifndef _gkg_graphics_X11_core_Shadow_h_
#define _gkg_graphics_X11_core_Shadow_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class Color;


// a drop shadowing Glyph
class Shadow : public MonoGlyph
{

  public:

    Shadow( Glyph* glyph,
            FloatCoordinate x, FloatCoordinate y,
            const Color* color,
            bool single = false );
    virtual ~Shadow();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void print( Printer* printer,
                        const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

  private:

    void computeGeometryRequirement( GeometryRequirement& geometryRequirement,
                                     FloatCoordinate offset ) const;
    void 
    computeGeometryAllocation( GeometryAllocation& geometryAllocation ) const;
    void computeGeometryAllotment( GeometryAllotment& geometryAllotment,
                                   FloatCoordinate offset ) const;
    void drawShadow( Canvas* canvas,
                     const GeometryAllocation& geometryAllocation ) const;
    void drawBody( Canvas* canvas,
                   const GeometryAllocation& geometryAllocation ) const;

    FloatCoordinate _offsetX;
    FloatCoordinate _offsetY;
    const Color* _color;
    bool _single;

};


}


#endif
