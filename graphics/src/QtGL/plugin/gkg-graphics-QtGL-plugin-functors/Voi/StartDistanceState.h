#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_StartDistanceState_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_StartDistanceState_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/EventState.h>


namespace gkg
{


class StartDistanceState : public EventState
{

  public:

    StartDistanceState( GraphicsScene* scene );

    void mousePressEvent( QGraphicsSceneMouseEvent* event );

};


}


#endif
