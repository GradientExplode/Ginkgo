#ifndef _gkg_graphics_X11_core_DragZoneHandler_h_
#define _gkg_graphics_X11_core_DragZoneHandler_h_


#include <gkg-graphics-X11-core/EventHandler.h>


namespace gkg
{


class DragZoneImplementation;


class DragZoneHandler : public EventHandler
{

  public:

    DragZoneHandler( DragZoneImplementation* dragZoneImplementation );
    virtual ~DragZoneHandler();

    virtual bool event( Event& event );

  protected:

    DragZoneImplementation* _dragZoneImplementation;

};


}


#endif
