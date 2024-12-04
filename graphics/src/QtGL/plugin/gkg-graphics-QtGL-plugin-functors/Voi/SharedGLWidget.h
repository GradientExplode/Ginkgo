#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_SharedGLWidget_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_SharedGLWidget_h_


#include <QGLWidget>


class QGLFormat;


namespace gkg
{


class SharedGLWidget : public QGLWidget
{

  public:

    SharedGLWidget( const QGLFormat& format, QWidget* parent = 0 );
    virtual ~SharedGLWidget();

};


}


#endif
