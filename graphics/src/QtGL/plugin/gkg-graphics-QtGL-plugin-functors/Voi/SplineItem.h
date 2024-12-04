#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_SplineItem_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_SplineItem_h_


#include <gkg-processing-colorimetry/RGBComponent.h>

#include <QGraphicsPathItem>


namespace gkg
{


class Node;


class SplineItem : public QGraphicsPathItem
{

  public:

    SplineItem( QGraphicsItem* parent = 0 );

    void setColor( RGBComponent color );

    void addKnot( Node* node );
    void insertKnot( Node* node );
    void removeKnot( Node* node );
    void closeSpline();

    QList< Node* >& getKnots();
    QPolygonF& getPolygon();

    void clear();
    int32_t size();

    void update();

  protected:

    QList< Node* > _knots;
    RGBComponent _color;
    QPolygonF _polygon;

};


}


#endif
