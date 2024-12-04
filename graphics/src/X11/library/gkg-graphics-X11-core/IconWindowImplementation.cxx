#include <gkg-graphics-X11-core/IconWindowImplementation.h>
#include <gkg-graphics-X11-core/IconWindow.h>


gkg::IconWindowImplementation::IconWindowImplementation(
                                                   gkg::IconWindow* iconWindow )
                              : _target( iconWindow )
{
}


gkg::IconWindowImplementation::~IconWindowImplementation()
{
}


gkg::IconWindow* gkg::IconWindowImplementation::getTarget() const
{

  return _target;

}

