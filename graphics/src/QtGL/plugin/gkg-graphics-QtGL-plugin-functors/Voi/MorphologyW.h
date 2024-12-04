#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_MorphologyW_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_MorphologyW_h_


#include <QGroupBox>


namespace gkg
{


class VoiObservable;
class VoiData;


class MorphologyW : public QGroupBox
{

  Q_OBJECT

  public:

    MorphologyW( VoiObservable*, QWidget* p = 0 );

  public slots:

    void radiusChange( double );
    void setOperation( int );

  private:

    VoiData* _data;

};


}


#endif
