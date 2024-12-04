#ifndef _gkg_graphics_X11_core_Patch_h_
#define _gkg_graphics_X11_core_Patch_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-graphics-X11-core/Transformer.h>


namespace gkg
{


// for repairing glyphs
class Patch : public MonoGlyph
{

  public:

    Patch( Glyph* body );
    virtual ~Patch();

    Canvas* getCanvas() const;
    const Transformer& getTransformer() const;
    const GeometryAllocation& getGeometryAllocation() const;
    const GeometryExtension& getGeometryExtension() const;

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void undraw();

    virtual void redraw() const;
    virtual void reallocate();
    virtual void repick( int32_t depth, Hit& hit );

  private:

    Canvas* _canvas;
    Transformer _transformer;
    GeometryAllocation _geometryAllocation;
    GeometryExtension _geometryExtension;

};


}


#endif
