#include <gkg-graphics-X11-core/TBComposition.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/PolyGlyph.h>


gkg::TBComposition::TBComposition( gkg::Glyph* context,
                                   gkg::Compositor* compositor,
                                   gkg::Glyph* separator,
                                   gkg::FloatCoordinate height,
                                   gkg::FloatCoordinate stretch,
                                   gkg::FloatCoordinate shrink,
                                   gkg::Glyph::Index size )
                   : gkg::Composition( context,
                                       compositor,
                                       separator,
                                       gkg::DimensionY,
                                       height,
                                       stretch,
                                       shrink,
                                       size )
{
}


gkg::TBComposition::~TBComposition()
{
}


gkg::Glyph* gkg::TBComposition::make( gkg::Break& theBreak )
{

  gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();
  gkg::Glyph* glyph = layoutFactory.createVBox();
  glyph->append( layoutFactory.createVGlue( theBreak.getBegin(), 0, 0, 1.0 ) );
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
  glyph->append( layoutFactory.createVGlue( theBreak.getEnd(), 0, 0, 0.0 ) );
  return glyph;

}

