#include <gkg-graphics-X11-core/ColorImplementation.h>


gkg::ColorImplementation::ColorImplementation( gkg::Visual* visual )
                         : _visual( visual )
{
}


gkg::ColorImplementation::~ColorImplementation()
{
}


gkg::Visual* gkg::ColorImplementation::getVisual() const
{

  return _visual;

}


bool gkg::ColorImplementation::isMasking() const
{

  return _masking;

}


