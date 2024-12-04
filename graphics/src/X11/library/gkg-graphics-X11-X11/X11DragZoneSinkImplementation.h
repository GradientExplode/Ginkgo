#ifndef _gkg_graphics_X11_X11_X11DragZoneSinkImplementation_h_
#define _gkg_graphics_X11_X11_X11DragZoneSinkImplementation_h_


#include <gkg-graphics-X11-core/DragZoneSinkImplementation.h>


namespace gkg
{


class X11DragZoneSinkImplementation : public DragZoneSinkImplementation
{

  public:

    X11DragZoneSinkImplementation( DragZoneSink* dragZoneSink );
    ~X11DragZoneSinkImplementation();

    void drop( Event& event, const char* data, int32_t length );
    void draw( Canvas* canvas );
    void pick( int32_t depth, Hit& hit );

    bool event( Event& event );

    static X11DragZoneSinkImplementation*
      getImplementation( DragZoneSink* dragZoneSink );

};


}


#endif
