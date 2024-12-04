#include <gkg-graphics-X11-core/BackwardScroller.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Adjustable.h>


gkg::BackwardScroller::BackwardScroller( gkg::Glyph* glyph,
                                         gkg::Style* style,
                                         gkg::TellTaleState* tellTaleState,
                                         gkg::Adjustable* adjustable,
                                         gkg::DimensionName dimensionName )
                      : gkg::Stepper( glyph, style, tellTaleState ),
                        _adjustable( adjustable ),
                        _dimensionName( dimensionName )
{
} 



gkg::BackwardScroller::~BackwardScroller()
{
}


void gkg::BackwardScroller::adjust()
{

  _adjustable->scrollBackward( _dimensionName );

}
