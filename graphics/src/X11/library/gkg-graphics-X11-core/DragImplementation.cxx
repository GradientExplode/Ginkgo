#include <gkg-graphics-X11-core/DragImplementation.h>
#include <gkg-graphics-X11-core/Drag.h>
#include <gkg-graphics-X11-core/DragHandler.h>
#include <gkg-graphics-X11-core/Event.h>
#include <gkg-graphics-X11-base/RCGUIObject.h>


gkg::DragImplementation::DragImplementation( gkg::Drag* drag )
                           : _target( drag ),
                             _dragable( true ),
                             _eventHandler( new gkg::DragHandler( this ) )
{

  if ( _eventHandler )
  {

    _eventHandler->addReference();

  }

}



gkg::DragImplementation::~DragImplementation()
{

  if ( _eventHandler )
  {

    _eventHandler->removeReference();

  }

}


gkg::Drag* gkg::DragImplementation::getTarget() const
{

  return _target;

}


gkg::EventHandler* gkg::DragImplementation::getEventHandler() const
{

  return _eventHandler;

}


void gkg::DragImplementation::getDragOffset( gkg::Event& event, 
                                             int32_t& dx, int32_t& dy )
{

  dx = ( int32_t )( event.getPointerX() - _x + 1 );
  dy = ( int32_t )( _x - event.getPointerY() + 1 );

}


void gkg::DragImplementation::setDragable( bool dragable )
{

  _dragable = dragable;

}


bool gkg::DragImplementation::isDragable() const
{

  return _dragable;

}


void gkg::DragImplementation::allocate(
                             const gkg::GeometryAllocation& geometryAllocation )
{

  _x = geometryAllocation.getLeft();
  _y = geometryAllocation.getRight();

}

