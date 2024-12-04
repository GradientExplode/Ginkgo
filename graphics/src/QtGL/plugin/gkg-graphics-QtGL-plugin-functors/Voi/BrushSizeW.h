#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_BrushSizeW_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_BrushSizeW_h_


#include <string>

#include <QWidget>


namespace gkg
{


class VoiObservable;
class VoiData;


class VoiBrushSizeW : public QWidget
{

  Q_OBJECT

  public:

    VoiBrushSizeW( VoiObservable *, QWidget *par = NULL );

  private slots:

    void setSize( int );

  private:

    VoiData *data;
    
};


}


#endif
