#include <gkg-graphics-X11-X11/X11PopupWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-core/PopupWindow.h>
#include <gkg-graphics-X11-core/Style.h>


gkg::X11PopupWindowImplementation::X11PopupWindowImplementation( 
                                                 gkg::PopupWindow* popupWindow )
                                 : gkg::PopupWindowImplementation( popupWindow )
{
}


gkg::X11PopupWindowImplementation::~X11PopupWindowImplementation()
{
}


void gkg::X11PopupWindowImplementation::setAttributes()
{
  _target->getStyle()->setAlias( "PopupWindow" );
  _target->gkg::Window::setAttributes();

  gkg::X11WindowImplementation* x11WindowImpl =
    gkg::X11WindowImplementation::getImplementation( _target );

  x11WindowImpl->setXAttributeMask( x11WindowImpl->getXAttributeMask() |
                                    CWOverrideRedirect );
  x11WindowImpl->getXAttributes().override_redirect = True;

}


gkg::X11PopupWindowImplementation* 
gkg::X11PopupWindowImplementation::getImplementation( 
                                                 gkg::PopupWindow* popupWindow )
{

  return static_cast< X11PopupWindowImplementation* >(
                                             popupWindow->getImplementation() );

}


