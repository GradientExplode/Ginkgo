#include <gkg-graphics-X11-core/DragZoneImplementation.h>
#include <gkg-graphics-X11-core/DragZone.h>
#include <gkg-graphics-X11-core/DragZoneHandler.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


gkg::DragZoneImplementation::DragZoneImplementation( DragZone* dragZone )
                            : _target( dragZone ),
                              _sensitive( true ),
                              _grabbing( false ),
                              _eventHandler( new gkg::DragZoneHandler( this ) )
{

  if ( _eventHandler )
  {

    _eventHandler->addReference();

  }

}


gkg::DragZoneImplementation::~DragZoneImplementation()
{

  if ( _eventHandler )
  {

    _eventHandler->removeReference();

  }

}


gkg::DragZone* gkg::DragZoneImplementation::getTarget() const
{

  return _target;

}


gkg::EventHandler* gkg::DragZoneImplementation::getEventHandler() const
{

  return _eventHandler;

}


void gkg::DragZoneImplementation::setSensitive( bool sensitive )
{

  _sensitive = sensitive;

}


bool gkg::DragZoneImplementation::isSensitive() const
{

  return _sensitive;

}


void gkg::DragZoneImplementation::allocate(
                              gkg::Canvas* canvas,
                              const gkg::GeometryAllocation& geometryAllocation,
                              gkg::GeometryExtension& geometryExtension )
{

  _canvas = canvas;
  _geometryAllocation = geometryAllocation;
  _geometryExtension = geometryExtension;

}


