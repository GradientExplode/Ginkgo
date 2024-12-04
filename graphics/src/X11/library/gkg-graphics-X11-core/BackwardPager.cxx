#include <gkg-graphics-X11-core/BackwardPager.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Adjustable.h>


gkg::BackwardPager::BackwardPager( gkg::Glyph* glyph,
                                   gkg::Style* style,
                                   gkg::TellTaleState* tellTaleState,
                                   gkg::Adjustable* adjustable,
                                   gkg::DimensionName dimensionName )
                   : gkg::Stepper( glyph, style, tellTaleState ),
                     _adjustable( adjustable ),
                     _dimensionName( dimensionName )
{
} 



gkg::BackwardPager::~BackwardPager()
{
}


void gkg::BackwardPager::adjust()
{

  _adjustable->pageBackward( _dimensionName );

}
