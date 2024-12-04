#include <gkg-graphics-X11-X11/X11UnvisibleWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-core/UnvisibleWindow.h>
#include <gkg-graphics-X11-core/Style.h>


gkg::X11UnvisibleWindowImplementation::X11UnvisibleWindowImplementation( 
                                          gkg::UnvisibleWindow* unvisibleWindow,
                                          gkg::Display* display )
                         : gkg::UnvisibleWindowImplementation( unvisibleWindow )
{

  gkg::X11DisplayImplementation*
    x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation(
                                                                      display );
  gkg::X11WindowImplementation*
  x11WindowImpl = gkg::X11WindowImplementation::getImplementation( _target );

  XDisplay* xDisplay = x11DisplayImpl->getXDisplay();
  XWindow xRoot = x11DisplayImpl->getXRoot();
  XWindow xWindow = XCreateSimpleWindow( xDisplay, xRoot, 0, 0, 1, 1, 0, 0, 0 );

  x11WindowImpl->forceDisplay( display );
  x11WindowImpl->forceXWindow( xWindow );
  x11WindowImpl->forceXTopLevelWindow( xWindow );

  x11DisplayImpl->addWindow( _target );

}


gkg::X11UnvisibleWindowImplementation::~X11UnvisibleWindowImplementation()
{
}


void gkg::X11UnvisibleWindowImplementation::setAttributes()
{

  _target->getStyle()->setAlias( "UnvisibleWindow" );
  _target->gkg::Window::setAttributes();

  gkg::X11WindowImplementation* x11WindowImpl =
    gkg::X11WindowImplementation::getImplementation( _target );

  x11WindowImpl->setXAttributeMask( x11WindowImpl->getXAttributeMask() |
                                    CWOverrideRedirect );
  x11WindowImpl->getXAttributes().override_redirect = True;

}


gkg::X11UnvisibleWindowImplementation* 
gkg::X11UnvisibleWindowImplementation::getImplementation( 
                                         gkg::UnvisibleWindow* unvisibleWindow )
{

  return static_cast< X11UnvisibleWindowImplementation* >(
                                         unvisibleWindow->getImplementation() );

}


