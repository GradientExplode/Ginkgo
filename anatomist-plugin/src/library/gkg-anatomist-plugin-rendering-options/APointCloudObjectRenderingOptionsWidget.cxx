#include <gkg-anatomist-plugin-rendering-options/APointCloudObjectRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/APointCloudObject.h>
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



gkg::APointCloudObjectRenderingOptionsWidget::
                                        APointCloudObjectRenderingOptionsWidget(
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
    
    _sphereVertexCount = 60; 
    _sphereRadius = 1.0; 
    
    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::APointCloudObject* 
      aPointCloudObject = dynamic_cast< gkg::APointCloudObject* >( *o );
      if ( aPointCloudObject )
      {

        _sphereVertexCount =
                aPointCloudObject->getSphereVertexCount();
        _sphereRadius =
                aPointCloudObject->getSphereRadius();
        
        break;

      }
      aPointCloudObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    _originalSphereVertexCount = _sphereVertexCount;
    _originalSphereRadius = _sphereRadius;

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
  GKG_CATCH( "gkg::APointCloudObjectRenderingOptionsWidget::"
             "APointCloudObjectRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::APointCloudObjectRenderingOptionsWidget::
                                  ~APointCloudObjectRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::APointCloudObjectRenderingOptionsWidget::update(
                                                 const anatomist::Observable*,
                                                 void* /* arg */ )
{
}

void gkg::APointCloudObjectRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::APointCloudObjectRenderingOptionsWidget::updateObjects()
{

  try
  {


    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::APointCloudObject*
      aPointCloudObject = dynamic_cast< gkg::APointCloudObject* >( *o );
      if ( aPointCloudObject )
      {

        aPointCloudObject->setSphereVertexCount( _sphereVertexCount );
        aPointCloudObject->setSphereRadius( _sphereRadius );

        aPointCloudObject->notifyObservers( this );
        aPointCloudObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "updateObjects()" );

}


void gkg::APointCloudObjectRenderingOptionsWidget::drawContents()
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
                                          "PointCloudRenderingOptions.ui" );

    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();
    
    // sphere vertex count
    _sphereVertexCountSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                                 "spinBox_SphereVertexCount" );
    _sphereVertexCountSpinBox->setRange( 6, 4000 );
    _sphereVertexCountSpinBox->setSingleStep( 1 );
    _sphereVertexCountSpinBox->setValue( _sphereVertexCount );

    // sphere radius
    _sphereRadiusDoubleSpinBox = qRootWidget->findChild< QDoubleSpinBox* >( 
                                                 "doubleSpinBox_SphereRadius" );
    _sphereRadiusDoubleSpinBox->setRange( 0.01, 100.0 );
    _sphereRadiusDoubleSpinBox->setSingleStep( 0.01 );
    _sphereRadiusDoubleSpinBox->setValue( ( double )_sphereRadius );

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

    connect( _sphereVertexCountSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( sphereVertexCountChanged( int ) ) );

    connect( _sphereRadiusDoubleSpinBox,
             SIGNAL( valueChanged( double ) ), 
             this,
             SLOT( sphereRadiusChanged( double ) ) );
    
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
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "drawContents()" );

}


void 
gkg::APointCloudObjectRenderingOptionsWidget::
                                          sphereVertexCountChanged( int value )
{

  try
  {

    _sphereVertexCount = value;

  }
  GKG_CATCH( "void "
             "gkg::APointCloudObjectRenderingOptionsWidget::"
             "sphereVertexCountChanged( int value )" );

}

void 
gkg::APointCloudObjectRenderingOptionsWidget::sphereRadiusChanged(
                                                                double value )
{

  try
  {

    _sphereRadius = ( float )value;

  }
  GKG_CATCH( "void "
             "gkg::APointCloudObjectRenderingOptionsWidget::"
             "sphereRadiusChanged( double value )" );

}



void gkg::APointCloudObjectRenderingOptionsWidget::cancel()
{

  try
  {


    _sphereVertexCount = _originalSphereVertexCount;
    _sphereRadius = _originalSphereRadius;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "cancel()" );

}


void gkg::APointCloudObjectRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    updateObjects();

  }
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "accept()" );

}


void gkg::APointCloudObjectRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "chooseObject()" );

}


void gkg::APointCloudObjectRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::"
             "objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::APointCloudObjectRenderingOptionsWidget::runCommand()
{
}


void gkg::APointCloudObjectRenderingOptionsWidget::create( 
                              const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::APointCloudObjectRenderingOptionsWidget*
    //  aOdfFieldRenderingOptionsWidget =
    new gkg::APointCloudObjectRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::APointCloudObjectRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
