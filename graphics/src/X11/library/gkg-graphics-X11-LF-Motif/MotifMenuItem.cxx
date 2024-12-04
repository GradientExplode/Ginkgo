#include <gkg-graphics-X11-LF-Motif/MotifMenuItem.h>
#include <gkg-graphics-X11-core/Canvas.h>


gkg::MotifMenuItem::MotifMenuItem( gkg::Glyph* glyph,
                                   gkg::TellTaleState* tellTaleState )
                   : gkg::MonoGlyph( glyph ) ,
                     _tellTaleState( tellTaleState )
{

  if ( _tellTaleState )
  {

    _tellTaleState->addReference();

  }

}


gkg::MotifMenuItem::~MotifMenuItem()
{

  if ( _tellTaleState )
  {

    _tellTaleState->removeReference();

  }

}


void 
gkg::MotifMenuItem::draw(
                       gkg::Canvas* canvas,
                       const gkg::GeometryAllocation& geometryAllocation ) const
{

  if ( _tellTaleState->test( gkg::TellTaleState::Chosen ) )
  {

    this->gkg::MonoGlyph::draw( canvas, geometryAllocation );

  }

}


void 
gkg::MotifMenuItem::pick( gkg::Canvas* canvas,
                          const gkg::GeometryAllocation& geometryAllocation,
                          int32_t depth,
                          gkg::Hit& hit )
{

  if ( _tellTaleState->test( gkg::TellTaleState::Chosen ) )
  {

    this->gkg::MonoGlyph::pick( canvas, geometryAllocation, depth, hit );

  }

}
