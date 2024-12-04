#include <gkg-anatomist-plugin-rendering-options/ATwoPoolModelFieldRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/ATwoPoolModelFieldObject.h>
#include <gkg-core-exception/Exception.h>
#include <anatomist/application/settings.h>
#include <anatomist/application/Anatomist.h>
#include <QPixmap>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDoubleSpinBox>

gkg::ATwoPoolModelFieldRenderingOptionsWidget::
                                       ATwoPoolModelFieldRenderingOptionsWidget(
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

    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::ATwoPoolModelFieldObject* 
        aOdfObject = dynamic_cast< gkg::ATwoPoolModelFieldObject* >( *o );
      if ( aOdfObject )
      {

        this->_outputOrientationCount = aOdfObject->getOutputOrientationCount();
        this->_meshScale = aOdfObject->getMeshScale();
        this->_hasApproximatedNormals = aOdfObject->hasApproximatedNormals();
        this->_hasHiddenShadows = aOdfObject->hasHiddenShadows();
        this->_realNormalsAlreadyComputed = 
                                    aOdfObject->areRealNormalsAlreadyComputed();

        break;

      }
      aOdfObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    this->_originalOutputOrientationCount = this->_outputOrientationCount;
    this->_originalHasApproximatedNormals = this->_hasApproximatedNormals;
    this->_originalHasHiddenShadows = this->_hasHiddenShadows;

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
  GKG_CATCH( "gkg::ATwoPoolModelFieldRenderingOptionsWidget::"
             "ATwoPoolModelFieldRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::ATwoPoolModelFieldRenderingOptionsWidget::
                                     ~ATwoPoolModelFieldRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::update(
                                                   const anatomist::Observable*,
                                                   void* /* arg */ )
{
}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::updateObjects()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::ATwoPoolModelFieldObject*
        aOdfObject = dynamic_cast< gkg::ATwoPoolModelFieldObject * >( *o );
      if ( aOdfObject )
      {

        aOdfObject->setShadowOptions( _hasHiddenShadows,
                                      _hasApproximatedNormals );
        aOdfObject->setMeshScale( _meshScale );
        aOdfObject->notifyObservers( this );
        aOdfObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH(
        "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::updateObjects()" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::drawContents()
{

  try
  {

    // main window and layout
    _mainWindow = new QWidget();
    QGridLayout* mainLayout = new QGridLayout( this );
    _mainWindow->setLayout( mainLayout );

    // selection object to modify
    _objectParamSelect = new ObjectParamSelect( _objects, this );

    // orientation count group box
    QGroupBox* 
      outputOrientationCountGroupBox = new QGroupBox( "Sphere appearance " );
    QGridLayout*
      outputOrientationCountLayout = new QGridLayout();
    outputOrientationCountGroupBox->setLayout( outputOrientationCountLayout );

    QLabel* meshScaleLabel = new QLabel( "Mesh scale" );
    _meshScaleDoubleSpinBox = new QDoubleSpinBox();
    _meshScaleDoubleSpinBox->setRange( 0.05, 100 );
    _meshScaleDoubleSpinBox->setValue( _meshScale );
    _meshScaleDoubleSpinBox->setSingleStep( 0.05 );
    outputOrientationCountLayout->addWidget( meshScaleLabel,
                                             0, 0, 1, 2 );
    outputOrientationCountLayout->addWidget( _meshScaleDoubleSpinBox,
                                             0, 2, 1, 1 );

    QLabel* orientationCountLabel = new QLabel( "Orientation count" );
    QLabel *orientationCountWarningLabel = new QLabel( "" );

    _outputOrientationCountSpinBox = new gkg::OrientationCountSpinBox();
    _outputOrientationCountSpinBox->setRange( 6, 4000 );
    _outputOrientationCountSpinBox->setNewValue( _outputOrientationCount );
    outputOrientationCountLayout->addWidget( orientationCountLabel,
                                             1, 0, 1, 2 );
    outputOrientationCountLayout->addWidget( orientationCountWarningLabel,
                                             2, 0, 1, 2 );
    outputOrientationCountLayout->addWidget( _outputOrientationCountSpinBox,
                                             1, 2, 1, 1 );

    // normal display group box
    QGroupBox* normalDisplayGroupBox = new QGroupBox( "Shadow display" );
    QGridLayout* normalDisplayLayout = new QGridLayout();
    normalDisplayGroupBox->setLayout( normalDisplayLayout );

    QCheckBox* hideShadowsCheckBox = new QCheckBox( "Hide shadows" );
    hideShadowsCheckBox->setChecked( _hasHiddenShadows );
    _hasApproximatedNormalsRadioButton = new QRadioButton( 
                                                       "Approximated normals" );
    _hasApproximatedNormalsRadioButton->setChecked( _hasApproximatedNormals );
    _hasRealNormalsRadioButton = new QRadioButton( "Real normals" );
    _hasRealNormalsLabel = new QLabel( "" );
    if ( !_hasApproximatedNormals )
    {

      _hasRealNormalsRadioButton->setChecked( true );

    }
    if ( _realNormalsAlreadyComputed )
    {

       _hasRealNormalsLabel->setText( "" );

    }
    else
    {

      _hasRealNormalsLabel->setText( 
                                   "<i>( Need reloading to be applied )</i>" );

    }
    if ( _hasHiddenShadows )
    {

      _hasApproximatedNormalsRadioButton->setEnabled( false );
      _hasRealNormalsRadioButton->setEnabled( false );
      _hasRealNormalsLabel->setEnabled( false );

    }
    normalDisplayLayout->addWidget( _hasApproximatedNormalsRadioButton,
                                    0, 0, 1, 1 );
    normalDisplayLayout->addWidget( _hasRealNormalsRadioButton,
                                    1, 0, 1, 1 );
    normalDisplayLayout->addWidget( hideShadowsCheckBox, 2, 0, 1, 1 );
    normalDisplayLayout->addWidget( _hasRealNormalsLabel, 1, 1, 1, 1 );

    // validation group box
    QGroupBox* validationGroupBox = new QGroupBox();
    validationGroupBox->setFlat( true );
    QGridLayout* validationLayout = new QGridLayout();
    validationGroupBox->setLayout( validationLayout );
 
    QPushButton* cancelPushButton = new QPushButton( "Cancel" );
    QPushButton* okPushButton = new QPushButton( "Ok" );
 
    validationLayout->addWidget( cancelPushButton, 0, 0, 1, 1 );
    validationLayout->addWidget( okPushButton, 0, 1, 1, 1 );
 
    mainLayout->addWidget( outputOrientationCountGroupBox, 1, 0, 1, 3 );
    mainLayout->addWidget( normalDisplayGroupBox, 2, 0, 1, 3 );
    mainLayout->addWidget( validationGroupBox, 3, 0, 1, 3 );

    connect( _meshScaleDoubleSpinBox, SIGNAL( valueChanged( double ) ),
             this, SLOT( meshScaleChanged( double ) ) );
    connect( _outputOrientationCountSpinBox, SIGNAL( valueChanged( int ) ),
             this, SLOT( outputOrientationCountChanged( int ) ) );
    connect( hideShadowsCheckBox, SIGNAL( stateChanged ( int ) ),
             this, SLOT( hideShadowsChanged( int ) ) );
    connect( _hasApproximatedNormalsRadioButton, SIGNAL( toggled( bool ) ),
             this, SLOT( normalDisplayChanged( bool ) ) );
    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( cancel() ) );
    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    _mainWindow->show();
 
  }
  GKG_CATCH( "void "
             "gkg::ATwoPoolModelFieldRenderingOptionsWidget::drawContents()" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::meshScaleChanged(
                                                                  double value )
{

  try
  {

    _meshScale = ( float )value;
    updateObjects();

  }
  GKG_CATCH( "void "
             "gkg::ATwoPoolModelFieldRenderingOptionsWidget::meshScaleChanged( "
             "float value )" );

}


void 
gkg::ATwoPoolModelFieldRenderingOptionsWidget::outputOrientationCountChanged(
                                                                     int value )
{

  _outputOrientationCount = value;

}


void 
gkg::ATwoPoolModelFieldRenderingOptionsWidget::hideShadowsChanged( int checked )
{
  try
  {

    _hasHiddenShadows = checked;
    if ( checked )
    {

      _hasApproximatedNormalsRadioButton->setEnabled( false );
      _hasRealNormalsRadioButton->setEnabled( false );
      _hasRealNormalsLabel->setEnabled( false );

    }
    else
    {

      _hasApproximatedNormalsRadioButton->setEnabled( true );
      _hasRealNormalsRadioButton->setEnabled( true );
      _hasRealNormalsLabel->setEnabled( true );

    }
    updateObjects();

  }
  GKG_CATCH( "void "
             "gkg::ATwoPoolModelFieldRenderingOptionsWidget::"
             "hideShadowsChanged( "
             "int32_t checked )" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::normalDisplayChanged(
                                                                  bool checked )
{

  try
  {

    _hasApproximatedNormals = checked;
    updateObjects();

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::"
             "normalDisplayChanged( "
             "bool checked )" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::cancel()
{

  try
  {

    _hasHiddenShadows = _originalHasHiddenShadows;
    _hasApproximatedNormals = _originalHasApproximatedNormals;
    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::cancel()" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();

    if ( ( _outputOrientationCountSpinBox->value() != 
           _originalOutputOrientationCount ) ||
         ( !_hasHiddenShadows && ( _hasApproximatedNormals == false ) && 
           ( _realNormalsAlreadyComputed == false ) ) )
    {

      int32_t result = QMessageBox::question(
                           this,
                           "Object reload",
                            QString( "You need to reload objects to take " ) +
                            QString( "your new display options into account." ),
                            QMessageBox::Ok | QMessageBox::Cancel );

      if ( result == QMessageBox::Ok )
      {

        std::set< anatomist::AObject * >::const_iterator 
          o = _objects.begin(),
          oe = _objects.end();

        while ( o != oe )
        {

          gkg::ATwoPoolModelFieldObject*
            aTwoPoolModelFieldObject =
                           dynamic_cast< gkg::ATwoPoolModelFieldObject* >( *o );
          if ( aTwoPoolModelFieldObject )
          {

            aTwoPoolModelFieldObject->setRenderingOptions(
                                        _hasHiddenShadows,
                                        _outputOrientationCountSpinBox->value(),
                                        _hasApproximatedNormals );

          }

          ++ o;

        }

      }
      else
      {

        cancel();

      }

    }
    updateObjects();

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::accept()" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::"
             "chooseObject()" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::"
             "objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::runCommand()
{
}


void gkg::ATwoPoolModelFieldRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::ATwoPoolModelFieldRenderingOptionsWidget* 
    //  aTwoPoolModelFieldRenderingOptionsWidget =
    new gkg::ATwoPoolModelFieldRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::ATwoPoolModelFieldRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
