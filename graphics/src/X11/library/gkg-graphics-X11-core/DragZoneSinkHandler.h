#ifndef _gkg_graphics_X11_core_DragZoneSinkHandler_h_
#define _gkg_graphics_X11_core_DragZoneSinkHandler_h_


#include <gkg-graphics-X11-core/EventHandler.h>


namespace gkg
{


class DragZoneSinkImplementation;


class DragZoneSinkHandler : public EventHandler
{

  public:

    DragZoneSinkHandler( DragZoneSinkImplementation* 
                           dragZoneSinkImplementation );
    virtual ~DragZoneSinkHandler();

    virtual bool event( Event& event );

  protected:

    DragZoneSinkImplementation* _dragZoneSinkImplementation;

};


}


#endif
