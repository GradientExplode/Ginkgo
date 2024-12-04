#include <gkg-graphics-QtGL-plugin-functors/Voi/SharedGLWidget.h>

#include <QGLFormat>


gkg::SharedGLWidget::SharedGLWidget( const QGLFormat& format, QWidget* parent )
                   : QGLWidget( format, parent )
{
}


gkg::SharedGLWidget::~SharedGLWidget()
{
}
