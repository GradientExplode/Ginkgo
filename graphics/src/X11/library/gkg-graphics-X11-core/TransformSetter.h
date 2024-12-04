#ifndef _gkg_graphics_X11_core_TransformSetter_h_
#define _gkg_graphics_X11_core_TransformSetter_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-graphics-X11-core/Transformer.h>


namespace gkg
{


class TransformSetter : public MonoGlyph
{

  public:

    TransformSetter( Glyph* glyph );
    TransformSetter( Glyph* glyph, const Transformer& transformer);
    virtual ~TransformSetter();

    virtual const Transformer& getTransformer() const;
    virtual Transformer& getTransformer();
    virtual void setTransformer( const Transformer& transformer );

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

  protected:

    virtual void transform( Transformer& transformer,
                            const GeometryAllocation& geometryAllocation,
                            const GeometryAllocation& natural ) const;

  private:

    void pushTransform( Canvas* canvas,
                        const GeometryAllocation& geometryAllocation,
                        const GeometryAllocation& natural ) const;

    Transformer _transformer;
    GeometryAllocation _natural;

};



}


#endif
