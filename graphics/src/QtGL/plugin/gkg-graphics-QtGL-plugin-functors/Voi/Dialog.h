#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Dialog_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Dialog_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Observable.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/BrushSizeW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolInfoW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/MorphologyW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/CylinderW.h>

#include <QWidget>


namespace gkg
{


class VoiDialog : public QWidget
{

  Q_OBJECT

  public:

    VoiDialog( VoiObservable*, QWidget* parent = 0 );

  public slots:

    void show( int );

  private:

    VoiBrushSizeW* binfo;
    VoiEvolutionInfoW* einfo;
    MorphologyW* minfo;
    CylinderW* cinfo;

};


}


#endif
