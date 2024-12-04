#include <gkg-graphics-QtGL-plugin-functors/Voi/ValidateState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/CreateState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>

#include <QGraphicsSceneMouseEvent>
#include <QApplication>


gkg::ValidateState::ValidateState( gkg::GraphicsScene* scene )
                  : gkg::EventState( scene )
{
}


void gkg::ValidateState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{

  if ( event->button() == Qt::LeftButton )
  {

    if ( QApplication::keyboardModifiers() & Qt::ShiftModifier )
    {

      _scene->move( event );

    }
    else
    {

      _scene->insertNode( event->scenePos() );
      event->accept();

    }

  }
  else if ( event->button() == Qt::RightButton )
  {

    if ( _scene->removeNode( event->scenePos() ) )
    {

      event->accept();

    }

  }
  else if ( event->button() == Qt::MiddleButton )
  {

    _scene->validateSpline();
    event->accept();
    _scene->setState( new gkg::CreateState( _scene ) );

  }

}
