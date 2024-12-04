#include <gkg-anatomist-plugin-rendering-options/AMedusaGeometryObjectRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/AMedusaGeometryObject.h>
#include <gkg-core-exception/Exception.h>
#include <anatomist/application/settings.h>
#include <anatomist/application/Anatomist.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <QFile>
#include <QUiLoader>
#include <QPixmap>
#include <QGridLayout>
#include <QGroupBox>
#include <QLayout>
#include <QFileDialog>



gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                                    AMedusaGeometryObjectRenderingOptionsWidget(
                                 const std::set< anatomist::AObject* >& objects,
                                 QWidget* parent,
                                 const char* name )
                                                 : QWidget( parent ),
                                                   _objects( objects ),
                                                   _operating( false ),
                                                   _initialObjects( objects ),
                                                   _modified( false )
{

  try
  {

    setObjectName( name );
    setAttribute( Qt::WA_DeleteOnClose );
    
    _vertexCountPerSphereAtom = 60; 
    _fiberPopulationVisibility = true; 
    _somaPopulationVisibility = true;
    _neuronPopulationVisibility = true;
    _dendritePopulationVisibility = true;
    _astrocytePopulationVisibility = true;
    _oligodendrocytePopulationVisibility = true;
    
    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::AMedusaGeometryObject* 
      aMedusaGeometryObject = dynamic_cast< gkg::AMedusaGeometryObject* >( *o );
      if ( aMedusaGeometryObject )
      {

        _vertexCountPerSphereAtom =
                aMedusaGeometryObject->getVertexCountPerSphereAtom();
        _fiberPopulationVisibility =
                aMedusaGeometryObject->getFiberPopulationVisibility();
        _somaPopulationVisibility =
                aMedusaGeometryObject->getSomaPopulationVisibility();
        _neuronPopulationVisibility =
                aMedusaGeometryObject->getNeuronPopulationVisibility();
        _dendritePopulationVisibility = 
                aMedusaGeometryObject->getDendritePopulationVisibility();
        _astrocytePopulationVisibility =
                aMedusaGeometryObject->getAstrocytePopulationVisibility();
        _oligodendrocytePopulationVisibility =
                aMedusaGeometryObject->getOligodendrocytePopulationVisibility();
        
        break;

      }
      aMedusaGeometryObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    _originalVertexCountPerSphereAtom = _vertexCountPerSphereAtom;
    _originalFiberPopulationVisibility = _fiberPopulationVisibility;
    _originalSomaPopulationVisibility = _somaPopulationVisibility;
    _originalNeuronPopulationVisibility = _neuronPopulationVisibility;
    _originalDendritePopulationVisibility = _dendritePopulationVisibility;
    _originalAstrocytePopulationVisibility = _astrocytePopulationVisibility;
    _originalOligodendrocytePopulationVisibility =
                                           _oligodendrocytePopulationVisibility;

    this->setWindowTitle( name );

    if ( !parent )
    {

      QPixmap anatomistIcon( 
            anatomist::Settings::findResourceFile( "icons/icon.xpm" ).c_str() );
      if ( !anatomistIcon.isNull() )
      {

        this->setWindowIcon( anatomistIcon );

      }

    }

    drawContents();

  }
  GKG_CATCH( "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "AMedusaGeometryObjectRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                                  ~AMedusaGeometryObjectRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::update(
                                                   const anatomist::Observable*,
                                                   void* /* arg */ )
{
}

void gkg::AMedusaGeometryObjectRenderingOptionsWidget::unregisterObservable(
                                             anatomist::Observable* observable )
{

  try
  {

    runCommand();
    this->Observer::unregisterObservable( observable );
    anatomist::AObject* 
      object = dynamic_cast< anatomist::AObject * >( observable );
    if ( !object )
    {

      return;

    }
    _objects.erase( object );
    _objectParamSelect->updateLabel( _objects );

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::updateObjects()
{

  try
  {


    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::AMedusaGeometryObject*
      aMedusaGeometryObject = dynamic_cast< gkg::AMedusaGeometryObject* >( *o );
      if ( aMedusaGeometryObject )
      {

        aMedusaGeometryObject->setVertexCountPerSphereAtom(
                                                    _vertexCountPerSphereAtom );
        aMedusaGeometryObject->setFiberPopulationVisibility(
                                                   _fiberPopulationVisibility );
        aMedusaGeometryObject->setSomaPopulationVisibility(
                                                    _somaPopulationVisibility );
        aMedusaGeometryObject->setNeuronPopulationVisibility(
                                                  _neuronPopulationVisibility );
        aMedusaGeometryObject->setDendritePopulationVisibility(
                                                _dendritePopulationVisibility );
        aMedusaGeometryObject->setAstrocytePopulationVisibility(
                                               _astrocytePopulationVisibility );
        aMedusaGeometryObject->setOligodendrocytePopulationVisibility(
                                         _oligodendrocytePopulationVisibility );

        aMedusaGeometryObject->notifyObservers( this );
        aMedusaGeometryObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "updateObjects()" );

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::drawContents()
{

  try
  {

    // main window and layout
    _mainWindow = new QWidget();
    QGridLayout* mainLayout = new QGridLayout( this );
    _mainWindow->setLayout( mainLayout );

    // selection object to modify
    _objectParamSelect = new ObjectParamSelect( _objects, this );


    // buidling rendering widget from designer .ui file
    std::string uiPath = gkg::ConfigurationInfo::getInstance().getUiPath(
                                          "anatomist-plugin",
                                          "MedusaGeometryRenderingOptions.ui" );

    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();
    
    // vertex count per sphere atom
    _vertexCountPerSphereAtomSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                           "spinBox_VertexCountPerSphereAtom" );
    _vertexCountPerSphereAtomSpinBox->setRange( 6, 4000 );
    _vertexCountPerSphereAtomSpinBox->setSingleStep( 1 );
    _vertexCountPerSphereAtomSpinBox->setValue( _vertexCountPerSphereAtom );


    // fiber population visibility
    _fiberPopulationVisibilityCheckBox = qRootWidget->findChild< QCheckBox* >( 
                                         "checkBox_FiberPopulationVisibility" );
    _fiberPopulationVisibilityCheckBox->setCheckState( 
                 ( _fiberPopulationVisibility ? Qt::Checked : Qt::Unchecked ) );

    // soma population visibility
    _somaPopulationVisibilityCheckBox = qRootWidget->findChild< QCheckBox* >( 
                                          "checkBox_SomaPopulationVisibility" );
    _somaPopulationVisibilityCheckBox->setCheckState( 
                  ( _somaPopulationVisibility ? Qt::Checked : Qt::Unchecked ) );

    // neuron population visibility
    _neuronPopulationVisibilityCheckBox = qRootWidget->findChild< QCheckBox* >( 
                                        "checkBox_NeuronPopulationVisibility" );
    _neuronPopulationVisibilityCheckBox->setCheckState( 
                ( _neuronPopulationVisibility ? Qt::Checked : Qt::Unchecked ) );

    // dendrite population visibility
    _dendritePopulationVisibilityCheckBox = 
                                           qRootWidget->findChild< QCheckBox* >(
                                      "checkBox_DendritePopulationVisibility" );
    _dendritePopulationVisibilityCheckBox->setCheckState( 
              ( _dendritePopulationVisibility ? Qt::Checked : Qt::Unchecked ) );

    // astrocyte population visibility
    _astrocytePopulationVisibilityCheckBox = 
                                           qRootWidget->findChild< QCheckBox* >(
                                     "checkBox_AstrocytePopulationVisibility" );
    _astrocytePopulationVisibilityCheckBox->setCheckState( 
             ( _astrocytePopulationVisibility ? Qt::Checked : Qt::Unchecked ) );

    // oligodendrocyte population visibility
    _oligodendrocytePopulationVisibilityCheckBox =
                               qRootWidget->findChild< QCheckBox* >( 
                               "checkBox_OligodendrocytePopulationVisibility" );
    _oligodendrocytePopulationVisibilityCheckBox->setCheckState( 
       ( _oligodendrocytePopulationVisibility ? Qt::Checked : Qt::Unchecked ) );

    // validation group box
    QGroupBox* validationGroupBox = new QGroupBox();
    validationGroupBox->setFlat( true );
    QGridLayout* validationLayout = new QGridLayout();
    validationGroupBox->setLayout( validationLayout );
 
    QPushButton* cancelPushButton = new QPushButton( "Cancel" );
    QPushButton* okPushButton = new QPushButton( "Ok" );
 
    validationLayout->addWidget( cancelPushButton, 0, 0, 1, 1 );
    validationLayout->addWidget( okPushButton, 0, 1, 1, 1 );
 
    // adding everything to the main layout
    mainLayout->addWidget( qRootWidget, 1, 0, 1, 3 );
    mainLayout->addWidget( validationGroupBox, 2, 0, 1, 3 );

    connect( _vertexCountPerSphereAtomSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( vertexCountPerSphereAtomChanged( int ) ) );

    connect( _fiberPopulationVisibilityCheckBox,
             SIGNAL( stateChanged( int ) ), 
             this,
             SLOT( fiberPopulationVisibilityChanged( int ) ) );

    connect( _somaPopulationVisibilityCheckBox,
             SIGNAL( stateChanged( int ) ), 
             this,
             SLOT( somaPopulationVisibilityChanged( int ) ) );

    connect( _neuronPopulationVisibilityCheckBox,
             SIGNAL( stateChanged( int ) ), 
             this,
             SLOT( neuronPopulationVisibilityChanged( int ) ) );

    connect( _dendritePopulationVisibilityCheckBox,
             SIGNAL( stateChanged( int ) ), 
             this,
             SLOT( dendritePopulationVisibilityChanged( int ) ) );

    connect( _astrocytePopulationVisibilityCheckBox,
             SIGNAL( stateChanged( int ) ), 
             this,
             SLOT( astrocytePopulationVisibilityChanged( int ) ) );

    connect( _oligodendrocytePopulationVisibilityCheckBox,
             SIGNAL( stateChanged( int ) ), 
             this,
             SLOT( oligodendrocytePopulationVisibilityChanged( int ) ) );
    
    connect( cancelPushButton,
             SIGNAL( clicked() ),
             this,
             SLOT( cancel() ) );

    connect( okPushButton,
             SIGNAL( clicked() ),
             this,
             SLOT( accept() ) );

    _mainWindow->show();
 
  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "drawContents()" );

}


void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                                    vertexCountPerSphereAtomChanged( int value )
{

  try
  {

    _vertexCountPerSphereAtom = value;

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "vertexCountPerSphereAtomChanged( int value )" );

}

void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                                  fiberPopulationVisibilityChanged( int value )
{

  try
  {

    _fiberPopulationVisibility = ( value ? true : false );

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "fiberPopulationVisibilityChanged( int value )" );

}

void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                                   somaPopulationVisibilityChanged( int value )
{

  try
  {

    _somaPopulationVisibility = ( value ? true : false );

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "somaPopulationVisibilityChanged( int value )" );

}

void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                                  neuronPopulationVisibilityChanged( int value )
{

  try
  {

    _neuronPopulationVisibility = ( value ? true : false );

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "neuronPopulationVisibilityChanged( int value )" );

}

void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                               dendritePopulationVisibilityChanged( int value )
{

  try
  {

    _dendritePopulationVisibility = ( value ? true : false );

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "dendritePopulationVisibilitychanged( int value )" );

}

void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                              astrocytePopulationVisibilityChanged( int value )
{

  try
  {

    _astrocytePopulationVisibility = ( value ? true : false );

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "astrocytePopulationVisibilitychanged( int value )" );

}

void 
gkg::AMedusaGeometryObjectRenderingOptionsWidget::
                        oligodendrocytePopulationVisibilityChanged( int value )
{

  try
  {

    _oligodendrocytePopulationVisibility = ( value ? true : false );

  }
  GKG_CATCH( "void "
             "gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "oligodendrocytePopulationVisibilityChanged( int value )" );

}



void gkg::AMedusaGeometryObjectRenderingOptionsWidget::cancel()
{

  try
  {


    _vertexCountPerSphereAtom = _originalVertexCountPerSphereAtom;
    _fiberPopulationVisibility = _originalFiberPopulationVisibility;
    _somaPopulationVisibility = _originalSomaPopulationVisibility;
    _neuronPopulationVisibility = _originalNeuronPopulationVisibility;
    _dendritePopulationVisibility = _originalDendritePopulationVisibility;
    _astrocytePopulationVisibility = _originalAstrocytePopulationVisibility;
    _oligodendrocytePopulationVisibility = 
                                   _originalOligodendrocytePopulationVisibility;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "cancel()" );

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    updateObjects();

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "accept()" );

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::chooseObject()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator 
      o = _initialObjects.begin(),
      oe = _initialObjects.end(),
      oprime;

    while ( o != oe )
    {

      if ( theAnatomist->hasObject( *o ) )
      {

        ++ o;

      }
      else
      {

        oprime = o;
        ++ o;
        _initialObjects.erase( oprime );

      }

    }

    _objectParamSelect->selectObjects( _initialObjects, _objects );

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "chooseObject()" );

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::objectsChosen(
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    _operating = true;
    runCommand();

    while( !_objects.empty() )
    {

      ( *_objects.begin() )->deleteObserver( this );

    }

    _objects = objects;

    std::set< anatomist::AObject * >::const_iterator 
      o = objects.begin(),
      oe = objects.end();
    while ( o != oe )
    {

      ( *o )->addObserver( this );

    }

    _objectParamSelect->updateLabel( objects );
    _operating = false;

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::"
             "objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::runCommand()
{
}


void gkg::AMedusaGeometryObjectRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::AMedusaGeometryObjectRenderingOptionsWidget*
    //  aOdfFieldRenderingOptionsWidget =
    new gkg::AMedusaGeometryObjectRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::AMedusaGeometryObjectRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
