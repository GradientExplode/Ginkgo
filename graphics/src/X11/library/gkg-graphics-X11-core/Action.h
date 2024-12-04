#ifndef _gkg_graphics_X11_core_Action_h_
#define _gkg_graphics_X11_core_Action_h_


#include <gkg-graphics-X11-base/RCGUIObject.h>


namespace gkg
{


class Action : public RCGUIObject
{

  public:

    virtual void execute() = 0;

  protected:

    Action();
    virtual ~Action();

};


}


#endif
