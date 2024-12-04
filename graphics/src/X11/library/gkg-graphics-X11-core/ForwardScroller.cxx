#include <gkg-graphics-X11-core/ForwardScroller.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Adjustable.h>


gkg::ForwardScroller::ForwardScroller( gkg::Glyph* glyph,
                                       gkg::Style* style,
                                       gkg::TellTaleState* tellTaleState,
                                       gkg::Adjustable* adjustable,
                                       gkg::DimensionName dimensionName )
                     : gkg::Stepper( glyph, style, tellTaleState ),
                       _adjustable( adjustable ),
                       _dimensionName( dimensionName )
{
} 



gkg::ForwardScroller::~ForwardScroller()
{
}


void gkg::ForwardScroller::adjust()
{

  _adjustable->scrollForward( _dimensionName );

}
