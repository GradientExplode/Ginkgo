#ifndef _gkg_graphics_X11_core_Bevel_h_
#define _gkg_graphics_X11_core_Bevel_h_


#include <gkg-graphics-X11-core/BevelFrame.h>


namespace gkg
{


class Color;


class Bevel : public BevelFrame
{

  public:

    typedef void ( *Beveler )( Canvas* canvas,
                               const Color* light,
                               const Color* medium,
                               const Color* dark,
                               FloatCoordinate thickness,
                               FloatCoordinate left,
                               FloatCoordinate bottom,
                               FloatCoordinate right,
                               FloatCoordinate top );

    Bevel( Glyph* glyph, Beveler beveler,
           const Color* light, const Color* medium, const Color* dark,
           FloatCoordinate thickness, float xAlign = 0.0, float yAlign = 0.0,
           bool hMargin = true, bool vMargin = true );
    virtual ~Bevel();

    virtual void drawFrame( Canvas* canvas,
                            const GeometryAllocation& geometryAllocation,
                            FloatCoordinate thickness ) const;

    static void drawRectangle( Canvas* canvas,
                               const Color* light,
                               const Color* medium,
                               const Color* dark,
                               FloatCoordinate thickness,
                               FloatCoordinate left,
                               FloatCoordinate bottom,
                               FloatCoordinate right,
                               FloatCoordinate top );
    static void drawLeftArrow( Canvas* canvas,
                               const Color* light,
                               const Color* medium,
                               const Color* dark,
                               FloatCoordinate thickness,
                               FloatCoordinate left,
                               FloatCoordinate bottom,
                               FloatCoordinate right,
                               FloatCoordinate top );
    static void drawRightArrow( Canvas* canvas,
                                const Color* light,
                                const Color* medium,
                                const Color* dark,
                                FloatCoordinate thickness,
                                FloatCoordinate left,
                                FloatCoordinate bottom,
                                FloatCoordinate right,
                                FloatCoordinate top );
    static void drawUpArrow( Canvas* canvas,
                             const Color* light,
                             const Color* medium,
                             const Color* dark,
                             FloatCoordinate thickness,
                             FloatCoordinate left,
                             FloatCoordinate bottom,
                             FloatCoordinate right,
                             FloatCoordinate top );
    static void drawDownArrow( Canvas* canvas,
                               const Color* light,
                               const Color* medium,
                               const Color* dark,
                               FloatCoordinate thickness,
                               FloatCoordinate left,
                               FloatCoordinate bottom,
                               FloatCoordinate right,
                               FloatCoordinate top );
    static void drawDiamond( Canvas* canvas,
                             const Color* light,
                             const Color* medium,
                             const Color* dark,
                             FloatCoordinate thickness,
                             FloatCoordinate left,
                             FloatCoordinate bottom,
                             FloatCoordinate right,
                             FloatCoordinate top );

  private:

    FloatCoordinate getThickness( Canvas* canvas ) const;
    void allocateBody( Glyph* glyph, FloatCoordinate thickness,
                       GeometryAllocation& geometryAllocation ) const;

    Beveler _beveler;
    const Color* _light;
    const Color* _medium;
    const Color* _dark;

};


}


#endif
