#ifndef _gkg_graphics_X11_core_TransformFitter_h_
#define _gkg_graphics_X11_core_TransformFitter_h_


#include <gkg-graphics-X11-core/TransformSetter.h>


namespace gkg
{


class TransformFitter : public TransformSetter
{

  public:

    TransformFitter( Glyph* glyph );
    virtual ~TransformFitter();

  protected:

    virtual void transform( Transformer& transformer,
                            const GeometryAllocation& geometryAllocation,
                            const GeometryAllocation& natural ) const;

};


}


#endif
