#ifndef _gkg_anatomist_plugin_control_MouseEvent_h_
#define _gkg_anatomist_plugin_control_MouseEvent_h_


#include <gkg-core-cppext/StdInt.h>

#include <anatomist/object/Object.h>
#include <anatomist/window/Window.h>


namespace gkg
{


class MouseEvent
{

  public:

    MouseEvent( const std::string& object = "" );
    MouseEvent( anatomist::AObject* object = 0 );
    virtual ~MouseEvent();

    void press( anatomist::AWindow* win, int32_t x, int32_t y );
    void move( anatomist::AWindow* win, int32_t x, int32_t y );
    void release( anatomist::AWindow* win, int32_t x, int32_t y );

    virtual void press( int32_t x, int32_t y, int32_t z );
    virtual void move( int32_t x, int32_t y, int32_t z );
    virtual void release( int32_t x, int32_t y, int32_t z );

  protected:

    void getPosition( anatomist::AWindow* win, 
                      int32_t x, int32_t y, 
                      Point3d& pos );
    bool validObject( anatomist::AObject* obj );

    anatomist::AObject* _object;

};


}


#endif
