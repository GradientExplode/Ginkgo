#include <gkg-graphics-X11-core/ForwardPager.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Adjustable.h>


gkg::ForwardPager::ForwardPager( gkg::Glyph* glyph,
                                 gkg::Style* style,
                                 gkg::TellTaleState* tellTaleState,
                                 gkg::Adjustable* adjustable,
                                 gkg::DimensionName dimensionName )
                  : gkg::Stepper( glyph, style, tellTaleState ),
                    _adjustable( adjustable ),
                    _dimensionName( dimensionName )
{
} 



gkg::ForwardPager::~ForwardPager()
{
}


void gkg::ForwardPager::adjust()
{

  _adjustable->pageForward( _dimensionName );

}
