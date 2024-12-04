#ifndef _gkg_graphics_X11_core_DragHandler_h_
#define _gkg_graphics_X11_core_DragHandler_h_


#include <gkg-graphics-X11-core/EventHandler.h>


namespace gkg
{


class DragImplementation;


class DragHandler : public EventHandler
{

  public:

    DragHandler( DragImplementation* dragImplementation );
    virtual ~DragHandler();

    virtual bool event( Event& event );

  protected:

    DragImplementation* _dragImplementation;

};


}


#endif
