#include <gkg-graphics-X11-core/IconWindow.h>
#include <gkg-graphics-X11-core/IconWindowImplementation.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>



gkg::IconWindow::IconWindow( gkg::Glyph* glyph )
                : gkg::ManagedWindow( glyph )
{

  _iconWindowImplementation = 
    gkg::getGUIImplementationFactory()->createIconWindowImplementation( this );

}


gkg::IconWindow::~IconWindow()
{

  delete _iconWindowImplementation;

}


void gkg::IconWindow::doMap()
{

  _iconWindowImplementation->doMap();

}


gkg::IconWindowImplementation* gkg::IconWindow::getImplementation() const
{

  return _iconWindowImplementation;

}
