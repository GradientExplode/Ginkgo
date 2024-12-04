#include <gkg-graphics-X11-LF-Motif/MotifForeground.h>
#include <gkg-graphics-X11-core/Canvas.h>


gkg::MotifForeground::MotifForeground( gkg::Glyph* glyph,
                                          const gkg::Color* color )
                        : gkg::MonoGlyph( glyph ),
                          _foreground( color )
{

  if ( _foreground )
  {

    _foreground->addReference();

  } 

}

gkg::MotifForeground::~MotifForeground()
{

  if ( _foreground )
  {

    _foreground->removeReference();

  } 

}


void 
gkg::MotifForeground::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  this->gkg::MonoGlyph::draw( canvas, geometryAllocation );
  canvas->fillRectangle( geometryAllocation.getLeft(),
                         geometryAllocation.getBottom(),
                         geometryAllocation.getRight(),
                         geometryAllocation.getTop(),
                         _foreground );

}

