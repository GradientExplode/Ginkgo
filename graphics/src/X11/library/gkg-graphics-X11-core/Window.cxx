#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/WindowImplementation.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::Window::Window( gkg::Glyph* glyph )
{

  _windowImplementation = 
    gkg::getGUIImplementationFactory()->createWindowImplementation( this,
                                                                    glyph );
  _windowImplementation->clearMappingInfo();

}


gkg::Window::Window( gkg::WindowImplementation* windowImplementation )
            : _windowImplementation( windowImplementation )
{
}


gkg::Window::~Window()
{

  this->gkg::Window::unbind();
  delete _windowImplementation;

}


gkg::Glyph* gkg::Window::getGlyph() const
{

  return _windowImplementation->getGlyph();

}


void gkg::Window::setStyle( gkg::Style* style )
{

  _windowImplementation->setStyle( style );

}


gkg::Style* gkg::Window::getStyle() const
{

  return _windowImplementation->getStyle();

}


void gkg::Window::setDisplay( gkg::Display* display )
{

  _windowImplementation->setDisplay( display );

}


gkg::Display* gkg::Window::getDisplay() const
{

  return _windowImplementation->getDisplay();

}


gkg::Canvas* gkg::Window::getCanvas() const
{

  return _windowImplementation->getCanvas();

}


void gkg::Window::setCursor( gkg::Cursor* cursor )
{

  _windowImplementation->setCursor( cursor );

}


gkg::Cursor* gkg::Window::getCursor() const
{

  return _windowImplementation->getCursor();

}


void gkg::Window::pushCursor()
{

  _windowImplementation->pushCursor();

}


void gkg::Window::popCursor()
{

  _windowImplementation->popCursor();

}


void gkg::Window::setPlace( gkg::FloatCoordinate left,
                            gkg::FloatCoordinate bottom )
{

  _windowImplementation->setPlace( left, bottom );

}


void gkg::Window::setPixelPlace( gkg::IntegerCoordinate left,
                                 gkg::IntegerCoordinate bottom )
{

  _windowImplementation->setPixelPlace( left, bottom );

}


void gkg::Window::align( float x, float y )
{

  _windowImplementation->align( x, y );

}


gkg::FloatCoordinate gkg::Window::getLeft() const
{

  return _windowImplementation->getLeft();

}


gkg::FloatCoordinate gkg::Window::getBottom() const
{

  return _windowImplementation->getBottom();

}


gkg::FloatCoordinate gkg::Window::getWidth() const
{

  return _windowImplementation->getWidth();

}


gkg::FloatCoordinate gkg::Window::getHeight() const
{

  return _windowImplementation->getHeight();

}


void gkg::Window::map()
{

  _windowImplementation->map();

}


void gkg::Window::unmap()
{

  _windowImplementation->unmap();

}


bool gkg::Window::isMapped() const
{

  return _windowImplementation->isMapped();

}


void gkg::Window::bind()
{

  _windowImplementation->bind();

}


void gkg::Window::unbind()
{

  _windowImplementation->unbind();

}


bool gkg::Window::isBound() const
{

  return _windowImplementation->isBound();

}


void gkg::Window::raise()
{

  _windowImplementation->raise();

}


void gkg::Window::lower()
{

  _windowImplementation->lower();

}


void gkg::Window::move( gkg::FloatCoordinate left,
                        gkg::FloatCoordinate bottom )
{

  _windowImplementation->move( left, bottom );

}


void gkg::Window::resize()
{

  _windowImplementation->resize();

}


void gkg::Window::receive( const gkg::Event& event )
{

  _windowImplementation->receive( event );

}


gkg::EventHandler* gkg::Window::getTarget( const gkg::Event& event ) const
{

  return _windowImplementation->getTarget( event );

}


void gkg::Window::getGrabPointer( gkg::Cursor* cursor ) const
{

  return _windowImplementation->getGrabPointer( cursor );

}


void gkg::Window::ungrabPointer() const
{

  _windowImplementation->ungrabPointer();

}


void gkg::Window::repair()
{

  _windowImplementation->repair();

}


void gkg::Window::configure()
{

  _windowImplementation->configure();

}


void gkg::Window::setDefaultGeometry()
{

  _windowImplementation->setDefaultGeometry();

}


void gkg::Window::computeGeometry()
{

  _windowImplementation->computeGeometry();

}


void gkg::Window::setAttributes()
{

  _windowImplementation->setAttributes();

}


void gkg::Window::setProperties()
{

  _windowImplementation->setProperties();

}


void gkg::Window::doMap()
{

  _windowImplementation->doMap();

}


gkg::WindowImplementation* gkg::Window::getImplementation() const
{

  return _windowImplementation;

}


