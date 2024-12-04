#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_GraphicsScene_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_GraphicsScene_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/EventState.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SplineCache.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>

#include <map>


namespace gkg
{


class VoiDrawA;
class VoiData;
class SplineItem;


class GraphicsScene : public QGraphicsScene
{

  Q_OBJECT

  public:

    GraphicsScene( VoiDrawA* da, VoiData* data );
    ~GraphicsScene();

    void drawBackground( QPainter* painter, const QRectF& rect );
    void clearScene();

    void setMode( gkg::VoiModeStrategy::ModeId id );
    void setState( EventState* state );

    void addNode( QPointF pos );
    void insertNode( QPointF pos );
    bool removeNode( QPointF pos );
    void removeLastNode();
    bool closeCurve();
    void move( QGraphicsSceneMouseEvent* event );
    void validateSpline();
    void undo();
    void copySpline();

    void addFirstPoint( QPointF pos );
    void drawLineTo( QPointF pos );
    void acceptDistance( bool accept );

  public slots:

    void setItemColor( int );

  protected:

    void mousePressEvent( QGraphicsSceneMouseEvent* event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent* event );
    void keyPressEvent( QKeyEvent* event );

  private:

    SplineCache _splineCache;
    EventState* _state;
    SplineItem* _splineItem;
    VoiDrawA* _da;
    VoiData* _data;
    int32_t _splineSlice;
    int32_t _splineLabel;

    bool _distanceMeasure;
    QPointF _startDistance;
    QGraphicsLineItem* _distanceLine;
    QGraphicsSimpleTextItem* _distanceText;
    std::map< QGraphicsLineItem*, QGraphicsSimpleTextItem* > _distances;

};


}


#endif
