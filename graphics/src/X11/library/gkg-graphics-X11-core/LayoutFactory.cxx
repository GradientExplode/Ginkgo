#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/Layout.h>
#include <gkg-graphics-X11-core/Box.h>
#include <gkg-graphics-X11-core/Superpose.h>
#include <gkg-graphics-X11-core/Tile.h>
#include <gkg-graphics-X11-core/TileReversed.h>
#include <gkg-graphics-X11-core/TileFirstAligned.h>
#include <gkg-graphics-X11-core/TileReversedFirstAligned.h>
#include <gkg-graphics-X11-core/Align.h>
#include <gkg-graphics-X11-core/LRScrollBox.h>
#include <gkg-graphics-X11-core/LRAutoScrollBox.h>
#include <gkg-graphics-X11-core/TBScrollBox.h>
#include <gkg-graphics-X11-core/TBAutoScrollBox.h>
#include <gkg-graphics-X11-core/Deck.h>
#include <gkg-graphics-X11-core/LayoutLayer.h>
#include <gkg-graphics-X11-core/Glue.h>
#include <gkg-graphics-X11-core/ShapeOf.h>
#include <gkg-graphics-X11-core/Discretionary.h>
#include <gkg-graphics-X11-core/Strut.h>
#include <gkg-graphics-X11-core/HStrut.h>
#include <gkg-graphics-X11-core/VStrut.h>
#include <gkg-graphics-X11-core/Font.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Space.h>
#include <gkg-graphics-X11-core/Placement.h>
#include <gkg-graphics-X11-core/CenterLayout.h>
#include <gkg-graphics-X11-core/FixedLayout.h>
#include <gkg-graphics-X11-core/VariableLayout.h>
#include <gkg-graphics-X11-core/NaturalLayout.h>
#include <gkg-graphics-X11-core/MarginLayout.h>


gkg::LayoutFactory::LayoutFactory()
{
}


gkg::LayoutFactory::~LayoutFactory()
{
}



gkg::PolyGlyph* gkg::LayoutFactory::createBox( gkg::Layout* layout ) const
{

  return new gkg::Box( layout );

}


gkg::PolyGlyph* 
gkg::LayoutFactory::createHBox( gkg::Glyph* g1, gkg::Glyph* g2,
                                gkg::Glyph* g3, gkg::Glyph* g4,
                                gkg::Glyph* g5, gkg::Glyph* g6,
                                gkg::Glyph* g7, gkg::Glyph* g8,
                                gkg::Glyph* g9, gkg::Glyph* g10 ) const
{

  return gkg::LayoutFactory::add( new gkg::Box(
                                    new gkg::Superpose(
                                      new gkg::Tile( gkg::DimensionX ),
                                      new gkg::Align( gkg::DimensionY )
                                                       )
                                               ),
                                  g1, g2, g3, g4, g5, g6, g7, g8, g9, g10 );

}


gkg::PolyGlyph* 
gkg::LayoutFactory::createVBox( gkg::Glyph* g1, gkg::Glyph* g2,
                                gkg::Glyph* g3, gkg::Glyph* g4,
                                gkg::Glyph* g5, gkg::Glyph* g6,
                                gkg::Glyph* g7, gkg::Glyph* g8,
                                gkg::Glyph* g9, gkg::Glyph* g10 ) const
{

  return gkg::LayoutFactory::add( new gkg::Box(
                                    new gkg::Superpose(
                                      new gkg::TileReversed( gkg::DimensionY ),
                                      new gkg::Align( gkg::DimensionX )
                                                       )
                                               ),
                                  g1, g2, g3, g4, g5, g6, g7, g8, g9, g10 );

}


