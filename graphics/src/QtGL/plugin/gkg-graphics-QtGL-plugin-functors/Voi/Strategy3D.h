#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Strategy3D_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Strategy3D_h_

#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VoxelEngine.h>
#include <gkg-graphics-QtGL-trackball/Trackball.h>

#include <GL/gl.h>


namespace gkg
{


class Voi3DStrategy : public VoiStrategy
{

  public:

    Voi3DStrategy();

    void setDrawA( VoiDrawA *d );
    
    void paint( QPainter* painter, const QRectF& rect );

    void updateDataLoaded( VoiData& );
    void updateLabelLoaded( VoiData& );
    void updateLabel( VoiData&, std::list< Vector3d< int32_t > >& );
    void updatePalette( VoiData& );
    void updateSlice( VoiData&, Vector3d< int32_t >&, int32_t );

    void mousePressEvent( VoiData&, QMouseEvent* );
    void mouseReleaseEvent( VoiData&, QMouseEvent* );
    void mouseMoveEvent( VoiData&, QMouseEvent* );
    void wheelEvent( VoiData&, QWheelEvent* );

    void centerObjects();
    void leave();

    VoiStrategy::StgyType type();

  private:

    void drawCube();
    void drawAxial();
    void drawSagittal();
    void drawCoronal();
    void drawCylinder();
    void linkedCursor( VoiData& );

    Trackball< float > trackball;
    VoxelEngine _voxelEngine;
    VoiData* _data;

    Trackball< float > trackball2;
    QPoint clickPos2;
    bool mouseDown2;

    GLfloat _fov;
    GLfloat _panX;
    GLfloat _panY;
    GLfloat _panX2;
    GLfloat _panY2;
    GLfloat _panZ2;

};


}


#endif
