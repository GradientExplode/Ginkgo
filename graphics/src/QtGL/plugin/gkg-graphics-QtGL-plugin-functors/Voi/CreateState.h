#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_CreateState_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_CreateState_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/EventState.h>


namespace gkg
{


class CreateState : public EventState
{

  public:

    CreateState( GraphicsScene* scene );

    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void keyPressEvent( QKeyEvent* event );

};


}


#endif
