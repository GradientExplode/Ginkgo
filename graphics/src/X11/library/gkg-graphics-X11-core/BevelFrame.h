#ifndef _gkg_graphics_X11_core_BevelFrame_h_
#define _gkg_graphics_X11_core_BevelFrame_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


// beveled framing of a glyph (3D)
class BevelFrame : public MonoGlyph
{

  public:

    BevelFrame( Glyph* glyph,
                FloatCoordinate thickness,
                float xAlign = 0.0, float yAlign = 0.0,
	        bool hMargin = true, bool vMargin = true );
    virtual ~BevelFrame();

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

    virtual void drawFrame( Canvas* canvas,
                            const GeometryAllocation& geometryAllocation,
                            FloatCoordinate thickness ) const;

  private:

    FloatCoordinate getThickness( Canvas* canvas ) const;
    void allocateBody( Glyph* glyph, FloatCoordinate thickness,
                       GeometryAllocation& geometryAllocation ) const;

    FloatCoordinate _thickness;
    float _xAlign;
    float _yAlign;
    bool _hMargin;
    bool _vMargin;

};


}


#endif
