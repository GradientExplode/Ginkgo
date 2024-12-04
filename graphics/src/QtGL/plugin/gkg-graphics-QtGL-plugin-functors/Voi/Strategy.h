#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Strategy_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Strategy_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/RoiCache.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <list>

#include <QMouseEvent>
#include <QCursor>
#include <QPointF>


namespace gkg
{


class VoiDrawA;


class VoiStrategy
{

  public:

    enum StgyType
    {
    
      StgyUndefined = 1,
      StgyAxial,
      StgyCoronal,
      StgySagittal,
      Stgy3D,
      StgyHistogram,
      StgyProfile
      
    };

    VoiStrategy();
    virtual ~VoiStrategy();

    virtual void setDrawA( VoiDrawA *d );

    virtual void paint( QPainter* painter, const QRectF& rect );

    virtual void mousePressEvent( VoiData&, QMouseEvent* );
    virtual void mouseReleaseEvent( VoiData&, QMouseEvent* );
    virtual void mouseMoveEvent( VoiData&, QMouseEvent* );
    virtual void wheelEvent( VoiData&, QWheelEvent* );
    virtual void keyPressEvent( VoiData&, QKeyEvent* event );

    virtual void updateDataLoaded( VoiData& );
    virtual void updateLabelLoaded( VoiData& );

    virtual void updateData( VoiData& );
    virtual void updatePalette( VoiData& );
    virtual void updateLabel( VoiData&, std::list< Vector3d< int32_t > >& );

    virtual void updateZoom();

    virtual void updateSlice( VoiData&, Vector3d< int32_t >&, int32_t );
    virtual void updateSlice( VoiData&, int32_t );
    virtual void updateFrame( VoiData&, int32_t );

    virtual void deltaSlice( VoiData&, int32_t );
    virtual void deltaFrame( VoiData&, int32_t );

    virtual void centerObjects();

    virtual void updateMode( VoiModeStrategy* strgy );
    virtual void leave();

    virtual QPoint mapFromScreen( float, float );
    virtual QPointF mapToScreen( int32_t, int32_t );
    virtual float distance( QPointF, QPointF );

    virtual void setIndicator( QPainter* painter );

    virtual StgyType type();

    float correctionH();
    float correctionV();
    float zoomFactor();
    virtual int32_t getSlice();

    virtual Vector3d< int32_t > click2Point4D( QPoint&, int32_t* t );
    virtual QPoint point4D2QPoint( Vector3d< int32_t >&, int32_t );

  protected:

    double correctH;
    double correctV;
    double zoomFact;
    QCursor tmpCursor;
    QPoint clickPos;
    QPoint depla;
    bool mouseDown;
    bool zoomDown;
    bool deplaDown;
    bool eraseDown;
    Vector3d< int32_t > point;
    int32_t t;
    VoiDrawA* da;
    VoiModeStrategy* modeStrategy;
    RoiCache _roiCache;

};


}


#endif
