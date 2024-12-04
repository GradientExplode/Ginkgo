#ifndef _gkg_gui_core_Label_h_
#define _gkg_gui_core_Label_h_


#include <string>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Color;
class Font;


// ASCII text glyph with font
class Label : public Glyph
{

  public:

    Label( const std::string& text, const Font* font, const Color* color);
    virtual ~Label();

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

    void computeMetrics();

    std::string _text;
    const Font* _font;
    const Color* _color;
    FloatCoordinate _left;
    FloatCoordinate _right;
    FloatCoordinate _ascent;
    FloatCoordinate _descent;
    FloatCoordinate _width;
    FloatCoordinate* _charWidths;

};


}


#endif
