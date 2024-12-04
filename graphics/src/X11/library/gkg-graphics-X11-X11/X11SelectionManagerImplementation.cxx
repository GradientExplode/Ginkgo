#include <gkg-graphics-X11-X11/X11SelectionManagerImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-core/UnvisibleWindow.h>
#include <gkg-graphics-X11-core/SelectionHandler.h>
#include <gkg-graphics-X11-core/SelectionManager.h>
#include <X11/Xatom.h>


gkg::X11SelectionManagerImplementation::X11SelectionManagerImplementation(
                                        gkg::SelectionManager* selectionManager,
                                        gkg::Display* display,
                                        const std::string& name )
                        : gkg::SelectionManagerImplementation( selectionManager,
                                                               display,
                                                               name )
{

  gkg::X11DisplayImplementation*
    x11DisplayImpl = gkg::X11DisplayImplementation::getImplementation( 
                                                                      display );
  _xDisplay = x11DisplayImpl->getXDisplay();

}


gkg::X11SelectionManagerImplementation::~X11SelectionManagerImplementation()
{
}


void gkg::X11SelectionManagerImplementation::own()
{

  XWindow xWindow = gkg::X11WindowImplementation::getImplementation(
                                                         _owner )->getXWindow();
  Atom atom = XInternAtom( _xDisplay, _name.c_str(), true );
  XSetSelectionOwner( _xDisplay, atom, xWindow, CurrentTime );


}


void gkg::X11SelectionManagerImplementation::putValue( const void* data,
                                                       int32_t length,
                                                       int32_t format )
{

  XChangeProperty( _xDisplay,
                   _xRequest.requestor, _xRequest.property,
                   XA_STRING,  // type
                   format,
                   PropModeReplace,
                   ( const unsigned char* )data,
                   length );
  XEvent xEvent;
  XSelectionEvent& xSelectionEvent = xEvent.xselection;
  xSelectionEvent.type = SelectionNotify;
  xSelectionEvent.requestor = _xRequest.requestor;
  xSelectionEvent.selection = _xRequest.selection;
  xSelectionEvent.target = _xRequest.target;
  xSelectionEvent.property = _xRequest.property;
  xSelectionEvent.time = _xRequest.time;
  XSendEvent( _xDisplay, xSelectionEvent.requestor, False, 0, &xEvent );

}


void gkg::X11SelectionManagerImplementation::request(
                          gkg::SelectionManager* selectionManager,
                          const XSelectionRequestEvent& xSelectionRequestEvent )
{

  if ( _convert )
  {

    _xRequest = xSelectionRequestEvent;
    _convert->handle( selectionManager );

  }

}


void gkg::X11SelectionManagerImplementation::notify(
                                     gkg::SelectionManager*,
                                     const XSelectionEvent& )
{

  // unimplemented

}


gkg::X11SelectionManagerImplementation* 
gkg::X11SelectionManagerImplementation::getImplementation(
                                       gkg::SelectionManager* selectionManager )
{

  return static_cast< X11SelectionManagerImplementation* >(
                                        selectionManager->getImplementation() );

}
