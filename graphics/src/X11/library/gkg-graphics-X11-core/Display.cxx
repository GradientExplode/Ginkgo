#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/DisplayImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::Display::Display( const std::string& device )
{

  _displayImplementation = 
    gkg::getGUIImplementationFactory()->createDisplayImplementation( this,
                                                                     device );

}


gkg::Display::~Display()
{

  delete _displayImplementation;

}


void gkg::Display::close()
{

  _displayImplementation->close();

}


int32_t gkg::Display::getFd() const
{

  return _displayImplementation->getFd();

}


gkg::FloatCoordinate gkg::Display::getWidth() const
{

  return _displayImplementation->getWidth();

}


gkg::FloatCoordinate gkg::Display::getHeight() const
{

  return _displayImplementation->getHeight();

}


gkg::PixelCoordinate gkg::Display::getPixelWidth() const
{

  return _displayImplementation->getPixelWidth();

}


gkg::PixelCoordinate gkg::Display::getPixelHeight() const
{

  return _displayImplementation->getPixelHeight();

}


gkg::FloatCoordinate gkg::Display::getAWidth() const
{

  return _displayImplementation->getAWidth();

}


gkg::FloatCoordinate gkg::Display::getAHeight() const
{

  return _displayImplementation->getAHeight();

}


gkg::PixelCoordinate 
gkg::Display::toPixelCoordinate( gkg::FloatCoordinate floatCoordinate ) const
{

  return _displayImplementation->toPixelCoordinate( floatCoordinate );

}


gkg::FloatCoordinate 
gkg::Display::toFloatCoordinate( gkg::PixelCoordinate pixelCoordinate ) const
{

  return _displayImplementation->toFloatCoordinate( pixelCoordinate );

}


bool gkg::Display::getDefaults( std::string& defaults ) const
{

  return _displayImplementation->getDefaults( defaults );

}


void gkg::Display::setStyle( gkg::Style* style )
{

  _displayImplementation->setStyle( style );

}


gkg::Style* gkg::Display::getStyle() const
{

  return _displayImplementation->getStyle();

}


void gkg::Display::setScreen( int32_t screen )
{

  _displayImplementation->setScreen( screen );

}


void gkg::Display::repair()
{

  _displayImplementation->repair();

}


void gkg::Display::flush()
{

  _displayImplementation->flush();

}


void gkg::Display::sync()
{

  _displayImplementation->sync();

}


bool gkg::Display::getEvent( gkg::Event& event )
{

  return _displayImplementation->getEvent( event );

}


void gkg::Display::putEvent( const gkg::Event& event )
{

  _displayImplementation->putEvent( event );

}


bool gkg::Display::isClosed()
{

  return _displayImplementation->isClosed();

}


void gkg::Display::grab( gkg::Window* window, gkg::EventHandler* eventHandler )
{

  _displayImplementation->grab( window, eventHandler );

}


void gkg::Display::ungrab( gkg::EventHandler* eventHandler, bool all )
{

  _displayImplementation->ungrab( eventHandler, all );

}


gkg::EventHandler* gkg::Display::getGrabber() const
{

  return _displayImplementation->getGrabber();

}


bool gkg::Display::isGrabbing( gkg::EventHandler* eventHandler ) const
{

  return _displayImplementation->isGrabbing( eventHandler );

}


void gkg::Display::ringBell( int32_t volume )
{

  _displayImplementation->ringBell( volume );

}


void gkg::Display::setKeyClick( int32_t percent )
{

  _displayImplementation->setKeyClick( percent );

}


void gkg::Display::setAutoRepeat( bool value )
{

  _displayImplementation->setAutoRepeat( value );

}


void gkg::Display::setPointerFeedback( int32_t thresh, int32_t scale )
{

  _displayImplementation->setPointerFeedback( thresh, scale );

}


void gkg::Display::movePointer( gkg::FloatCoordinate x, gkg::FloatCoordinate y )
{

  _displayImplementation->movePointer( x, y );

}


gkg::SelectionManager* gkg::Display::getPrimarySelection()
{

  return _displayImplementation->getPrimarySelection();

}


gkg::SelectionManager* gkg::Display::getSecondarySelection()
{

  return _displayImplementation->getSecondarySelection();

}


gkg::SelectionManager* gkg::Display::getClipboardSelection()
{

  return _displayImplementation->getClipboardSelection();

}


gkg::SelectionManager* gkg::Display::findSelection( const std::string& name )
{

  return _displayImplementation->findSelection( name );

}


gkg::DisplayImplementation* gkg::Display::getImplementation() const
{

  return _displayImplementation;

}
