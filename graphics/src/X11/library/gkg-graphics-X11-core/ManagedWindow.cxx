#include <gkg-graphics-X11-core/ManagedWindow.h>
#include <gkg-graphics-X11-core/ManagedWindowImplementation.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/Glyph.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>



gkg::ManagedWindow::ManagedWindow( gkg::Glyph* glyph )
                   : gkg::Window( glyph )
{

  _managedWindowImplementation = 
  gkg::getGUIImplementationFactory()->createManagedWindowImplementation( this );

}


gkg::ManagedWindow::~ManagedWindow()
{

  delete _managedWindowImplementation;

}


void gkg::ManagedWindow::setIcon( gkg::ManagedWindow* managedWindow )
{

  _managedWindowImplementation->setIcon( managedWindow );

}


gkg::ManagedWindow* gkg::ManagedWindow::getIcon() const
{

  return _managedWindowImplementation->getIcon();

}


void gkg::ManagedWindow::setIconBitmap( gkg::Bitmap* bitmap )
{

  _managedWindowImplementation->setIconBitmap( bitmap );

}


gkg::Bitmap* gkg::ManagedWindow::getIconBitmap() const
{

  return _managedWindowImplementation->getIconBitmap();

}


void gkg::ManagedWindow::setIconMask( gkg::Bitmap* bitmap )
{

  _managedWindowImplementation->setIconMask( bitmap );

}


gkg::Bitmap* gkg::ManagedWindow::getIconMask() const
{

  return _managedWindowImplementation->getIconMask();

}


void gkg::ManagedWindow::iconify()
{

  _managedWindowImplementation->iconify();

}


void gkg::ManagedWindow::deiconify()
{

  _managedWindowImplementation->iconify();

}


void gkg::ManagedWindow::setFocusEvent( gkg::EventHandler* in,
                                        gkg::EventHandler* out )
{

  _managedWindowImplementation->setFocusEvent( in, out );

}


void gkg::ManagedWindow::setWmDelete( gkg::EventHandler* eventHandler )
{

  _managedWindowImplementation->setWmDelete( eventHandler );

}


void gkg::ManagedWindow::resize()
{

  _managedWindowImplementation->resize();

}


void gkg::ManagedWindow::computeGeometry()
{

  _managedWindowImplementation->computeGeometry();

}


void gkg::ManagedWindow::setProperties()
{

  _managedWindowImplementation->setProperties();

}


gkg::ManagedWindowImplementation* gkg::ManagedWindow::getImplementation() const
{

  return _managedWindowImplementation;

}
