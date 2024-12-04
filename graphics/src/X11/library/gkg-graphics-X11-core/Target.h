#ifndef _gkg_graphics_X11_core_Target_h_
#define _gkg_graphics_X11_core_Target_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


// pick sensitivity control
class Target : public MonoGlyph
{

  public:

    enum Sensitivity
    {

      NeverHit,
      AlwaysHit,
      PrimitiveHit,
      CharacterHit,
      BodyHit

    };

    Target( Glyph* body, Sensitivity sensitivity );
    virtual ~Target();

    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

  private:

    Sensitivity _sensitivity;

};



}


#endif
