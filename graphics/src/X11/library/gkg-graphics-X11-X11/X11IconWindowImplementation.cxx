#include <gkg-graphics-X11-X11/X11IconWindowImplementation.h>
#include <gkg-graphics-X11-core/IconWindow.h>


gkg::X11IconWindowImplementation::X11IconWindowImplementation( 
                                                   gkg::IconWindow* iconWindow )
                                 : gkg::IconWindowImplementation( iconWindow )
{
}


gkg::X11IconWindowImplementation::~X11IconWindowImplementation()
{
}


void gkg::X11IconWindowImplementation::doMap()
{
}


gkg::X11IconWindowImplementation* 
gkg::X11IconWindowImplementation::getImplementation( 
                                                   gkg::IconWindow* iconWindow )
{

  return static_cast< X11IconWindowImplementation* >(
                                              iconWindow->getImplementation() );

}


