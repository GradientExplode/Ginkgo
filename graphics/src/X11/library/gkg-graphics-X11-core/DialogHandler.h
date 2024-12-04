#ifndef _gkg_graphics_X11_core_DialogHandler_h_
#define _gkg_graphics_X11_core_DialogHandler_h_


#include <gkg-graphics-X11-core/EventHandler.h>


namespace gkg
{


class Dialog;


class DialogHandler : public EventHandler
{

  public:

    DialogHandler( Dialog* dialog );
    virtual ~DialogHandler();

    virtual bool event( Event& event );

  private:

    Dialog* _dialog;

};


}


#endif