gkg::PolyGlyph* 
gkg::LayoutFactory::createHBoxFirstAligned(
	                                 gkg::Glyph* g1, gkg::Glyph* g2,
                                         gkg::Glyph* g3, gkg::Glyph* g4,
                                         gkg::Glyph* g5, gkg::Glyph* g6,
                                         gkg::Glyph* g7, gkg::Glyph* g8,
                                         gkg::Glyph* g9, gkg::Glyph* g10 ) const
{

  return gkg::LayoutFactory::add( new gkg::Box(
                                    new gkg::Superpose(
                                      new gkg::TileFirstAligned(
                                                              gkg::DimensionX ),
                                      new gkg::Align( gkg::DimensionY )
                                                       )
                                               ),
                                  g1, g2, g3, g4, g5, g6, g7, g8, g9, g10 );

}


gkg::PolyGlyph* gkg::LayoutFactory::createVBoxFirstAligned(
	                                 gkg::Glyph* g1, gkg::Glyph* g2,
                                         gkg::Glyph* g3, gkg::Glyph* g4,
                                         gkg::Glyph* g5, gkg::Glyph* g6,
                                         gkg::Glyph* g7, gkg::Glyph* g8,
                                         gkg::Glyph* g9, gkg::Glyph* g10 ) const
{

  return gkg::LayoutFactory::add( new gkg::Box(
                                    new gkg::Superpose(
                                      new gkg::TileReversedFirstAligned(
                                                              gkg::DimensionY ),
                                      new gkg::Align( gkg::DimensionX )
                                                       )
                                               ),
                                  g1, g2, g3, g4, g5, g6, g7, g8, g9, g10 );

}


gkg::ScrollBox* gkg::LayoutFactory::createHScrollBox( bool shrinkable ) const
{

  return new gkg::LRScrollBox( shrinkable );

}


gkg::LRAutoScrollBox* 
gkg::LayoutFactory::createHAutoScrollBox( bool shrinkable ) const
{

  return new gkg::LRAutoScrollBox( shrinkable );

}


gkg::ScrollBox* gkg::LayoutFactory::createVScrollBox( bool shrinkable ) const
{

  return new gkg::TBScrollBox( shrinkable );

}


gkg::TBAutoScrollBox* 
gkg::LayoutFactory::createVAutoScrollBox( bool shrinkable ) const
{

  return new gkg::TBAutoScrollBox( shrinkable );

}


gkg::PolyGlyph* gkg::LayoutFactory::createOverlay(
	                                 gkg::Glyph* g1, gkg::Glyph* g2,
                                         gkg::Glyph* g3, gkg::Glyph* g4,
                                         gkg::Glyph* g5, gkg::Glyph* g6,
                                         gkg::Glyph* g7, gkg::Glyph* g8,
                                         gkg::Glyph* g9, gkg::Glyph* g10 ) const
{

  return gkg::LayoutFactory::add( new gkg::Box(
                                    new gkg::Superpose(
                                      new gkg::Align( gkg::DimensionX ),
                                      new gkg::Align( gkg::DimensionY )
                                                      )
                                               ),
                                  g1, g2, g3, g4, g5, g6, g7, g8, g9, g10 );

}


gkg::Deck* gkg::LayoutFactory::createDeck(
                                         gkg::Glyph* g1, gkg::Glyph* g2,
                                         gkg::Glyph* g3, gkg::Glyph* g4,
                                         gkg::Glyph* g5, gkg::Glyph* g6,
                                         gkg::Glyph* g7, gkg::Glyph* g8,
                                         gkg::Glyph* g9, gkg::Glyph* g10 ) const
{

  gkg::Deck* deck = new gkg::Deck();
  gkg::LayoutFactory::add( deck, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10 );
  return deck;

}


