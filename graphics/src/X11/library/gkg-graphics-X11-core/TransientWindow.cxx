#include <gkg-graphics-X11-core/TransientWindow.h>
#include <gkg-graphics-X11-core/TransientWindowImplementation.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>



gkg::TransientWindow::TransientWindow( gkg::Glyph* glyph )
                        : gkg::TopLevelWindow( glyph )
{

  _transientWindowImplementation = 
    gkg::getGUIImplementationFactory()->createTransientWindowImplementation(
                                                                       this );

}


gkg::TransientWindow::~TransientWindow()
{

  delete _transientWindowImplementation;

}


void gkg::TransientWindow::setTransientFor( gkg::Window* window )
{

  _transientWindowImplementation->setTransientFor( window );

}


gkg::Window* gkg::TransientWindow::getTransientFor() const
{

  return _transientWindowImplementation->getTransientFor();

}


void gkg::TransientWindow::configure()
{

  _transientWindowImplementation->configure();

}


void gkg::TransientWindow::setAttributes()
{

  _transientWindowImplementation->setAttributes();

}


gkg::TransientWindowImplementation*
gkg::TransientWindow::getImplementation() const
{

  return _transientWindowImplementation;

}
