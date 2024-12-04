#include <gkg-graphics-X11-core/TopLevelWindowImplementation.h>
#include <gkg-graphics-X11-core/TopLevelWindow.h>
#include <gkg-graphics-X11-core/ManagedWindow.h>
#include <gkg-graphics-X11-core/ManagedWindowImplementation.h>


gkg::TopLevelWindowImplementation::TopLevelWindowImplementation(
                                           gkg::TopLevelWindow* topLevelWindow )
                                  : _target( topLevelWindow )
{
}


gkg::TopLevelWindowImplementation::~TopLevelWindowImplementation()
{
}


gkg::TopLevelWindow* gkg::TopLevelWindowImplementation::getTarget() const
{

  return _target;

}


gkg::Window* gkg::TopLevelWindowImplementation::getGroupLeader() const
{

  return _target->gkg::ManagedWindow::getImplementation()->getGroupLeader();

}


void gkg::TopLevelWindowImplementation::setProperties()
{

  _target->gkg::ManagedWindow::setProperties();

}