gkg::MonoGlyph*
gkg::LayoutFactory::createBack( gkg::Glyph* glyph, gkg::Glyph* under ) const
{

  return new gkg::LayoutLayer( glyph, under, 0 );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createFront( gkg::Glyph* glyph, gkg::Glyph* over ) const
{

  return new gkg::LayoutLayer( glyph, 0, over );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createBetween( gkg::Glyph* glyph,
                                   gkg::Glyph* under,
                                   gkg::Glyph* over) const
{

  return new gkg::LayoutLayer( glyph, under, over );

}


gkg::Glyph* 
gkg::LayoutFactory::createGlue( gkg::DimensionName dimensionName,
                                gkg::FloatCoordinate natural,
                                gkg::FloatCoordinate stretch,
                                gkg::FloatCoordinate shrink,
	                        float alignment ) const
{

  return new gkg::Glue( dimensionName, natural, stretch, shrink, alignment );

}


gkg::Glyph* 
gkg::LayoutFactory::createGlue( const gkg::GeometryRequisition&
                                                     geometryRequisition ) const
{

  return new gkg::Glue( geometryRequisition );

}


gkg::Glyph* gkg::LayoutFactory::createHGlue() const
{

  return new gkg::Glue( gkg::DimensionX, 0, fil, 0, 0.0 );

}


gkg::Glyph* 
gkg::LayoutFactory::createHGlue( gkg::FloatCoordinate natural ) const
{

  return new gkg::Glue( gkg::DimensionX, natural, fil, 0, 0.0 );

}


gkg::Glyph* 
gkg::LayoutFactory::createHGlue( gkg::FloatCoordinate natural,
                                 gkg::FloatCoordinate stretch,
                                 gkg::FloatCoordinate shrink ) const
{

  return new gkg::Glue( gkg::DimensionX, natural, stretch, shrink, 0.0 );

}


gkg::Glyph* 
gkg::LayoutFactory::createHGlue( gkg::FloatCoordinate natural,
                                 gkg::FloatCoordinate stretch,
                                 gkg::FloatCoordinate shrink,
                                 float alignment ) const
{

  return new gkg::Glue( gkg::DimensionX, natural, stretch, shrink, alignment );

}


gkg::Glyph*
gkg::LayoutFactory::createHSpace( gkg::FloatCoordinate natural ) const
{

  return new gkg::Glue( gkg::DimensionX, natural, 0, 0, 0.0 );

}


gkg::Glyph* gkg::LayoutFactory::createVGlue() const
{

  return new gkg::Glue( gkg::DimensionY, 0, fil, 0, 0.0 );

}


gkg::Glyph* 
gkg::LayoutFactory::createVGlue( gkg::FloatCoordinate natural ) const
{

  return new gkg::Glue( gkg::DimensionY, natural, fil, 0, 0.0 );

}


gkg::Glyph* 
gkg::LayoutFactory::createVGlue( gkg::FloatCoordinate natural,
                                 gkg::FloatCoordinate stretch,
                                 gkg::FloatCoordinate shrink ) const
{

  return new gkg::Glue( gkg::DimensionY, natural, stretch, shrink, 0.0 );

}


gkg::Glyph* 
gkg::LayoutFactory::createVGlue( gkg::FloatCoordinate natural,
                                 gkg::FloatCoordinate stretch,
                                 gkg::FloatCoordinate shrink,
                                 float alignment ) const
{

  return new gkg::Glue( gkg::DimensionY, natural, stretch, shrink, alignment );

}


gkg::Glyph* 
gkg::LayoutFactory::createVSpace( gkg::FloatCoordinate natural ) const
{

  return new gkg::Glue( gkg::DimensionY, natural, 0, 0, 0.0 );

}

gkg::Glyph* gkg::LayoutFactory::createShapeOf( gkg::Glyph* glyph ) const
{

  return new gkg::ShapeOf( glyph, glyph );

}


gkg::Glyph* gkg::LayoutFactory::createShapeOfXY( gkg::Glyph* x,
                                                 gkg::Glyph* y ) const
{

  return new gkg::ShapeOf( x, y );

}


gkg::Glyph*
gkg::LayoutFactory::createDiscretionary( int32_t penalty, 
                                         gkg::Glyph* glyph ) const
{

  return new gkg::Discretionary( penalty, glyph );

}


gkg::Glyph* 
gkg::LayoutFactory::createDiscretionary( int32_t penalty,
                                         gkg::Glyph* no,
                                         gkg::Glyph* before,
                                         gkg::Glyph* in,
                                         gkg::Glyph* after ) const
{

  return new gkg::Discretionary( penalty, no, before, in, after );

}


gkg::Glyph* 
gkg::LayoutFactory::createStrut( const gkg::Font* font,
                                 gkg::FloatCoordinate natural,
                                 gkg::FloatCoordinate stretch,
                                 gkg::FloatCoordinate shrink ) const
{

  return new gkg::Strut( font, natural, stretch, shrink );

}


gkg::Glyph* 
gkg::LayoutFactory::createHStrut( gkg::FloatCoordinate rightBearing,
                                  gkg::FloatCoordinate leftBearing,
                                  gkg::FloatCoordinate natural,
                                  gkg::FloatCoordinate stretch,
                                  gkg::FloatCoordinate shrink ) const
{

  return new gkg::HStrut( rightBearing, leftBearing, natural, stretch, shrink );

}


gkg::Glyph* 
gkg::LayoutFactory::createVStrut( gkg::FloatCoordinate ascent,
                                  gkg::FloatCoordinate descent,
                                  gkg::FloatCoordinate natural,
                                  gkg::FloatCoordinate stretch,
                                  gkg::FloatCoordinate shrink ) const
{

  return new gkg::VStrut( ascent, descent, natural, stretch, shrink );

}


gkg::Glyph* 
gkg::LayoutFactory::createSpaces( int32_t count,
                                  gkg::FloatCoordinate each,
                                  const gkg::Font* font,
                                  const gkg::Color* color ) const
{

  return new gkg::Space( count, each, font, color );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createCenter( gkg::Glyph* glyph,
                                  float xAlign, float yAlign ) const
{

  return new gkg::Placement(
                          glyph,
                          new gkg::Superpose(
                            new gkg::CenterLayout( gkg::DimensionX, xAlign ),
                            new gkg::CenterLayout( gkg::DimensionY, yAlign) ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createCenterDimension( gkg::Glyph* glyph,
                                           gkg::DimensionName dimensionName,
                                           float align ) const
{

  return new gkg::Placement( glyph, new gkg::CenterLayout( dimensionName, 
                                                           align ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createHCenter( gkg::Glyph* glyph, float x ) const
{

  return createCenterDimension( glyph, gkg::DimensionX, x );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createVCenter( gkg::Glyph* glyph, float y ) const
{

  return createCenterDimension( glyph, gkg::DimensionY, y );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createFixed( gkg::Glyph* glyph,
                                 gkg::FloatCoordinate x,
                                 gkg::FloatCoordinate y ) const
{

  return new gkg::Placement( glyph,
                             new gkg::Superpose(
                               new gkg::FixedLayout( gkg::DimensionX, x ),
                               new gkg::FixedLayout( gkg::DimensionY, y ) ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createFixedDimension( gkg::Glyph* glyph,
                                          gkg::DimensionName dimensionName,
                                          gkg::FloatCoordinate span ) const
{

  return new gkg::Placement( glyph, new gkg::FixedLayout( dimensionName, 
                                                          span ) );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createHFixed( gkg::Glyph* glyph, 
                                  gkg::FloatCoordinate x ) const
{

  return createFixedDimension( glyph, gkg::DimensionX, x );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createVFixed( gkg::Glyph* glyph,
                                  gkg::FloatCoordinate y ) const
{

  return createFixedDimension( glyph, gkg::DimensionY, y );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createFlexible( gkg::Glyph* glyph,
                                    gkg::FloatCoordinate stretch,
                                    gkg::FloatCoordinate shrink ) const
{

  return new gkg::Placement(
              glyph,
              new gkg::Superpose(
                new gkg::VariableLayout( gkg::DimensionX, stretch, shrink ),
                new gkg::VariableLayout( gkg::DimensionY, stretch, shrink ) ) );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createFlexibleDimension( gkg::Glyph* glyph,
                                             gkg::DimensionName dimensionName,
                                             gkg::FloatCoordinate stretch,
                                             gkg::FloatCoordinate shrink ) const
{

  return new gkg::Placement( glyph,
                             new gkg::VariableLayout( dimensionName,
                                                      stretch, shrink ) );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createHFlexible( gkg::Glyph* glyph,
                                     gkg::FloatCoordinate stretch,
                                     gkg::FloatCoordinate shrink ) const
{

  return createFlexibleDimension( glyph, gkg::DimensionX, stretch, shrink );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createVFlexible( gkg::Glyph* glyph,
                                     gkg::FloatCoordinate stretch,
                                     gkg::FloatCoordinate shrink ) const
{

  return createFlexibleDimension( glyph, gkg::DimensionY, stretch, shrink );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createNatural( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate x,
                                   gkg::FloatCoordinate y ) const
{

  return new gkg::Placement( glyph,
                             new gkg::Superpose(
                               new gkg::NaturalLayout( gkg::DimensionX, x ),
                               new gkg::NaturalLayout( gkg::DimensionY, y ) ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createNaturalDimension( gkg::Glyph* glyph,
                                            gkg::DimensionName dimensionName,
                                            gkg::FloatCoordinate span ) const
{

  return new gkg::Placement( glyph, new gkg::NaturalLayout( dimensionName,
                                                            span ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createHNatural( gkg::Glyph* glyph,
                                    gkg::FloatCoordinate x ) const
{

  return createNaturalDimension( glyph, gkg::DimensionX, x );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createVNatural( gkg::Glyph* glyph,
                                    gkg::FloatCoordinate y ) const
{

  return createNaturalDimension( glyph, gkg::DimensionY, y );

}



gkg::MonoGlyph* 
gkg::LayoutFactory::createMargin( gkg::Glyph* glyph,
                                  gkg::FloatCoordinate size ) const
{

  return new gkg::Placement( glyph, new gkg::MarginLayout( size ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createMargin( gkg::Glyph* glyph,
                                  gkg::FloatCoordinate hmargin,
                                  gkg::FloatCoordinate vmargin ) const
{

  return new gkg::Placement( glyph, new gkg::MarginLayout( hmargin, vmargin ) );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createMargin( gkg::Glyph* glyph,
	                          gkg::FloatCoordinate lmargin,
                                  gkg::FloatCoordinate rmargin,
                                  gkg::FloatCoordinate bmargin,
                                  gkg::FloatCoordinate tmargin ) const
{

  return new gkg::Placement( glyph, new gkg::MarginLayout( lmargin,
                                                           rmargin,
                                                           bmargin,
                                                           tmargin ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createMargin( gkg::Glyph* glyph,
                                  gkg::FloatCoordinate lmargin,
                                  gkg::FloatCoordinate lstretch,
                                  gkg::FloatCoordinate lshrink,
                                  gkg::FloatCoordinate rmargin,
                                  gkg::FloatCoordinate rstretch,
                                  gkg::FloatCoordinate rshrink,
                                  gkg::FloatCoordinate bmargin,
                                  gkg::FloatCoordinate bstretch,
                                  gkg::FloatCoordinate bshrink,
                                  gkg::FloatCoordinate tmargin,
                                  gkg::FloatCoordinate tstretch,
                                  gkg::FloatCoordinate tshrink ) const
{

  return new gkg::Placement(
                          glyph,
                          new gkg::MarginLayout( lmargin, lstretch, lshrink,
                                                 rmargin, rstretch, rshrink,
                                                 bmargin, bstretch, bshrink,
                                                 tmargin, tstretch, tshrink ) );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createHMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size ) const
{

  return createMargin( glyph, size, size, 0, 0 );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createHMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate lmargin,
                                   gkg::FloatCoordinate rmargin ) const
{

  return createMargin( glyph, lmargin, rmargin, 0, 0 );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createHMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate lmargin,
                                   gkg::FloatCoordinate lstretch,
                                   gkg::FloatCoordinate lshrink,
                                   gkg::FloatCoordinate rmargin,
                                   gkg::FloatCoordinate rstretch,
                                   gkg::FloatCoordinate rshrink ) const
{

  return createMargin( glyph,
                       lmargin, lstretch, lshrink, rmargin, rstretch, rshrink,
                       0, 0, 0, 0, 0, 0 );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createVMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size ) const
{

  return createMargin( glyph, 0, 0, size, size );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createVMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate bmargin,
                                   gkg::FloatCoordinate tmargin ) const
{

  return createMargin( glyph, 0, 0, bmargin, tmargin );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createVMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate bmargin,
                                   gkg::FloatCoordinate bstretch,
                                   gkg::FloatCoordinate bshrink,
                                   gkg::FloatCoordinate tmargin,
                                   gkg::FloatCoordinate tstretch,
                                   gkg::FloatCoordinate tshrink ) const
{

  return createMargin( glyph,
                       0, 0, 0, 0, 0, 0,
                       bmargin, bstretch, bshrink, tmargin, tstretch, tshrink );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createLMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size ) const
{

  return createMargin( glyph, size, 0, 0, 0 );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createLMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size,
                                   gkg::FloatCoordinate stretch,
                                   gkg::FloatCoordinate shrink ) const
{

  return createMargin( glyph, size, stretch, shrink,
                       0, 0, 0, 0, 0, 0, 0, 0, 0 );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createRMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size ) const
{

  return createMargin( glyph, 0, size, 0, 0 );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createRMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size,
                                   gkg::FloatCoordinate stretch,
                                   gkg::FloatCoordinate shrink ) const
{

  return createMargin( glyph, 0, 0, 0, size, stretch, shrink,
                       0, 0, 0, 0, 0, 0 );

}


gkg::MonoGlyph* 
gkg::LayoutFactory::createBMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size ) const
{

  return createMargin( glyph, 0, 0, size, 0 );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createBMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size,
                                   gkg::FloatCoordinate stretch,
                                   gkg::FloatCoordinate shrink ) const
{

  return createMargin( glyph, 0, 0, 0, 0, 0, 0,
                       size, stretch, shrink, 0, 0, 0 );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createTMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size ) const
{

  return createMargin( glyph, 0, 0, 0, size );

}


gkg::MonoGlyph*
gkg::LayoutFactory::createTMargin( gkg::Glyph* glyph,
                                   gkg::FloatCoordinate size,
                                   gkg::FloatCoordinate stretch,
                                   gkg::FloatCoordinate shrink ) const
{

  return createMargin( glyph, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       size, stretch, shrink );

}


gkg::PolyGlyph* gkg::LayoutFactory::add( gkg::PolyGlyph* polyGlyph,
                                         gkg::Glyph* g1, gkg::Glyph* g2,
                                         gkg::Glyph* g3, gkg::Glyph* g4,
                                         gkg::Glyph* g5, gkg::Glyph* g6,
                                         gkg::Glyph* g7, gkg::Glyph* g8,
                                         gkg::Glyph* g9, gkg::Glyph* g10 )
{

  if ( g1 )
  {

    polyGlyph->append( g1 );

  }
  if ( g2 )
  {

    polyGlyph->append( g2 );

  }
  if ( g3 )
  {

    polyGlyph->append( g3 );

  }
  if ( g4 )
  {

    polyGlyph->append( g4 );

  }
  if ( g5 )
  {

    polyGlyph->append( g5 );

  }
  if ( g6 )
  {

    polyGlyph->append( g6 );

  }
  if ( g7 )
  {

    polyGlyph->append( g7 );

  }
  if ( g8 )
  {

    polyGlyph->append( g8 );

  }
  if ( g9 )
  {

    polyGlyph->append( g9 );

  }
  if ( g10 )
  {

    polyGlyph->append( g10 );

  }
  return polyGlyph;

}
