#include <gkg-graphics-X11-X11/X11ApplicationWindowImplementation.h>
#include <gkg-graphics-X11-X11/X11WindowImplementation.h>
#include <gkg-graphics-X11-X11/X11SessionImplementation.h>
#include <gkg-graphics-X11-X11/X11DisplayImplementation.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/Canvas.h>
#include <gkg-graphics-X11-core/CanvasImplementation.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/Style.h>
#include <gkg-graphics-X11-core/Session.h>


gkg::X11ApplicationWindowImplementation::X11ApplicationWindowImplementation( 
                                     gkg::ApplicationWindow* applicationWindow )
                  : gkg::ApplicationWindowImplementation( applicationWindow )
{
}


gkg::X11ApplicationWindowImplementation::~X11ApplicationWindowImplementation()
{
}


void gkg::X11ApplicationWindowImplementation::computeGeometry()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation(_target);
  gkg::Canvas* canvas = x11WindowImpl->getCanvas();
  gkg::Display* display = canvas->getImplementation()->getDisplay();

  uint32_t spec = 0;
  std::string s;
  if ( x11WindowImpl->getStyle() && 
       x11WindowImpl->getStyle()->findAttribute( "geometry", s ) )
  {

    s += '\0';
    uint32_t xWidth, xHeight;
    int32_t xPosition, yPosition;
    spec = XParseGeometry( s.c_str(),
                           &xPosition, &yPosition,
                           &xWidth, &xHeight );
    x11WindowImpl->setXPosition( xPosition );
    x11WindowImpl->setYPosition( yPosition );
    const uint32_t userPlace = XValue | YValue;
    if ( ( spec & userPlace ) == userPlace )
    {

      x11WindowImpl->setPlaced( true );

    }

    if ( spec & WidthValue )
    {

      canvas->setPixelWidth( ( gkg::PixelCoordinate )xWidth );

    }
    if ( spec & HeightValue )
    {

      canvas->setPixelHeight( ( gkg::PixelCoordinate )xHeight );

    }

    if ( ( spec & XValue ) && ( spec & XNegative ) )
    {

      x11WindowImpl->setXPosition( display->getPixelHeight() + 
                                   x11WindowImpl->getXPosition() -
                                   canvas->getPixelWidth() );

    }

    if ( ( spec & YValue ) && ( spec & YNegative ) )
    {

      x11WindowImpl->setYPosition( display->getPixelHeight() + 
                                   x11WindowImpl->getYPosition() -
                                   canvas->getPixelHeight() );

    }

  }
  _target->gkg::ManagedWindow::computeGeometry();

}


void gkg::X11ApplicationWindowImplementation::setProperties()
{

  gkg::X11WindowImplementation*
    x11WindowImpl = gkg::X11WindowImplementation::getImplementation( _target );

  gkg::X11SessionImplementation*
    x11SessionImpl = gkg::X11SessionImplementation::getImplementation(
                                                 &gkg::Session::getInstance() );

  gkg::Display* display = x11WindowImpl->getDisplay();

  if ( !display )
  {

    display = x11SessionImpl->getDefaultDisplay();

  }

  XDisplay* xDisplay =
    gkg::X11DisplayImplementation::getImplementation( display )->getXDisplay();

  XSetCommand( xDisplay, x11WindowImpl->getXWindow(),
               x11SessionImpl->getArgv(), x11SessionImpl->getArgc() );

  _target->gkg::ManagedWindow::setProperties();

}


gkg::X11ApplicationWindowImplementation* 
gkg::X11ApplicationWindowImplementation::getImplementation( 
                                     gkg::ApplicationWindow* applicationWindow )
{

  return static_cast< X11ApplicationWindowImplementation* >(
                     applicationWindow->getImplementation() );

}


