#ifndef _gkg_anatomist_plugin_rendering_options_AInteractiveBundleMapRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_AInteractiveBundleMapRenderingOptionsWidget_h_


#include <anatomist/observer/Observer.h>
#include <anatomist/observer/Observable.h>
#include <anatomist/object/Object.h>
#include <anatomist/object/objectparamselect.h>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QStackedWidget>
#include <QString>
#include <QWidget>
#include <qwt_counter.h>

#include <cmath>


namespace gkg
{


class AInteractiveBundleMapRenderingOptionsWidget : public QWidget,
                                                 public anatomist::Observer
{

  Q_OBJECT

  public:

    AInteractiveBundleMapRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent = 0,
                                 const char* name = 0 );
    virtual ~AInteractiveBundleMapRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:


    // color encoding methods
    void fiberLineWidthChanged( double );
    void fiberColorEncodingChanged( int );
    void fileNameScalarFeatureVolumeChanged( const QString& );
    void selectFileNameScalarFeatureVolume();
    void fileNameScalarFeatureVolumeToBundleTransform3dChanged(
                                                               const QString& );
    void selectFileNameScalarFeatureVolumeToBundleTransform3d();
    void lowerBoundChanged( const QString& );
    void upperBoundChanged( const QString& );
    
    // level of detail methods
    void minRatioPercentChanged( double );
    void maxRatioPercentChanged( double );
    void maxLevelOfDetailChanged( int );
    void numberOfCubesPerAxisChanged( int );
    void preloadNewLevelOfDetailDone();
    void currentLevelOfDetailChanged( int );

    // resampling methods
    void fiberSubSamplingFactorChanged( int );
    void enableResamplingChanged( int );
    void resamplingMaximumStepChanged( double );
    void resamplingErrorThresholdChanged( double );
    void preloadNewResamplingDone();


    void cancel();
    void accept();

    void chooseObject();
    void objectsChosen( const std::set< anatomist::AObject* >& objects );

  protected:

    void unregisterObservable( anatomist::Observable* observable );
    void updateObjects();
    void drawContents();
    void runCommand();

    void updateLevelOfDetailPreload();
    void updateResamplingPreload();
    void setRatiosPercent();

    std::set< anatomist::AObject* > _objects;
    bool _operating;
    std::set< anatomist::AObject* > _initialObjects;
    bool _modified;


    // color encoding attributes
    float _fiberLineWidth;
    int32_t _fiberColorEncoding;
    std::string _fileNameScalarFeatureVolume;
    std::string _fileNameScalarFeatureVolumeToBundleTransform3d;
    QString _lowerBoundString;
    QString _upperBoundString;
    
    // level of detail attributes
    double _minRatioPercent;
    double _maxRatioPercent;
    int32_t _maxLevelOfDetail;
    int32_t _numberOfCubesPerAxis;
    int32_t _currentLevelOfDetail;
    std::vector< double > _ratiosPercent;
    double _currentPercent;
    int32_t _currentPercentInt;

    // resampling attributes
    int32_t _fiberSubSamplingFactor;
    bool _enableResampling;
    double _resamplingMaximumStep;
    double _resamplingErrorThreshold;
    

    // original color encoding attributes
    float _originalFiberLineWidth;
    int32_t _originalFiberColorEncoding;
    std::string _originalFileNameScalarFeatureVolume;
    std::string _originalFileNameScalarFeatureVolumeToBundleTransform3d;
    QString _originalLowerBoundString;
    QString _originalUpperBoundString;
    
    // original level of detail attributes
    double _originalMinRatioPercent;
    double _originalMaxRatioPercent;
    int32_t _originalMaxLevelOfDetail;
    int32_t _originalNumberOfCubesPerAxis;
    int32_t _originalCurrentLevelOfDetail;

    // original resampling attributes
    int32_t _originalFiberSubSamplingFactor;
    bool _originalEnableResampling;
    double _originalResamplingMaximumStep;
    double _originalResamplingErrorThreshold;


    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;


    // color encoding boxes
    QDoubleSpinBox* _fiberLineWidthDoubleSpinBox;
    QComboBox* _fiberColorEncodingComboBox;
    QStackedWidget* _fiberColorEncodingStackedWidget;
    QLineEdit* _fileNameScalarFeatureVolumeLineEdit;
    QPushButton* _fileNameScalarFeatureVolumePushButton;
    QLineEdit* _fileNameScalarFeatureVolumeToBundleTransform3dLineEdit;
    QPushButton* _fileNameScalarFeatureVolumeToBundleTransform3dPushButton;
    QLineEdit* _lowerBoundLineEdit;
    QLineEdit* _upperBoundLineEdit;
    
    // level of detail boxes
    QwtCounter* _minRatioPercentCounter;
    QwtCounter* _maxRatioPercentCounter;
    QSpinBox* _maxLevelOfDetailSpinBox;
    QSpinBox* _numberOfCubesPerAxisSpinBox;
    QPushButton* _preloadNewLevelOfDetailPushButton;
    QSpinBox* _currentLevelOfDetailSpinBox;
    QSlider* _currentLevelOfDetailSlider;
    QProgressBar* _currentLevelOfDetailProgressBar;

    // resampling boxes
    QSpinBox* _fiberSubSamplingFactorSpinBox;
    QCheckBox* _enableResamplingCheckBox;
    QDoubleSpinBox* _resamplingMaximumStepDoubleSpinBox;
    QDoubleSpinBox* _resamplingErrorThresholdDoubleSpinBox;
    QPushButton* _preloadNewResamplingPushButton;
    
};


}


#endif
