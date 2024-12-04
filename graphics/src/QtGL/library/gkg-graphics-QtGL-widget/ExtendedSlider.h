#ifndef _gkg_graphics_QtGL_widget_ExtendedSlider_h_
#define _gkg_graphics_QtGL_widget_ExtendedSlider_h_


#include <QWidget>

#include <string>
#include <limits>


class QLabel;
class QSlider;


namespace gkg
{


class ExtendedSlider : public QWidget
{

  Q_OBJECT

  public:

    ExtendedSlider( Qt::Orientation orientation, 
                    QWidget* parent = 0,
                    const std::string& name = "",
                    float resolution = 1000.0,
                    float min = std::numeric_limits< float >::lowest(),
                    float max = std::numeric_limits< float >::max(),
                    float val = 0 );

    float getResolution();

    void setRange( float min, float max );

    void setPageStep( float step );
    void setSingleStep( float step );
    float getSingleStep();

    float getMaximum();
    float getMinimum();
    float getValue() const;

    void setSliderPosition( float );
    float getSliderPosition();

    QSlider* getSlider() const;

    void setEnabled( bool enable );
    void show();

  public slots:

    void updateValue( int value );
    void setValue( float value );

  signals:

    void valueChanged( float value );

  private:

    float _minimum;
    float _maximum;
    float _value;
    float _resolution;
    int32_t _precision;

    QSlider* _slider;
    QLabel* _labelValue;
    QLabel* _labelMin;
    QLabel* _labelMax;

};


}


#endif
