#include <gkg-graphics-X11-core/ApplicationWindowImplementation.h>


gkg::ApplicationWindowImplementation::ApplicationWindowImplementation(
                                     gkg::ApplicationWindow* applicationWindow )
                                     : _target( applicationWindow )
{
}


gkg::ApplicationWindowImplementation::~ApplicationWindowImplementation()
{
}


gkg::ApplicationWindow* gkg::ApplicationWindowImplementation::getTarget() const
{

  return _target;

}
