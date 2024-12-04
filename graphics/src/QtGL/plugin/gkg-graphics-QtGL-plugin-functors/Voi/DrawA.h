#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_DrawA_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_DrawA_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Observable.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeStrategy.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QGraphicsView>
#include <QGLWidget>
#include <QPointF>

#include <string>
#include <list>


class QGLFormat;


namespace gkg
{


class ToolBar;
class ScrollBars;
class GraphicsScene;


class VoiDrawA : public QGraphicsView, public VoiObserver
{

  Q_OBJECT

  public:

    VoiDrawA( VoiStrategy::StgyType, VoiObservable *, 
              const QGLFormat& format,
              const QGLWidget* shareWidget, QWidget *parent = 0 );

    void updateDataLoaded();
    void updateLabelLoaded();
    void updateData();
    void updatePalette();
    void updateLabel( std::list< Vector3d< int32_t > >& );
    void updateScrollBar();
    void updateZoom();
    void updateSlice( Vector3d< int32_t >&, int32_t );
    void updateFrame( int );
    void redrawView();
    void updateMode( VoiModeStrategy * );

    void paint( QPainter* painter, const QRectF& rect );

    QPoint mapFromScreen( QPointF );
    QPoint mapFromScreen( float, float );
    QPointF mapToScreen( QPoint );
    QPointF mapToScreen( int32_t, int32_t );
    float distance( QPointF, QPointF );
    int32_t getCurrentSlice();

  public slots:

    void increase();
    void decrease();
    void incFrame();
    void decFrame();
    void updateSlice( int );
    void updateOneFrame( int );

  protected slots:

    void muteWidget( int type );

  protected:

    void resizeEvent( QResizeEvent* );
    void wheelEvent( QWheelEvent* );
    void mousePressEvent( QMouseEvent* );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );
    void keyPressEvent( QKeyEvent* event );
    void leaveEvent( QEvent* e );

  private:

    VoiStrategy *strategy;
    std::vector< VoiStrategy* > _strategies;
    ToolBar* _toolBar;
    ScrollBars* _scrollBars;
    GraphicsScene* _scene;

};


}


#endif
