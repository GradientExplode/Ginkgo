#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_EventState_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_EventState_h_


class QGraphicsSceneMouseEvent;
class QKeyEvent;


namespace gkg
{


class GraphicsScene;


class EventState
{

  public:

    EventState( GraphicsScene* scene );
    virtual~EventState();

    virtual void mousePressEvent( QGraphicsSceneMouseEvent* event );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );

  protected:

    GraphicsScene* _scene;

};


}


#endif
