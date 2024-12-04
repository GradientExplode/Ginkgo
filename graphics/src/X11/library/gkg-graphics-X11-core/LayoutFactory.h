#ifndef _gkg_graphics_X11_core_LayoutFactory_h_
#define _gkg_graphics_X11_core_LayoutFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-graphics-X11-base/Coordinates.h>
#include <gkg-graphics-X11-core/Glyph.h>


namespace gkg
{


class Layout;
class MonoGlyph;
class PolyGlyph;
class ScrollBox;
class LRAutoScrollBox;
class TBAutoScrollBox;
class Deck;
class Font;
class Color;


class LayoutFactory : public Singleton< LayoutFactory >
{

  public:

    virtual ~LayoutFactory();

    virtual PolyGlyph*
              createBox(
                Layout* layout ) const;
    virtual PolyGlyph*
              createHBox(
                Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0,
                Glyph* g4 = 0, Glyph* g5 = 0, Glyph* g6 = 0,
                Glyph* g7 = 0, Glyph* g8 = 0, Glyph* g9 = 0,
                Glyph* g10 = 0 ) const;
    virtual PolyGlyph*
              createVBox(
                Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0,
                Glyph* g4 = 0, Glyph* g5 = 0, Glyph* g6 = 0,
                Glyph* g7 = 0, Glyph* g8 = 0, Glyph* g9 = 0,
                Glyph* g10 = 0 ) const;
    virtual PolyGlyph*
              createHBoxFirstAligned(
	        Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0,
                Glyph* g4 = 0, Glyph* g5 = 0, Glyph* g6 = 0,
                Glyph* g7 = 0, Glyph* g8 = 0, Glyph* g9 = 0,
                Glyph* g10 = 0 ) const;
    virtual PolyGlyph*
              createVBoxFirstAligned(
	        Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0,
                Glyph* g4 = 0, Glyph* g5 = 0, Glyph* g6 = 0,
                Glyph* g7 = 0, Glyph* g8 = 0, Glyph* g9 = 0,
                Glyph* g10 = 0 ) const;

    virtual ScrollBox*
              createHScrollBox(
                bool shrinkable = true ) const;
    virtual LRAutoScrollBox*
              createHAutoScrollBox(
                bool shrinkable = true ) const;
    virtual ScrollBox*
              createVScrollBox(
                bool shrinkable = true ) const;
    virtual TBAutoScrollBox*
              createVAutoScrollBox(
                bool shrinkable = true ) const;

    virtual PolyGlyph*
              createOverlay(
	        Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0,
                Glyph* g4 = 0, Glyph* g5 = 0, Glyph* g6 = 0,
                Glyph* g7 = 0, Glyph* g8 = 0, Glyph* g9 = 0,
                Glyph* g10 = 0 ) const;

    virtual Deck*
              createDeck(
                Glyph* g1 = 0, Glyph* g2 = 0, Glyph* g3 = 0,
                Glyph* g4 = 0, Glyph* g5 = 0, Glyph* g6 = 0,
                Glyph* g7 = 0, Glyph* g8 = 0, Glyph* g9 = 0,
                Glyph* g10 = 0 ) const;

    virtual MonoGlyph*
              createBack(
                Glyph* glyph, Glyph* under ) const;
    virtual MonoGlyph*
              createFront(
                Glyph* glyph, Glyph* over ) const;
    virtual MonoGlyph*
              createBetween(
                Glyph* glyph,
                Glyph* under,
                Glyph* over) const;

