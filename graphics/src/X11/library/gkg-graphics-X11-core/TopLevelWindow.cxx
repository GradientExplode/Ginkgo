#include <gkg-graphics-X11-core/TopLevelWindow.h>
#include <gkg-graphics-X11-core/TopLevelWindowImplementation.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>



gkg::TopLevelWindow::TopLevelWindow( gkg::Glyph* glyph )
                    : gkg::ManagedWindow( glyph )
{

  _topLevelWindowImplementation = 
    gkg::getGUIImplementationFactory()->createTopLevelWindowImplementation(
                                                                         this );

}


gkg::TopLevelWindow::~TopLevelWindow()
{

  delete _topLevelWindowImplementation;

}


void gkg::TopLevelWindow::setGroupLeader( gkg::Window* window )
{

  _topLevelWindowImplementation->setGroupLeader( window );

}


gkg::Window* gkg::TopLevelWindow::getGroupLeader() const
{

  return _topLevelWindowImplementation->getGroupLeader();

}


void gkg::TopLevelWindow::setProperties()
{

  _topLevelWindowImplementation->setProperties();

}


gkg::TopLevelWindowImplementation*
gkg::TopLevelWindow::getImplementation() const
{

  return _topLevelWindowImplementation;

}
