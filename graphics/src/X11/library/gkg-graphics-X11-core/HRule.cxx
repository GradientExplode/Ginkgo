#include <gkg-graphics-X11-core/HRule.h>
#include <gkg-graphics-X11-core/Color.h>


gkg::HRule::HRule( const gkg::Color* color, gkg::FloatCoordinate thickness )
           : gkg::Rule( gkg::DimensionY, color, thickness )
{
}


gkg::HRule::~HRule()
{
}
