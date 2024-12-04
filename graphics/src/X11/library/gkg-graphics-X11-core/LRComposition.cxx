#include <gkg-graphics-X11-core/LRComposition.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


gkg::LRComposition::LRComposition( gkg::Glyph* context,
                                   gkg::Compositor* compositor,
                                   gkg::Glyph* separator,
                                   gkg::FloatCoordinate width,
                                   gkg::FloatCoordinate stretch,
                                   gkg::FloatCoordinate shrink,
                                   gkg::Glyph::Index size )
                    : gkg::Composition( context,
                                        compositor,
                                        separator,
                                        gkg::DimensionX,
                                        width,
                                        stretch,
                                        shrink,
                                        size )
{
}


gkg::LRComposition::~LRComposition()
{
}


gkg::Glyph* gkg::LRComposition::make( gkg::Break& theBreak )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::Glyph* glyph = layoutFactory.createHBox();
  glyph->append( layoutFactory.createHGlue( theBreak.getBegin(), 0, 0, 0.0 ) );
  if ( theBreak.getFirst() > 0 )
  {

    gkg::Glyph* g = getComponent( theBreak.getFirst() - 1 );
    if ( g )
    {

      glyph->append( g->compose( gkg::Glyph::PostBreak ) );

    }
    else
    {

      glyph->append( 0 );

    }

  }
  else
  {

    glyph->append( 0 );

  }

  for ( gkg::Glyph::Index k = theBreak.getFirst();
        k <= theBreak.getLast(); k++ )
  {

    gkg::Glyph* g = getComponent( k );
    if ( g )
    {

      glyph->append( g->compose( gkg::Glyph::NoBreak ) );

    }
    else
    {

      glyph->append( 0 );

    }

  }
  if ( theBreak.getLast() < getCount() - 1 )
  {

    gkg::Glyph* g = getComponent( theBreak.getLast() + 1 );
    if ( g )
    {

      glyph->append( g->compose( gkg::Glyph::PreBreak ) );

    }
    else
    {

      glyph->append( 0 );

    }


  }
  else
  {

    glyph->append( 0 );

  }
  glyph->append( layoutFactory.createHGlue( theBreak.getEnd(), 0, 0, 1.0 ) );
  return glyph;

}

