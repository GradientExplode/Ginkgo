#ifndef _gkg_anatomist_plugin_control_MouseAction_h_
#define _gkg_anatomist_plugin_control_MouseAction_h_


#include <gkg-core-pattern/RCPointer.h>

#include <anatomist/controler/action.h>


namespace gkg
{


class MouseEvent;


class MouseAction : public anatomist::Action
{

  public:

    MouseAction();
    MouseAction( const MouseAction& a );

    static anatomist::Action* creator();

    std::string name() const;

    void setMouseEvent( MouseEvent* event );

    void press( int32_t x, int32_t y, 
                int32_t globalX, int32_t globalY );
    void move( int32_t x, int32_t y, 
               int32_t globalX, int32_t globalY );
    void release( int32_t x, int32_t y, 
                  int32_t globalX, int32_t globalY );

  private:

    RCPointer< MouseEvent > _mouseEvent;

};


}


#endif
