#ifndef _gkg_anatomist_plugin_rendering_options_AMedusaGeometryObjectRenderingOptionsWidget_h_
#define _gkg_anatomist_plugin_rendering_options_AMedusaGeometryObjectRenderingOptionsWidget_h_


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


class AMedusaGeometryObjectRenderingOptionsWidget : public QWidget,
                                                    public anatomist::Observer
{

  Q_OBJECT

  public:

    AMedusaGeometryObjectRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent = 0,
                                 const char* name = 0 );
    virtual ~AMedusaGeometryObjectRenderingOptionsWidget();

    void update( const anatomist::Observable* observable, void* arg );

    static void create( const std::set< anatomist::AObject* >& objects );

  protected slots:

    void vertexCountPerSphereAtomChanged( int );
    void fiberPopulationVisibilityChanged( int );
    void somaPopulationVisibilityChanged( int );
    void neuronPopulationVisibilityChanged( int );
    void dendritePopulationVisibilityChanged( int );
    void astrocytePopulationVisibilityChanged( int );
    void oligodendrocytePopulationVisibilityChanged( int );
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


    int32_t _vertexCountPerSphereAtom;
    bool _fiberPopulationVisibility; 
    bool _somaPopulationVisibility;
    bool _neuronPopulationVisibility;
    bool _dendritePopulationVisibility;
    bool _astrocytePopulationVisibility;
    bool _oligodendrocytePopulationVisibility;

    int32_t _originalVertexCountPerSphereAtom;
    bool _originalFiberPopulationVisibility; 
    bool _originalSomaPopulationVisibility;
    bool _originalNeuronPopulationVisibility;
    bool _originalDendritePopulationVisibility;
    bool _originalAstrocytePopulationVisibility;
    bool _originalOligodendrocytePopulationVisibility;

    ObjectParamSelect* _objectParamSelect;

    QWidget* _mainWindow;
    QSpinBox* _vertexCountPerSphereAtomSpinBox;
    QCheckBox* _fiberPopulationVisibilityCheckBox;
    QCheckBox* _somaPopulationVisibilityCheckBox;
    QCheckBox* _neuronPopulationVisibilityCheckBox;
    QCheckBox* _dendritePopulationVisibilityCheckBox;
    QCheckBox* _astrocytePopulationVisibilityCheckBox;
    QCheckBox* _oligodendrocytePopulationVisibilityCheckBox;

};


}


#endif
