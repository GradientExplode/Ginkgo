#ifndef _gkg_graphics_X11_core_DragZoneImplementation_h_
#define _gkg_graphics_X11_core_DragZoneImplementation_h_


#include <gkg-graphics-X11-core/MonoGlyph.h>


namespace gkg
{


class DragZone;
class EventHandler;
class Event;


class DragZoneImplementation
{

  public:

    DragZoneImplementation( DragZone* dragZone );
    virtual ~DragZoneImplementation();

    virtual DragZone* getTarget() const;
    virtual EventHandler* getEventHandler() const;

    virtual void setSensitive( bool sensitive );
    virtual bool isSensitive() const;

    virtual void allocate( Canvas* canvas,
                           const GeometryAllocation& geometryAllocation,
                           GeometryExtension& geometryExtension );

    virtual bool caught(const Event& event ) const = 0;
    virtual bool event( Event& event ) = 0;

  protected:

    DragZone* _target;
    bool _sensitive;
    bool _grabbing;
    EventHandler* _eventHandler;
    Canvas* _canvas;
    GeometryAllocation _geometryAllocation;
    GeometryExtension _geometryExtension;

};


}


#endif