    virtual Glyph*
              createGlue(
                DimensionName dimensionName,
                FloatCoordinate natural,
                FloatCoordinate stretch,
                FloatCoordinate shrink,
	        float alignment ) const;
    virtual Glyph*
              createGlue(
                const GeometryRequisition& geometryRequisition ) const;
    virtual Glyph*
              createHGlue() const;
    virtual Glyph*
              createHGlue(
                FloatCoordinate natural ) const;
    virtual Glyph*
              createHGlue(
                FloatCoordinate natural,
                FloatCoordinate stretch,
                FloatCoordinate shrink ) const;
    virtual Glyph*
              createHGlue(
                FloatCoordinate natural,
                FloatCoordinate stretch,
                FloatCoordinate shrink,
                float alignment ) const;
    virtual Glyph*
              createHSpace(
                FloatCoordinate natural ) const;
    virtual Glyph*
              createVGlue() const;
    virtual Glyph*
              createVGlue(
                FloatCoordinate natural ) const;
    virtual Glyph*
              createVGlue(
                FloatCoordinate natural,
                FloatCoordinate stretch,
                FloatCoordinate shrink ) const;
    virtual Glyph*
              createVGlue(
                FloatCoordinate natural,
                FloatCoordinate stretch,
                FloatCoordinate shrink,
                float alignment ) const;
    virtual Glyph*
              createVSpace(
                FloatCoordinate natural ) const;

    virtual Glyph*
              createShapeOf(
                Glyph* glyph ) const;
    virtual Glyph*
              createShapeOfXY(
                Glyph* x, Glyph* y ) const;

    virtual Glyph*
              createDiscretionary(
                int32_t penalty, Glyph* glyph ) const;
    virtual Glyph*
              createDiscretionary(
                int32_t penalty,
                Glyph* no,
                Glyph* before,
                Glyph* in,
                Glyph* after ) const;

    virtual Glyph*
              createStrut(
                const Font* font,
                FloatCoordinate natural = 0,
                FloatCoordinate stretch = 0,
                FloatCoordinate shrink = 0 ) const;
    virtual Glyph*
              createHStrut(
                FloatCoordinate rightBearing,
                FloatCoordinate leftBearing = 0,
                FloatCoordinate natural = 0,
                FloatCoordinate stretch = 0,
                FloatCoordinate shrink = 0 ) const;
    virtual Glyph*
              createVStrut(
                FloatCoordinate ascent,
                FloatCoordinate descent = 0,
                FloatCoordinate natural = 0,
                FloatCoordinate stretch = 0,
                FloatCoordinate shrink = 0 ) const;

    virtual Glyph*
              createSpaces(
                int32_t count,
                FloatCoordinate each,
                const Font* font,
                const Color* color ) const;

    virtual MonoGlyph*
              createCenter(
                Glyph* glyph,
                float xAlign = 0.5,
                float yAlign = 0.5 ) const;
    virtual MonoGlyph*
              createCenterDimension(
                Glyph* glyph,
                DimensionName dimensionName,
                float align ) const;
    virtual MonoGlyph*
              createHCenter(
                Glyph* glyph, float x = 0.5 ) const;
    virtual MonoGlyph*
              createVCenter(
                Glyph* glyph, float y = 0.5 ) const;

    virtual MonoGlyph*
              createFixed(
                Glyph* glyph,
                FloatCoordinate x,
                FloatCoordinate y ) const;
    virtual MonoGlyph*
              createFixedDimension(
                Glyph* glyph,
                DimensionName dimensionName,
                FloatCoordinate span ) const;
    virtual MonoGlyph*
              createHFixed(
                Glyph* glyph, FloatCoordinate x ) const;
    virtual MonoGlyph*
              createVFixed(
                Glyph* glyph, FloatCoordinate y ) const;

    virtual MonoGlyph*
              createFlexible(
                Glyph* glyph,
                FloatCoordinate stretch = fil,
                FloatCoordinate shrink = fil ) const;
    virtual MonoGlyph*
              createFlexibleDimension(
	        Glyph* glyph,
                DimensionName dimensionName,
                FloatCoordinate stretch = fil,
                FloatCoordinate shrink = fil ) const;
    virtual MonoGlyph*
              createHFlexible(
                Glyph* glyph,
                FloatCoordinate stretch = fil,
                FloatCoordinate shrink = fil ) const;
    virtual MonoGlyph*
              createVFlexible(
                Glyph* glyph,
                FloatCoordinate stretch = fil,
                FloatCoordinate shrink = fil ) const;

