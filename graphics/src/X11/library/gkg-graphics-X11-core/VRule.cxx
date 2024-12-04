#include <gkg-graphics-X11-core/VRule.h>
#include <gkg-graphics-X11-core/Color.h>


gkg::VRule::VRule( const gkg::Color* color, gkg::FloatCoordinate thickness )
           : gkg::Rule( gkg::DimensionX, color, thickness )
{
}


gkg::VRule::~VRule()
{
}
