#ifndef _gkg_anatomist_plugin_rendering_options_APointCloudFieldObjectRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_APointCloudFieldObjectRenderingOptionsWidget_h_


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
#include <QCheckBox>




namespace gkg
{


class APointCloudFieldObjectRenderingOptionsWidget : public QWidget,
                                                     public anatomist::Observer
{

  Q_OBJECT

  public:

    APointCloudFieldObjectRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent = 0,
                                 const char* name = 0 );
    virtual ~APointCloudFieldObjectRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:

    void vertexCountPerSphereChanged( int );
    void pointRadiusChanged( double );
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


    int32_t _vertexCountPerSphere;
    float _pointRadius;
    float _meshScale;

    int32_t _originalVertexCountPerSphere;
    float _originalPointRadius;
    float _originalMeshScale;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QSpinBox* _vertexCountPerSphereSpinBox;
    QDoubleSpinBox* _pointRadiusDoubleSpinBox;
    QDoubleSpinBox* _meshScaleDoubleSpinBox;

};


}


#endif