    virtual MonoGlyph*
              createNatural(
                Glyph* glyph,
                FloatCoordinate x,
                FloatCoordinate y ) const;
    virtual MonoGlyph*
              createNaturalDimension(
                Glyph* glyph,
                DimensionName dimensionName,
                FloatCoordinate span ) const;
    virtual MonoGlyph*
              createHNatural(
                Glyph* glyph, FloatCoordinate x ) const;
    virtual MonoGlyph*
              createVNatural(
                Glyph* glyph, FloatCoordinate y ) const;

    virtual MonoGlyph*
              createMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createMargin(
                Glyph* glyph,
                FloatCoordinate hmargin,
                FloatCoordinate vmargin ) const;
    virtual MonoGlyph*
              createMargin(
                Glyph* glyph,
	        FloatCoordinate lmargin,
                FloatCoordinate rmargin,
                FloatCoordinate bmargin,
                FloatCoordinate tmargin ) const;
    virtual MonoGlyph*
              createMargin(
                Glyph* glyph,
                FloatCoordinate lmargin,
                FloatCoordinate lstretch,
                FloatCoordinate lshrink,
                FloatCoordinate rmargin,
                FloatCoordinate rstretch,
                FloatCoordinate rshrink,
                FloatCoordinate bmargin,
                FloatCoordinate bstretch,
                FloatCoordinate bshrink,
                FloatCoordinate tmargin,
                FloatCoordinate tstretch,
                FloatCoordinate tshrink ) const;
    virtual MonoGlyph*
              createHMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createHMargin(
                Glyph* glyph,
                FloatCoordinate lmargin,
                FloatCoordinate rmargin ) const;
    virtual MonoGlyph*
              createHMargin(
                Glyph* glyph,
                FloatCoordinate lmargin,
                FloatCoordinate lstretch,
                FloatCoordinate lshrink,
                FloatCoordinate rmargin,
                FloatCoordinate rstretch,
                FloatCoordinate rshrink ) const;
    virtual MonoGlyph*
              createVMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createVMargin(
                Glyph* glyph,
                FloatCoordinate bmargin,
                FloatCoordinate tmargin ) const;
    virtual MonoGlyph*
              createVMargin(
                Glyph* glyph,
                FloatCoordinate bmargin,
                FloatCoordinate bstretch,
                FloatCoordinate bshrink,
                FloatCoordinate tmargin,
                FloatCoordinate tstretch,
                FloatCoordinate tshrink ) const;
    virtual MonoGlyph*
              createLMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createLMargin(
                Glyph* glyph,
                FloatCoordinate size,
                FloatCoordinate stretch,
                FloatCoordinate shrink ) const;
    virtual MonoGlyph*
              createRMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createRMargin(
                Glyph* glyph,
                FloatCoordinate size,
                FloatCoordinate stretch,
                FloatCoordinate shrink ) const;
    virtual MonoGlyph*
              createBMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createBMargin(
                Glyph* glyph,
                FloatCoordinate size,
                FloatCoordinate stretch,
                FloatCoordinate shrink ) const;
    virtual MonoGlyph*
              createTMargin(
                Glyph* glyph, FloatCoordinate size ) const;
    virtual MonoGlyph*
              createTMargin(
                Glyph* glyph,
                FloatCoordinate size,
                FloatCoordinate stretch,
                FloatCoordinate shrink ) const;

  protected:

    friend class Singleton< LayoutFactory >;

    LayoutFactory();

    static PolyGlyph* add( PolyGlyph* polyGlyph,
                           Glyph* g1, Glyph* g2, Glyph* g3, Glyph* g4,
                           Glyph* g5, Glyph* g6, Glyph* g7, Glyph* g8,
                           Glyph* g9, Glyph* g10 );

};

}


#endif
