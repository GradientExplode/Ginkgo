#ifndef _gkg_graphics_X11_core_GrabInfo_h_
#define _gkg_graphics_X11_core_GrabInfo_h_


namespace gkg
{


class Window;
class EventHandler;


struct GrabInfo
{

  GrabInfo( Window* theWindow, EventHandler* theEventHandler )
          : window( theWindow ),
            eventHandler( theEventHandler )
  { }

  Window* window;
  EventHandler* eventHandler;

};


}


#endif
