#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ValidateState_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ValidateState_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/EventState.h>


namespace gkg
{


class ValidateState : public EventState
{

  public:

    ValidateState( GraphicsScene* scene );

    void mousePressEvent( QGraphicsSceneMouseEvent* event );

};


}


#endif
