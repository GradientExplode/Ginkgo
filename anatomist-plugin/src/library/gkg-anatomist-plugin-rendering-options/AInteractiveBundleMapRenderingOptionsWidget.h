#ifndef _gkg_anatomist_plugin_rendering_options_AInteractiveBundleMapRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_AInteractiveBundleMapRenderingOptionsWidget_h_


#include <anatomist/observer/Observer.h>
#include <anatomist/observer/Observable.h>
#include <anatomist/object/Object.h>
#include <anatomist/object/objectparamselect.h>

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QStackedWidget>


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

    void fiberSubSamplingFactorChanged( int );
    void fiberLineWidthChanged( double );
    void fiberColorEncodingChanged( int );
    void fileNameScalarFeatureVolumeChanged( const QString& );
    void selectFileNameScalarFeatureVolume();
    void fileNameScalarFeatureVolumeToBundleTransform3dChanged(
                                                               const QString& );
    void selectFileNameScalarFeatureVolumeToBundleTransform3d();
    void lowerBoundChanged( const QString& );
    void upperBoundChanged( const QString& );
    void cancel();
    void accept();

    void chooseObject();
    void objectsChosen( const std::set< anatomist::AObject* >& objects );

  protected:

    void unregisterObservable( anatomist::Observable* observable );
    void updateObjects();
    void drawContents();
    void runCommand();

    std::set< anatomist::AObject* > _objects;
    bool _operating;
    std::set< anatomist::AObject* > _initialObjects;
    bool _modified;

    int32_t _fiberSubSamplingFactor;
    float _fiberLineWidth;
    int32_t _fiberColorEncoding;
    std::string _fileNameScalarFeatureVolume;
    std::string _fileNameScalarFeatureVolumeToBundleTransform3d;
    QString _lowerBoundString;
    QString _upperBoundString;

    int32_t _originalFiberSubSamplingFactor;
    float _originalFiberLineWidth;
    int32_t _originalFiberColorEncoding;
    std::string _originalFileNameScalarFeatureVolume;
    std::string _originalFileNameScalarFeatureVolumeToBundleTransform3d;
    QString _originalLowerBoundString;
    QString _originalUpperBoundString;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QSpinBox* _fiberSubSamplingFactorSpinBox;
    QDoubleSpinBox* _fiberLineWidthDoubleSpinBox;
    QComboBox* _fiberColorEncodingComboBox;
    QStackedWidget* _fiberColorEncodingStackedWidget;
    QLineEdit* _fileNameScalarFeatureVolumeLineEdit;
    QPushButton* _fileNameScalarFeatureVolumePushButton;
    QLineEdit* _fileNameScalarFeatureVolumeToBundleTransform3dLineEdit;
    QPushButton* _fileNameScalarFeatureVolumeToBundleTransform3dPushButton;
    QLineEdit* _lowerBoundLineEdit;
    QLineEdit* _upperBoundLineEdit;

};


}


#endif
