#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_StopDistanceState_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_StopDistanceState_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/EventState.h>


namespace gkg
{


class StopDistanceState : public EventState
{

  public:

    StopDistanceState( GraphicsScene* scene );

    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

};


}


#endif
