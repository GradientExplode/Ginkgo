#ifndef _gkg_anatomist_plugin_rendering_options_AFiberRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_AFiberRenderingOptionsWidget_h_


#include <anatomist/observer/Observer.h>
#include <anatomist/observer/Observable.h>
#include <anatomist/object/Object.h>
#include <anatomist/object/objectparamselect.h>
#include <QWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QSlider>


namespace gkg
{


class AFiberRenderingOptionsWidget : public QWidget,
                                     public anatomist::Observer
{

  Q_OBJECT

  public:

    AFiberRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent = 0,
                                 const char* name = 0 );
    virtual ~AFiberRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:

    void noDataDataTypeChanged( bool );
    void orientationDataTypeChanged( bool );
    void lengthDataTypeChanged( bool );
    void bundleProfileDataTypeChanged( bool );
    void colorBundleProfileDataTypeChanged( bool );
    void minimumRangeChanged( const QString& value );
    void maximumRangeChanged( const QString& value );
    void minimumSliderValueChanged( int value );
    void maximumSliderValueChanged( int value );
    void samplingValueChanged( int value );
    void fileNameDataChanged( const QString );
    void fileNameVolumeChanged( const QString );
    void cancel();
    void accept();

    void chooseObject();
    void objectsChosen( const std::set< anatomist::AObject* >& objects );

  protected:

    void unregisterObservable( anatomist::Observable* observable );
    void updateObjects();
    void drawContents();
    void runCommand();

    void setRange( const double minimumRange, const double maximumRange );

    std::set< anatomist::AObject* > _objects;
    bool _operating;
    std::set< anatomist::AObject* > _initialObjects;
    bool _modified;

    double _originalMinimumValue;
    double _originalMaximumValue;
    double _originalMinimumRange;
    double _originalMaximumRange;
    int32_t _originalSamplingValue;
    std::string _originalDataType;
    std::string _originalFileNameData;
    std::string _originalFileNameVolume;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QSlider* _minimumValueSlider;
    QSlider* _maximumValueSlider;
    QLabel* _minimumValueLabel;
    QLabel* _maximumValueLabel;
    QSpinBox* _samplingValueSpinBox;
    QLineEdit* _minimumRangeLineEdit;
    QLineEdit* _maximumRangeLineEdit;
    QLineEdit* _fileNameDataLineEdit;
    QLineEdit* _fileNameVolumeLineEdit;

    double _minimumValue;
    double _maximumValue;
    double _minimumRange;
    double _maximumRange;
    int32_t _samplingValue;
    std::string _dataType;
    std::string _fileNameData;
    std::string _fileNameVolume;

};


}


#endif
