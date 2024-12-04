#ifndef _gkg_anatomist_plugin_rendering_options_APointCloudObjectRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_APointCloudObjectRenderingOptionsWidget_h_


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




namespace gkg
{


class APointCloudObjectRenderingOptionsWidget : public QWidget,
                                                public anatomist::Observer
{

  Q_OBJECT

  public:

    APointCloudObjectRenderingOptionsWidget(
                               const std::set< anatomist::AObject* >& objects,
                               QWidget* parent = 0,
                               const char* name = 0 );
    virtual ~APointCloudObjectRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:

    void sphereVertexCountChanged( int );
    void sphereRadiusChanged( double );
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


    int32_t _sphereVertexCount;
    float _sphereRadius;

    int32_t _originalSphereVertexCount;
    float _originalSphereRadius;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QSpinBox* _sphereVertexCountSpinBox;
    QDoubleSpinBox* _sphereRadiusDoubleSpinBox;

};


}


#endif
