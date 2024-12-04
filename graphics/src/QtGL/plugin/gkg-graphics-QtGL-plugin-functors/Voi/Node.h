#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Node_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Node_h_


#include <QGraphicsEllipseItem>
#include <QPointF>


namespace gkg
{


class SplineItem;


class Node : public QGraphicsEllipseItem 
{
  public:

    Node( SplineItem* splineItem, QPointF pos );

  protected:

    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );

    virtual QVariant itemChange( GraphicsItemChange change,
                                 const QVariant& value );

  private:

    SplineItem* spline;

};


}


#endif
