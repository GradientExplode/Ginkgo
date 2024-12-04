#include <gkg-graphics-X11-core/UnvisibleWindow.h>
#include <gkg-graphics-X11-core/UnvisibleWindowImplementation.h>
#include <gkg-graphics-X11-core/Patch.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::UnvisibleWindow::UnvisibleWindow( gkg::Display* display )
                     : gkg::Window( new gkg::Patch( 0 ) )
{

  _unvisibleWindowImplementation = 
    gkg::getGUIImplementationFactory()->createUnvisibleWindowImplementation(
                                                              this, display );

}


gkg::UnvisibleWindow::~UnvisibleWindow()
{

  delete _unvisibleWindowImplementation;

}


void gkg::UnvisibleWindow::setAttributes()
{

  _unvisibleWindowImplementation->setAttributes();

}


gkg::UnvisibleWindowImplementation*
gkg::UnvisibleWindow::getImplementation() const
{

  return _unvisibleWindowImplementation;

}
