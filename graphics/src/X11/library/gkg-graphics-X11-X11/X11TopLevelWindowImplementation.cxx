#include <gkg-graphics-X11-X11/X11TopLevelWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11ManagedWindowImplementation.h>
#include <gkg-graphics-X11-core/TopLevelWindow.h>


gkg::X11TopLevelWindowImplementation::X11TopLevelWindowImplementation( 
                                           gkg::TopLevelWindow* topLevelWindow )
                           : gkg::TopLevelWindowImplementation( topLevelWindow )
{
}


gkg::X11TopLevelWindowImplementation::~X11TopLevelWindowImplementation()
{
}


void gkg::X11TopLevelWindowImplementation::setGroupLeader( gkg::Window* window )
{

  gkg::X11ManagedWindowImplementation* x11ManagedWindowImpl =
              gkg::X11ManagedWindowImplementation::getImplementation( _target );
  x11ManagedWindowImpl->setGroupLeader( window );
  x11ManagedWindowImpl->doSet(
                     &gkg::X11ManagedWindowImplementation::setInfoGroupLeader );

}


gkg::X11TopLevelWindowImplementation* 
gkg::X11TopLevelWindowImplementation::getImplementation( 
                                           gkg::TopLevelWindow* topLevelWindow )
{

  return static_cast< X11TopLevelWindowImplementation* >(
                                          topLevelWindow->getImplementation() );

}


