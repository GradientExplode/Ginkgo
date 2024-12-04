#ifndef _gkg_graphics_X11_X11_X11DragZoneImplementation_h_
#define _gkg_graphics_X11_X11_X11DragZoneImplementation_h_


#include <gkg-graphics-X11-core/DragZoneImplementation.h>


namespace gkg
{


class X11DragZoneImplementation : public DragZoneImplementation
{

  public:

    X11DragZoneImplementation( DragZone* dragZone );
    ~X11DragZoneImplementation();

    bool caught(const Event& event ) const;
    bool event( Event& event );

    static X11DragZoneImplementation* getImplementation( DragZone* dragZone );

};


}


#endif
