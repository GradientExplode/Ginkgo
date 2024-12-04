#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_ToolBar_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_ToolBar_h_


#include <QWidget>


namespace gkg
{


class ToolBar : public QWidget
{

  Q_OBJECT

  public:

    ToolBar( QWidget* parent = 0 );

  signals:

    void changeView( int type );

  protected slots:

    void buttonClick( int id );

};


}


#endif
