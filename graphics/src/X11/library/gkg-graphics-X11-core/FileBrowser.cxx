#include <gkg-graphics-X11-core/FileBrowser.h>
#include <gkg-graphics-X11-core/FileBrowserImplementation.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Event.h>



gkg::FileBrowser::FileBrowser( gkg::Action* accept, gkg::Action* cancel )
                 : gkg::Browser( 0,
                gkg::LookAndFeel::getInstance().getWidgetFactory()->getStyle(),
                                 accept,
                                 cancel )
{

  _fileBrowserImplementation = new gkg::FileBrowserImplementation( this );

}


gkg::FileBrowser::~FileBrowser()
{

  delete _fileBrowserImplementation;

}


void gkg::FileBrowser::press( const gkg::Event& event )
{

  _fileBrowserImplementation->press( event );

}


void gkg::FileBrowser::drag( const gkg::Event& event )
{

  _fileBrowserImplementation->drag( event );

}


void gkg::FileBrowser::release( const gkg::Event& event )
{

  _fileBrowserImplementation->release( event );

}


void gkg::FileBrowser::keyStroke( const gkg::Event& event )
{

  _fileBrowserImplementation->keyStroke( event );

}


gkg::InputHandler* gkg::FileBrowser::focusIn()
{

  _fileBrowserImplementation->focusIn();
  return this->gkg::Browser::focusIn();

}


void gkg::FileBrowser::focusOut()
{

  _fileBrowserImplementation->focusOut();
  this->gkg::Browser::select( -1 );

}


void gkg::FileBrowser::select( gkg::Glyph::Index index )
{

  _fileBrowserImplementation->select( index );
  this->gkg::Browser::select( index );

}


gkg::Adjustable* gkg::FileBrowser::getAdjustable() const
{

  return _fileBrowserImplementation->getAdjustable();

}


void gkg::FileBrowser::refresh()
{

  _fileBrowserImplementation->refresh();

}


gkg::FileBrowserImplementation* gkg::FileBrowser::getImplementation() const
{

  return _fileBrowserImplementation;

}
