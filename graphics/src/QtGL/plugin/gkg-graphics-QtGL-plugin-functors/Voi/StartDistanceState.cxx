#include <gkg-graphics-QtGL-plugin-functors/Voi/StartDistanceState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StopDistanceState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>

#include <QGraphicsSceneMouseEvent>


gkg::StartDistanceState::StartDistanceState( gkg::GraphicsScene* scene )
                       : gkg::EventState( scene )
{
}


void gkg::StartDistanceState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{

  if ( event->button() == Qt::LeftButton )
  {

    _scene->addFirstPoint( event->scenePos() );
    event->accept();
    _scene->setState( new gkg::StopDistanceState( _scene ) );

  }

}
