#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_CoordinateSpinBoxes_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_CoordinateSpinBoxes_h_


#include <gkg-graphics-QtGL-plugin-functors/Voi/Observable.h>

#include <QWidget>


class QSpinBox;


namespace gkg
{


class VoiData;


class CoordinateSpinBoxes : public QWidget, public VoiObserver
{

  Q_OBJECT

  public:

    CoordinateSpinBoxes( VoiObservable*, QWidget* parent = NULL );

    void updateDataLoaded();
    void updateLabelLoaded();
    void updateSlice( Vector3d< int32_t >&, int32_t );

  private slots:

    void setX( int );
    void setY( int );
    void setZ( int );

  private:

    VoiData* _data;
    Vector3d< int32_t > _coordinate;
    int32_t _t;

    QSpinBox* _spinX;
    QSpinBox* _spinY;
    QSpinBox* _spinZ;

};


}


#endif
