#include <gkg-graphics-X11-X11/X11TransientWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11ManagedWindowImplementation.h>
#include <gkg-graphics-X11-core/TransientWindow.h>


gkg::X11TransientWindowImplementation::X11TransientWindowImplementation( 
                                         gkg::TransientWindow* transientWindow )
                         : gkg::TransientWindowImplementation( transientWindow )
{
}


gkg::X11TransientWindowImplementation::~X11TransientWindowImplementation()
{
}


void 
gkg::X11TransientWindowImplementation::setTransientFor( gkg::Window* window )
{

  gkg::X11ManagedWindowImplementation* x11ManagedWindowImpl =
    gkg::X11ManagedWindowImplementation::getImplementation( _target );
  x11ManagedWindowImpl->setTransientFor( window );
  x11ManagedWindowImpl->doSet(
                    &gkg::X11ManagedWindowImplementation::setInfoTransientFor );

}


gkg::X11TransientWindowImplementation* 
gkg::X11TransientWindowImplementation::getImplementation( 
                                         gkg::TransientWindow* transientWindow )
{

  return static_cast< X11TransientWindowImplementation* >(
                                         transientWindow->getImplementation() );

}


