#include <gkg-graphics-X11-core/UnvisibleWindowImplementation.h>


gkg::UnvisibleWindowImplementation::UnvisibleWindowImplementation(
                                         gkg::UnvisibleWindow* unvisibleWindow )
                                   : _target( unvisibleWindow )
{
}


gkg::UnvisibleWindowImplementation::~UnvisibleWindowImplementation()
{
}


gkg::UnvisibleWindow* gkg::UnvisibleWindowImplementation::getTarget() const
{

  return _target;

}
