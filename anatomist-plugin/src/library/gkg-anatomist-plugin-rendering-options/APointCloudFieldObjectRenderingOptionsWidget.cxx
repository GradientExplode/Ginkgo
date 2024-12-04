#include <gkg-anatomist-plugin-rendering-options/APointCloudFieldObjectRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/APointCloudFieldObject.h>
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



gkg::APointCloudFieldObjectRenderingOptionsWidget::
                                   APointCloudFieldObjectRenderingOptionsWidget(
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
    
    _vertexCountPerSphere = 60;
    _pointRadius = 0.05;
    _meshScale = 1.0;

    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::APointCloudFieldObject* 
        aPointCloudFieldObject = dynamic_cast< gkg::APointCloudFieldObject* >(
                                                                           *o );
      if ( aPointCloudFieldObject )
      {

        _vertexCountPerSphere =
                aPointCloudFieldObject->getVertexCountPerSphere();
        _pointRadius =
                aPointCloudFieldObject->getPointRadius();
        _meshScale =
                aPointCloudFieldObject->getMeshScale();
        
        break;

      }
      aPointCloudFieldObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    _originalVertexCountPerSphere = _vertexCountPerSphere;
    _originalPointRadius = _pointRadius;
    _originalMeshScale = _meshScale;

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
  GKG_CATCH( "gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "APointCloudFieldObjectRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::APointCloudFieldObjectRenderingOptionsWidget::
                                 ~APointCloudFieldObjectRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::update(
                                                   const anatomist::Observable*,
                                                   void* /* arg */ )
{
}

void gkg::APointCloudFieldObjectRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::updateObjects()
{

  try
  {


    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::APointCloudFieldObject*
        aPointCloudFieldObject = dynamic_cast< gkg::APointCloudFieldObject* >(
                                                                           *o );
      if ( aPointCloudFieldObject )
      {

        aPointCloudFieldObject->setVertexCountPerSphere(
                                                        _vertexCountPerSphere );
        aPointCloudFieldObject->setPointRadius( _pointRadius );
        aPointCloudFieldObject->setMeshScale( _meshScale );

        aPointCloudFieldObject->notifyObservers( this );
        aPointCloudFieldObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "updateObjects()" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::drawContents()
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
                                         "PointCloudFieldRenderingOptions.ui" );

    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();
    
    // vertex count per ODF
    _vertexCountPerSphereSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                               "spinBox_VertexCountPerSphere" );
    _vertexCountPerSphereSpinBox->setRange( 12, 4000 );
    _vertexCountPerSphereSpinBox->setSingleStep( 2 );
    _vertexCountPerSphereSpinBox->setValue( _vertexCountPerSphere );

    // point radius
    _pointRadiusDoubleSpinBox = qRootWidget->findChild< QDoubleSpinBox* >( 
                                                  "doubleSpinBox_PointRadius" );
    _pointRadiusDoubleSpinBox->setRange( 0.01, 10.0 );
    _pointRadiusDoubleSpinBox->setSingleStep( 0.01 );
    _pointRadiusDoubleSpinBox->setValue( _pointRadius );

    // mesh scale of sphere
    _meshScaleDoubleSpinBox = qRootWidget->findChild< QDoubleSpinBox* >( 
                                                  "doubleSpinBox_MeshScale" );
    _meshScaleDoubleSpinBox->setRange( 0.01, 100.0 );
    _meshScaleDoubleSpinBox->setSingleStep( 0.01 );
    _meshScaleDoubleSpinBox->setValue( _meshScale );


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

    connect( _vertexCountPerSphereSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( vertexCountPerSphereChanged( int ) ) );
    connect( _pointRadiusDoubleSpinBox,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( pointRadiusChanged( double ) ) );
    connect( _meshScaleDoubleSpinBox,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( meshScaleChanged( double ) ) );

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
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "drawContents()" );

}


void 
gkg::APointCloudFieldObjectRenderingOptionsWidget::vertexCountPerSphereChanged(
                                                                     int value )
{

  try
  {

    _vertexCountPerSphere = value;

  }
  GKG_CATCH( "void "
             "gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "vertexCountPerSphereChanged( int value )" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::pointRadiusChanged(
                                                                  double value )
{

  try
  {

    _pointRadius = ( float )value;

  }
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "pointRadiusChanged( "
             "float value )" );

}



void gkg::APointCloudFieldObjectRenderingOptionsWidget::meshScaleChanged(
                                                                  double value )
{

  try
  {

    _meshScale = ( float )value;

  }
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "meshScaleChanged( "
             "float value )" );

}



void gkg::APointCloudFieldObjectRenderingOptionsWidget::cancel()
{

  try
  {


    _vertexCountPerSphere = _originalVertexCountPerSphere;
    _pointRadius = _originalPointRadius;
    _meshScale = _originalMeshScale;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "cancel()" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    updateObjects();

  }
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "accept()" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "chooseObject()" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::"
             "objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::runCommand()
{
}


void gkg::APointCloudFieldObjectRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::APointCloudFieldObjectRenderingOptionsWidget*
    //  aPointCloudFieldRenderingOptionsWidget =
    new gkg::APointCloudFieldObjectRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::APointCloudFieldObjectRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
