#ifndef _gkg_graphics_X11_core_Discretionary_h_
#define _gkg_graphics_X11_core_Discretionary_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Discretionary : public Glyph
{

  public:

    Discretionary( int32_t penalty, Glyph* glyph );
    Discretionary( int32_t penalty, Glyph* no, Glyph* pre, Glyph* in, 
                   Glyph* post );
    virtual ~Discretionary();

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual Glyph* compose( Glyph::BreakType breakType );

  private:

    int32_t _penalty;
    Glyph* _noBreak;
    Glyph* _preBreak;
    Glyph* _inBreak;
    Glyph* _postBreak;

};


}


#endif
