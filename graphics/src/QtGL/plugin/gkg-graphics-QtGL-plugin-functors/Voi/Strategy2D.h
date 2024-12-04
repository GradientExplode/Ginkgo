#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Strategy2D_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Strategy2D_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ContrastStrategy.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <QMouseEvent>
#include <QImage>


namespace gkg
{


class Voi2DStrategy : public VoiStrategy
{

  public:

    Voi2DStrategy();

    void paint( QPainter* painter, const QRectF& rect );

    void updatePalette( VoiData& );
    void updateDataLoaded( VoiData& );
    void updateZoom();
    void updateFrame( VoiData&, int32_t );
    void deltaFrame( VoiData&, int32_t );

    void mousePressEvent( VoiData&, QMouseEvent * );
    void mouseReleaseEvent( VoiData&, QMouseEvent * );
    void mouseMoveEvent( VoiData&, QMouseEvent * );
    void keyPressEvent( VoiData&, QKeyEvent* event );

    QPoint mapFromScreen( float, float );
    QPointF mapToScreen( int32_t, int32_t );
    float distance( QPointF p1, QPointF p2 );

  protected:

    float zoomLimit;
    QImage buffer;
    QPoint orig;
    VoiContrastStrategy contrastStrategy;
    bool contrastDown;
    virtual void initialize( VoiData& ) = 0;
    virtual void copyRoi( VoiData& d, int32_t label ) = 0;
    virtual void pasteRoi( VoiData& d, int32_t label ) = 0;

  private:

    void clipping();

};


}


#endif
