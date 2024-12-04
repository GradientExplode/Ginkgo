#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_EvolInfoW_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_EvolInfoW_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ComboBox.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Slider.h>

#include <QGroupBox>
#include <QSpinBox>

#include <string>


namespace gkg
{


class VoiEvolutionInfoW : public QGroupBox
{

  Q_OBJECT

  public:

    VoiEvolutionInfoW( VoiObservable *, QWidget *p = NULL );
    ~VoiEvolutionInfoW();

  public slots:

    void dimChange( int );
    void changeQRadio( int );
    void pctChange( int );

  private:

    VoiData *data;
    QSpinBox *sb;
    VoiSlider *sl;
    VoiComboBox *cbConnex;

};


}


#endif
