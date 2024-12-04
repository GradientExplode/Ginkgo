#include <gkg-graphics-QtGL-plugin-functors/Voi/Node.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineItem.h>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QBrush>

 
gkg::Node::Node( gkg::SplineItem* splineItem, QPointF pos )
         : QGraphicsEllipseItem( -5, -5, 11, 11 ),
           spline( splineItem )
{

  setPos( pos );
  setZValue( 1 );
  setFlag( QGraphicsItem::ItemIsMovable );
  setFlag( QGraphicsItem::ItemSendsGeometryChanges );
  setBrush( QBrush( Qt::green ) );
  setRect( -5, -5, 11, 11 );

}

void gkg::Node::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{

  if ( scene()->sceneRect().contains( event->scenePos() ) )
  {

    QGraphicsEllipseItem::mouseMoveEvent( event );

  }

}
 
QVariant gkg::Node::itemChange( QGraphicsItem::GraphicsItemChange change,
                                const QVariant& value )
{

  if ( change == ItemPositionChange ) 
  {

    spline->update();

  }

  return QGraphicsEllipseItem::itemChange( change, value );

}
