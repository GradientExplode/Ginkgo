#ifndef _gkg_graphics_QtGL_plugin_functors_Voi_Slider_h_
#define _gkg_graphics_QtGL_plugin_functors_Voi_Slider_h_


#include <gkg-core-cppext/StdInt.h>

#include <string>

#include <QWidget>
#include <QSlider>
#include <QLabel>


namespace gkg
{


class VoiSlider : public QWidget
{

  Q_OBJECT

  public:

    VoiSlider( QWidget* parent = 0, const std::string& n = "" );

    void setRange( int32_t, int32_t );
    void setValue( int32_t );
    void setSteps( int32_t, int32_t );
    void updateValue( int32_t );

    int32_t value() { return sld->value(); }

  public slots:

    void change( int );

  signals:

    void valueChanged( int );

  private:

    QSlider *sld;
    QLabel *lcd;

};


}


#endif
