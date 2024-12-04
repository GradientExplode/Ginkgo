#ifndef _gkg_graphics_X11_core_DragZoneSinkImplementation_h_
#define _gkg_graphics_X11_core_DragZoneSinkImplementation_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


class DragZoneSink;
class EventHandler;
class Canvas;
class GeometryAllocation;
class Event;
class Hit;


class DragZoneSinkImplementation
{

  public:

    DragZoneSinkImplementation( DragZoneSink* dragZoneSink );
    virtual ~DragZoneSinkImplementation();

    virtual DragZoneSink* getTarget() const;
    virtual EventHandler* getEventHandler() const;
    virtual bool isDragPublished() const;

    virtual void drop( Event& event, const char* data, int32_t length ) = 0;

    virtual void draw( Canvas* canvas ) = 0;
    virtual void pick( int32_t depth, Hit& hit ) = 0;

    virtual bool event( Event& event ) = 0;

  protected:

    DragZoneSink* _target;
    bool _dragPublished;
    EventHandler* _eventHandler;

};


}


#endif
