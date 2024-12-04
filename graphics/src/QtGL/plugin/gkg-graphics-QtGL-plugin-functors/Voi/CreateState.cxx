#include <gkg-graphics-QtGL-plugin-functors/Voi/CreateState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ValidateState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/GraphicsScene.h>

#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QApplication>


gkg::CreateState::CreateState( gkg::GraphicsScene* scene )
                : gkg::EventState( scene )
{
}


void gkg::CreateState::mousePressEvent( QGraphicsSceneMouseEvent* event )
{

  if ( event->button() == Qt::LeftButton )
  {

    if ( QApplication::keyboardModifiers() & Qt::ShiftModifier )
    {

      _scene->move( event );

    }
    else
    {

      _scene->addNode( event->scenePos() );
      event->accept();

    }

  }
  else if ( event->button() == Qt::RightButton )
  {

    _scene->undo();
    event->accept();

  }
  else if ( event->button() == Qt::MiddleButton )
  {

    if ( _scene->closeCurve() )
    {

      event->accept();
      _scene->setState( new gkg::ValidateState( _scene ) );

    }

  }

}


void gkg::CreateState::keyPressEvent( QKeyEvent* event )
{

  if ( ( event->key() == Qt::Key_V ) && 
       ( QApplication::keyboardModifiers() & Qt::ControlModifier ) )
  {

    _scene->copySpline();
    event->accept();

  }
  else if ( event->key() == Qt::Key_Escape )
  {

    _scene->removeLastNode();
    event->accept();

  }

}
