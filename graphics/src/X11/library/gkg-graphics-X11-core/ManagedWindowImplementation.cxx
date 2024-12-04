#include <gkg-graphics-X11-core/ManagedWindowImplementation.h>
#include <gkg-graphics-X11-core/ManagedWindow.h>
#include <gkg-graphics-X11-core/Window.h>
#include <gkg-graphics-X11-core/WindowImplementation.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>
#include <gkg-graphics-X11-core/Bitmap.h>


gkg::ManagedWindowImplementation::ManagedWindowImplementation(
                                             gkg::ManagedWindow* managedWindow )
                                 : _target( managedWindow ),
                                   _icon( 0 ),
                                   _iconBitmap( 0 ),
                                   _iconMask( 0 ),
                                   _groupLeader( 0 ),
                                   _transientFor( 0 )
{
}


gkg::ManagedWindowImplementation::~ManagedWindowImplementation()
{

  if ( _iconBitmap )
  {

    _iconBitmap->removeReference();

  }
  if ( _iconMask )
  {

    _iconMask->removeReference();

  }

}


gkg::ManagedWindow* gkg::ManagedWindowImplementation::getTarget() const
{

  return _target;

}


void gkg::ManagedWindowImplementation::setIcon( 
                                             gkg::ManagedWindow* managedWindow )
{

  _icon = managedWindow;
  doSetInfoIcon();

}


gkg::ManagedWindow* gkg::ManagedWindowImplementation::getIcon() const
{

  return _icon;

}


void gkg::ManagedWindowImplementation::setIconBitmap( gkg::Bitmap* bitmap)
{

  if ( bitmap )
  {

    bitmap->addReference();

  }
  if ( _iconBitmap )
  {

    _iconBitmap->removeReference();

  }
  _iconBitmap = bitmap;
  doSetInfoIconBitmap();

}


gkg::Bitmap* gkg::ManagedWindowImplementation::getIconBitmap() const
{

  return _iconBitmap;

}


void gkg::ManagedWindowImplementation::setIconMask( gkg::Bitmap* bitmap )
{

  if ( bitmap )
  {

    bitmap->addReference();

  }
  if ( _iconMask )
  {

    _iconMask->removeReference();

  }
  _iconMask = bitmap;
  doSetInfoIconMask();

}


gkg::Bitmap* 
gkg::ManagedWindowImplementation::getIconMask() const
{

  return _iconMask;

}


void 
gkg::ManagedWindowImplementation::setFocusEvent( gkg::EventHandler* in,
                                                 gkg::EventHandler* out )
{

  _target->gkg::Window::getImplementation()->setFocusEvent( in, out );

}


void gkg::ManagedWindowImplementation::setWmDelete(
                                               gkg::EventHandler* eventHandler )
{

  _target->gkg::Window::getImplementation()->setWmDelete( eventHandler );

}


void gkg::ManagedWindowImplementation::resize()
{

  _target->gkg::Window::getImplementation()->setDefaultGeometry();
  setWmNormalHints();
  _target->gkg::Window::getImplementation()->resize();


}


void 
gkg::ManagedWindowImplementation::setGroupLeader( gkg::Window* window )
{

  _groupLeader = window;

}


gkg::Window* gkg::ManagedWindowImplementation::getGroupLeader() const
{

  return _groupLeader;

}


void gkg::ManagedWindowImplementation::setTransientFor( gkg::Window* window )
{

  _transientFor = window;

}


gkg::Window* gkg::ManagedWindowImplementation::getTransientFor() const
{

  return _transientFor;

}
