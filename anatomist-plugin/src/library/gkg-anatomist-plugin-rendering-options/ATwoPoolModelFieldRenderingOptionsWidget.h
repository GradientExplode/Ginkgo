#ifndef _gkg_anatomist_plugin_rendering_options_ATwoPoolModelFieldRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_ATwoPoolModelFieldRenderingOptionsWidget_h_


#include <anatomist/observer/Observer.h>
#include <anatomist/observer/Observable.h>
#include <anatomist/object/Object.h>
#include <anatomist/object/objectparamselect.h>
#include <gkg-anatomist-plugin-rendering-options/OrientationCountSpinBox.h>
#include <QWidget>
#include <QRadioButton>
#include <QLabel>


namespace gkg
{


class ATwoPoolModelFieldRenderingOptionsWidget : public QWidget,
                                                 public anatomist::Observer
{

  Q_OBJECT

  public:

    ATwoPoolModelFieldRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent = 0,
                                 const char* name = 0 );
    virtual ~ATwoPoolModelFieldRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:

    void outputOrientationCountChanged( int );
    void hideShadowsChanged( int );
    void normalDisplayChanged( bool );
    void meshScaleChanged( double );
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

    float _meshScale;
    int32_t _outputOrientationCount;
    bool _hasApproximatedNormals;
    bool _hasHiddenShadows;
    bool _realNormalsAlreadyComputed;

    int32_t _originalOutputOrientationCount;
    bool _originalHasApproximatedNormals;
    bool _originalHasHiddenShadows;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QDoubleSpinBox* _meshScaleDoubleSpinBox;
    OrientationCountSpinBox* _outputOrientationCountSpinBox;
    QRadioButton* _hasApproximatedNormalsRadioButton;
    QRadioButton* _hasRealNormalsRadioButton;
    QLabel* _hasRealNormalsLabel;

};


}


#endif
