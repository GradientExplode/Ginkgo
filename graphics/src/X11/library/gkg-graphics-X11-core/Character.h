#ifndef _gkg_graphics_X11_core_Character_h_
#define _gkg_graphics_X11_core_Character_h_


#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;
class Font;


// a character glyph with font
class Character : public Glyph
{

  public:

    Character( int32_t code, const Font* font, const Color* color );
    Character( char c, const Font* font, const Color* color );
    virtual ~Character();

    virtual int32_t getCode() const;

    virtual void request( GeometryRequisition& geometryRequisition ) const;
    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );
    virtual void draw( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation ) const;
    virtual void pick( Canvas* canvas,
                       const GeometryAllocation& geometryAllocation,
                       int32_t depth,
                       Hit& hit );

  private:

    void doInitialization();

    int32_t _code;
    const Font* _font;
    const Color* _color;
    FloatCoordinate _leftBearing;
    FloatCoordinate _rightBearing;
    FloatCoordinate _ascent;
    FloatCoordinate _descent;
    FloatCoordinate _width;
    FloatCoordinate _height;
    float _alignment;

};


}


#endif
