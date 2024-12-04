#ifndef _gkg_anatomist_plugin_rendering_options_AOdfFieldRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_AOdfFieldRenderingOptionsWidget_h_


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


class AOdfFieldRenderingOptionsWidget : public QWidget,
                                        public anatomist::Observer
{

  Q_OBJECT

  public:

    AOdfFieldRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent = 0,
                                 const char* name = 0 );
    virtual ~AOdfFieldRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:

    void vertexCountPerOdfChanged( int );
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


    int32_t _vertexCountPerOdf;
    float _meshScale;

    int32_t _originalVertexCountPerOdf;
    float _originalMeshScale;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QSpinBox* _vertexCountPerOdfSpinBox;
    QDoubleSpinBox* _meshScaleDoubleSpinBox;

};


}


#endif
