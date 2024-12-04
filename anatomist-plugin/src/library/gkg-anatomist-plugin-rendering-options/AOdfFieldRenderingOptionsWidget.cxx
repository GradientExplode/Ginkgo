#include <gkg-anatomist-plugin-rendering-options/AOdfFieldRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/AOdfFieldObject.h>
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



gkg::AOdfFieldRenderingOptionsWidget::AOdfFieldRenderingOptionsWidget(
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
    
    _vertexCountPerOdf = 60;
    _meshScale = 1.0;
    
    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::AOdfFieldObject* 
        aOdfFieldObject = dynamic_cast< gkg::AOdfFieldObject* >( *o );
      if ( aOdfFieldObject )
      {

        _vertexCountPerOdf =
                aOdfFieldObject->getVertexCountPerOdf();
        _meshScale =
                aOdfFieldObject->getMeshScale();
        
        break;

      }
      aOdfFieldObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    _originalVertexCountPerOdf = _vertexCountPerOdf;
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
  GKG_CATCH( "gkg::AOdfFieldRenderingOptionsWidget::"
             "AOdfFieldRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::AOdfFieldRenderingOptionsWidget::~AOdfFieldRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::AOdfFieldRenderingOptionsWidget::update( const anatomist::Observable*,
                                                   void* /* arg */ )
{
}

void gkg::AOdfFieldRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::"
             "unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::AOdfFieldRenderingOptionsWidget::updateObjects()
{

  try
  {


    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::AOdfFieldObject*
        aOdfFieldObject = dynamic_cast< gkg::AOdfFieldObject* >( *o );
      if ( aOdfFieldObject )
      {

        aOdfFieldObject->setVertexCountPerOdf( _vertexCountPerOdf );
        aOdfFieldObject->setMeshScale( _meshScale );

        aOdfFieldObject->notifyObservers( this );
        aOdfFieldObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::AOdfFieldObjectRenderingOptionsWidget::"
             "updateObjects()" );

}


void gkg::AOdfFieldRenderingOptionsWidget::drawContents()
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
                                                "OdfFieldRenderingOptions.ui" );

    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();
    
    // vertex count per ODF
    _vertexCountPerOdfSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                                  "spinBox_VertexCountPerOdf" );
    _vertexCountPerOdfSpinBox->setRange( 12, 4000 );
    _vertexCountPerOdfSpinBox->setSingleStep( 2 );
    _vertexCountPerOdfSpinBox->setValue( _vertexCountPerOdf );

    // mesh scale of ODF
    _meshScaleDoubleSpinBox = qRootWidget->findChild< QDoubleSpinBox* >( 
                                                  "doubleSpinBox_MeshScale" );
    _meshScaleDoubleSpinBox->setRange( 0.05, 100.0 );
    _meshScaleDoubleSpinBox->setSingleStep( 0.05 );
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

    connect( _vertexCountPerOdfSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( vertexCountPerOdfChanged( int ) ) );
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
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::"
             "drawContents()" );

}


void 
gkg::AOdfFieldRenderingOptionsWidget::vertexCountPerOdfChanged( int value )
{

  try
  {

    _vertexCountPerOdf = value;

  }
  GKG_CATCH( "void "
             "gkg::AOdfFieldRenderingOptionsWidget::"
             "vertexCountPerOdfChanged( int value )" );

}


void gkg::AOdfFieldRenderingOptionsWidget::meshScaleChanged( double value )
{

  try
  {

    _meshScale = ( float )value;

  }
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::meshScaleChanged( "
             "float value )" );

}



void gkg::AOdfFieldRenderingOptionsWidget::cancel()
{

  try
  {


    _vertexCountPerOdf = _originalVertexCountPerOdf;
    _meshScale = _originalMeshScale;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::"
             "cancel()" );

}


void gkg::AOdfFieldRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    updateObjects();

  }
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::"
             "accept()" );

}


void gkg::AOdfFieldRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::"
             "chooseObject()" );

}


void gkg::AOdfFieldRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::"
             "objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::AOdfFieldRenderingOptionsWidget::runCommand()
{
}


void gkg::AOdfFieldRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::AOdfFieldObjectRenderingOptionsWidget*
    //  aOdfFieldRenderingOptionsWidget =
    new gkg::AOdfFieldRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::AOdfFieldRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
