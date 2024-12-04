#include <gkg-graphics-X11-core/TransientWindowImplementation.h>
#include <gkg-graphics-X11-core/TransientWindow.h>
#include <gkg-graphics-X11-core/ManagedWindow.h>
#include <gkg-graphics-X11-core/ManagedWindowImplementation.h>
#include <gkg-graphics-X11-core/Style.h>



gkg::TransientWindowImplementation::TransientWindowImplementation(
                                         gkg::TransientWindow* transientWindow )
                                   : _target( transientWindow )
{
}


gkg::TransientWindowImplementation::~TransientWindowImplementation()
{
}


gkg::TransientWindow* gkg::TransientWindowImplementation::getTarget() const
{

  return _target;

}


gkg::Window* gkg::TransientWindowImplementation::getTransientFor() const
{

  return _target->gkg::ManagedWindow::getImplementation()->getTransientFor();

}


void gkg::TransientWindowImplementation::configure()
{

  _target->gkg::Window::configure();

}


void gkg::TransientWindowImplementation::setAttributes()
{

  _target->getStyle()->setAlias( "TransientWindow" );
  _target->gkg::TopLevelWindow::setAttributes();

}


