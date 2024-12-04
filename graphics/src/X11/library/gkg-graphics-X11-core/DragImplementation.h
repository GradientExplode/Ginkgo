#ifndef _gkg_graphics_X11_core_DragImplementation_h_
#define _gkg_graphics_X11_core_DragImplementation_h_


#include <gkg-graphics-X11-base/Coordinates.h>


namespace gkg
{


class Drag;
class EventHandler;
class Event;
class GeometryAllocation;


class DragImplementation
{

  public:

    DragImplementation( Drag* drag );
    virtual ~DragImplementation();

    virtual Drag* getTarget() const;
    virtual EventHandler* getEventHandler() const;

    virtual void getDragOffset( Event& event, int32_t& dx, int32_t& dy );
    virtual void setDragable( bool dragable );
    virtual bool isDragable() const;
    virtual void allocate( const GeometryAllocation& geometryAllocation );


    virtual bool event( Event& event ) = 0;

  protected:

    Drag* _target;
    bool _dragable;
    EventHandler* _eventHandler;
    FloatCoordinate _x;
    FloatCoordinate _y;


};


}


#endif
