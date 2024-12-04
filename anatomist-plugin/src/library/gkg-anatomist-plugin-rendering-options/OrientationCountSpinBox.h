#ifndef _gkg_anatomist_plugin_rendering_options_OrientationCountSpinBox_h_
#define _gkg_anatomist_plugin_rendering_options_OrientationCountSpinBox_h_


#include <QMap>
#include <QSpinBox>
#include <QLineEdit>
#include <map>


namespace gkg
{


class OrientationCountSpinBox : public QSpinBox
{

  Q_OBJECT

  public:

    OrientationCountSpinBox( QWidget* parent = 0 );

    void setValueFromIndex( int32_t value ) ;

  public slots:

    void setNewValue( int value );

  private:

    int32_t _index;
    std::map< int32_t, int32_t > _availableOrientationCounts;



};


}


#endif
