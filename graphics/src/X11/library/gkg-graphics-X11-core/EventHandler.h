#ifndef _gkg_graphics_X11_core_EventHandler_h_
#define _gkg_graphics_X11_core_EventHandler_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Event;


class EventHandler : public RCGUIObject
{

  public:

    virtual bool event( Event& event ) = 0;

  protected:

    EventHandler();
    virtual ~EventHandler();

};


}


#endif
