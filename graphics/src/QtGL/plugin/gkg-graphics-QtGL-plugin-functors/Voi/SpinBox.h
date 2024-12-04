#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_SpinBox_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_SpinBox_h_


#include <string>

#include <QSpinBox>


namespace gkg
{


class VoiSpinBox : public QSpinBox
{

  Q_OBJECT

  public:

    VoiSpinBox( int32_t, QWidget *p = NULL, const std::string& n = "" );
    VoiSpinBox( int32_t, int32_t, int32_t, int32_t st=1, 
                QWidget *p = NULL, const std::string& n = "" );

  public slots:

    void valChange( int );

  signals:

    void valueChanged( float );

  private:

    int32_t prec;

};


}


#endif
