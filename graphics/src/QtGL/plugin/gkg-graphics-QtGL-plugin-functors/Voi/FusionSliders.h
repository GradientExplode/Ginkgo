#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_FusionSliders_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_FusionSliders_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <QWidget>

#include <string>


namespace gkg
{


class FusionSliders : public QWidget
{

  Q_OBJECT

  public:

    FusionSliders( VoiObservable*, QWidget* parent = NULL );

  private slots:

    void setFusionLabel( int );
    void setFusionBaseVolume( int );

  private:

    VoiData* data;

};


}


#endif
