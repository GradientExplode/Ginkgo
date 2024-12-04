#ifndef _gkg_anatomist_plugin_control_MouseControl_h_
#define _gkg_anatomist_plugin_control_MouseControl_h_


#include <anatomist/controler/control.h>
#include <anatomist/controler/action.h>
#include <anatomist/controler/actionpool.h>


namespace gkg
{


class MouseControl : public anatomist::Control
{

  public:

    MouseControl( const std::string& name = "GkgMouseControl" );
    MouseControl( const MouseControl& c );

    static anatomist::Control* creator();

    virtual void eventAutoSubscription( anatomist::ActionPool* actionPool );

};


}


#endif
