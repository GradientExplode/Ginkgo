#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/ApplicationWindowImplementation.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::ApplicationWindow::ApplicationWindow( gkg::Glyph* glyph )
                       : gkg::ManagedWindow( glyph )
{

  _applicationWindowImplementation = 
    gkg::getGUIImplementationFactory()->createApplicationWindowImplementation(
                                                                         this );

}


gkg::ApplicationWindow::~ApplicationWindow()
{

  delete _applicationWindowImplementation;

}


void gkg::ApplicationWindow::computeGeometry()
{

  _applicationWindowImplementation->computeGeometry();

}


void gkg::ApplicationWindow::setProperties()
{

  _applicationWindowImplementation->setProperties();

}


gkg::ApplicationWindowImplementation*
gkg::ApplicationWindow::getImplementation() const
{

  return _applicationWindowImplementation;

}
