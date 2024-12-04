#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_CylinderW_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_CylinderW_h_


#include <QGroupBox>


namespace gkg
{


class VoiObservable;
class VoiData;


class CylinderW : public QGroupBox
{

  Q_OBJECT

  public:

    CylinderW( VoiObservable*, QWidget* p = 0 );

  public slots:

    void diameterChange( int );
    void setCylinderOrientation( int );

  private:

    VoiData* _data;

};


}


#endif
