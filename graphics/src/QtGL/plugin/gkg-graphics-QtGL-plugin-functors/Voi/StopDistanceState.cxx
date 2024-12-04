#include <gkg-graphics-QtGL-plugin-functors/Voi/StopDistanceState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/StartDistanceState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>

#include <QGraphicsSceneMouseEvent>


gkg::StopDistanceState::StopDistanceState( gkg::GraphicsScene* scene )
                      : gkg::EventState( scene )
{
}


void gkg::StopDistanceState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{

  if ( ( event->button() == Qt::LeftButton ) ||
       ( event->button() == Qt::RightButton ) )
  {

    event->accept();
    _scene->setState( new gkg::StartDistanceState( _scene ) );
    _scene->acceptDistance( ( event->button() == Qt::LeftButton ) ? 
                            true : 
                            false );

  }

}


void gkg::StopDistanceState::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

  _scene->drawLineTo( event->scenePos() );
  event->accept();

}
