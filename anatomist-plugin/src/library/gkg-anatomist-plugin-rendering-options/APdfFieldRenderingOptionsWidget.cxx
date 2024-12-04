#include <gkg-anatomist-plugin-rendering-options/APdfFieldRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/APdfFieldObject.h>
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

gkg::APdfFieldRenderingOptionsWidget::APdfFieldRenderingOptionsWidget(
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

      gkg::APdfFieldObject* 
        aPdfObject = dynamic_cast< gkg::APdfFieldObject* >( *o );
      if ( aPdfObject )
      {

        this->_outputOrientationCount = aPdfObject->getOutputOrientationCount();
        this->_resamplingNeighborCount =
                                       aPdfObject->getResamplingNeighborCount();
        this->_resamplingGaussianSigma =
                                       aPdfObject->getResamplingGaussianSigma();
        this->_meshScale = aPdfObject->getMeshScale();
        this->_displacementMagnitude = aPdfObject->getDisplacementMagnitude();
        this->_hasApproximatedNormals = aPdfObject->hasApproximatedNormals();
        this->_hasHiddenShadows = aPdfObject->hasHiddenShadows();
        this->_isPolarHarmonics = aPdfObject->isPolarHarmonics();
        this->_realNormalsAlreadyComputed = 
                                    aPdfObject->areRealNormalsAlreadyComputed();

        break;

      }
      aPdfObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    this->_originalDisplacementMagnitude = this->_displacementMagnitude;
    this->_originalOutputOrientationCount = this->_outputOrientationCount;
    this->_originalResamplingNeighborCount = this->_resamplingNeighborCount;
    this->_originalResamplingGaussianSigma = this->_resamplingGaussianSigma;
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
  GKG_CATCH( "gkg::APdfFieldRenderingOptionsWidget::"
             "APdfFieldRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::APdfFieldRenderingOptionsWidget::~APdfFieldRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::APdfFieldRenderingOptionsWidget::update( const anatomist::Observable*,
                                                   void* /* arg */ )
{
}


void gkg::APdfFieldRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::APdfFieldRenderingOptionsWidget::updateObjects()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::APdfFieldObject*
        aPdfObject = dynamic_cast< gkg::APdfFieldObject * >( *o );
      if ( aPdfObject )
      {

        aPdfObject->setShadowOptions( _hasHiddenShadows,
                                      _hasApproximatedNormals );
        aPdfObject->setMeshScale( _meshScale );
        aPdfObject->notifyObservers( this );
        aPdfObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::updateObjects()" );

}


void gkg::APdfFieldRenderingOptionsWidget::drawContents()
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
      sphereAppearanceBox = new QGroupBox( "Sphere appearance " );
    QGridLayout*
      sphereAppearanceLayout = new QGridLayout();
    sphereAppearanceBox->setLayout( sphereAppearanceLayout );

    QLabel* meshScaleLabel = new QLabel( "Mesh scale" );
    _meshScaleDoubleSpinBox = new QDoubleSpinBox();
    _meshScaleDoubleSpinBox->setRange( 0.05, 10000 );
    _meshScaleDoubleSpinBox->setValue( _meshScale );
    _meshScaleDoubleSpinBox->setSingleStep( 0.05 );
    sphereAppearanceLayout->addWidget( meshScaleLabel,
                                             0, 0, 1, 2 );
    sphereAppearanceLayout->addWidget( _meshScaleDoubleSpinBox,
                                             0, 2, 1, 1 );

    QLabel* displacementMagnitudeLabel = new QLabel( "Displacement (in m)" );
    _displacementMagnitudeDoubleSpinBox = new QDoubleSpinBox();
    _displacementMagnitudeDoubleSpinBox->setDecimals( 10 );
    _displacementMagnitudeDoubleSpinBox->setRange( 0.0, 1000e-6 );
    _displacementMagnitudeDoubleSpinBox->setValue( _displacementMagnitude );
    _displacementMagnitudeDoubleSpinBox->setSingleStep( 1e-7 );
    sphereAppearanceLayout->addWidget( displacementMagnitudeLabel,
                                             1, 0, 1, 2 );
    sphereAppearanceLayout->addWidget( _displacementMagnitudeDoubleSpinBox,
                                             1, 2, 1, 1 );


    QLabel* orientationCountLabel = new QLabel( "Orientation count" );
    QLabel *orientationCountWarningLabel = new QLabel( "" );

    _outputOrientationCountSpinBox = new gkg::OrientationCountSpinBox();
    _outputOrientationCountSpinBox->setRange( 6, 4000 );
    _outputOrientationCountSpinBox->setNewValue( _outputOrientationCount );
    sphereAppearanceLayout->addWidget( orientationCountLabel,
                                             2, 0, 1, 2 );
    sphereAppearanceLayout->addWidget( _outputOrientationCountSpinBox,
                                             2, 2, 1, 1 );
    sphereAppearanceLayout->addWidget( orientationCountWarningLabel,
                                             3, 0, 1, 2 );

    orientationCountWarningLabel->setText(
                                       "<i>Need reloading to be applied</i>" );

    QLabel* resamplingNeighborCountLabel =
                                      new QLabel( "Resampling neighbor count" );
    _resamplingNeighborCountSpinBox = new QSpinBox();
    _resamplingNeighborCountSpinBox->setRange( 3, 100 );
    _resamplingNeighborCountSpinBox->setValue( _resamplingNeighborCount );
    _resamplingNeighborCountSpinBox->setSingleStep( 1 );
    sphereAppearanceLayout->addWidget( resamplingNeighborCountLabel,
                                             4, 0, 1, 2 );
    sphereAppearanceLayout->addWidget( _resamplingNeighborCountSpinBox,
                                             4, 2, 1, 1 );

    QLabel* resamplingGaussianSigmaLabel =
                                      new QLabel( "Resampling Gaussian sigma" );
    _resamplingGaussianSigmaDoubleSpinBox = new QDoubleSpinBox();
    _resamplingGaussianSigmaDoubleSpinBox->setRange( 1.0, 100.0 );
    _resamplingGaussianSigmaDoubleSpinBox->setValue( _resamplingGaussianSigma );
    _resamplingNeighborCountSpinBox->setSingleStep( 0.1 );
    sphereAppearanceLayout->addWidget( resamplingGaussianSigmaLabel,
                                             5, 0, 1, 2 );
    sphereAppearanceLayout->addWidget( _resamplingGaussianSigmaDoubleSpinBox,
                                             5, 2, 1, 1 );


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
 
    mainLayout->addWidget( sphereAppearanceBox, 1, 0, 1, 3 );
    mainLayout->addWidget( normalDisplayGroupBox, 2, 0, 1, 3 );
    mainLayout->addWidget( validationGroupBox, 3, 0, 1, 3 );

    connect( _meshScaleDoubleSpinBox, SIGNAL( valueChanged( double ) ),
             this, SLOT( meshScaleChanged( double ) ) );
    connect( _displacementMagnitudeDoubleSpinBox, SIGNAL( valueChanged( double ) ),
             this, SLOT( displacementMagnitudeChanged( double ) ) );
    connect( _outputOrientationCountSpinBox, SIGNAL( valueChanged( int ) ),
             this, SLOT( outputOrientationCountChanged( int ) ) );
    connect( _resamplingNeighborCountSpinBox, SIGNAL( valueChanged( int ) ),
             this, SLOT( resamplingNeighborCountChanged( int ) ) );
    connect( _resamplingGaussianSigmaDoubleSpinBox,
             SIGNAL( valueChanged( double ) ),
             this, SLOT( resamplingGaussianSigmaChanged( double ) ) );
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
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::drawContents()" );

}


void gkg::APdfFieldRenderingOptionsWidget::meshScaleChanged( double value )
{

  try
  {

    _meshScale = ( float )value;
    updateObjects();

  }
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::meshScaleChanged( "
             "float value )" );

}


void gkg::APdfFieldRenderingOptionsWidget::displacementMagnitudeChanged(
                                                                  double value )
{

  try
  {

    _displacementMagnitude = ( float )value;
    updateObjects();

  }
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::"
             "displacementMagnitudeChanged( "
             "float value )" );

}


void 
gkg::APdfFieldRenderingOptionsWidget::outputOrientationCountChanged( int value )
{

  _outputOrientationCount = value;
  updateObjects();

}


void 
gkg::APdfFieldRenderingOptionsWidget::resamplingNeighborCountChanged(
                                                                     int value )
{

  _resamplingNeighborCount = value;
  updateObjects();

}


void 
gkg::APdfFieldRenderingOptionsWidget::resamplingGaussianSigmaChanged(
                                                                  double value )
{

  _resamplingGaussianSigma = ( float )value;
  updateObjects();

}


void 
gkg::APdfFieldRenderingOptionsWidget::hideShadowsChanged( int checked )
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
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::hideShadowsChanged( "
             "int32_t checked )" );

}


void gkg::APdfFieldRenderingOptionsWidget::normalDisplayChanged( bool checked )
{

  try
  {

    _hasApproximatedNormals = checked;
    updateObjects();

  }
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::normalDisplayChanged( "
             "bool checked )" );

}


void gkg::APdfFieldRenderingOptionsWidget::cancel()
{

  try
  {

    _hasHiddenShadows = _originalHasHiddenShadows;
    _hasApproximatedNormals = _originalHasApproximatedNormals;
    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::cancel()" );

}


void gkg::APdfFieldRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();

    if ( ( _outputOrientationCountSpinBox->value() != 
           _originalOutputOrientationCount ) ||
         ( _resamplingNeighborCountSpinBox->value() != 
           _originalResamplingNeighborCount ) ||
         ( ( float )_resamplingGaussianSigmaDoubleSpinBox->value() != 
           _originalResamplingGaussianSigma ) ||
         ( _displacementMagnitudeDoubleSpinBox->value() != 
           _originalDisplacementMagnitude ) ||
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

          gkg::APdfFieldObject*
            aPdfObject = dynamic_cast< gkg::APdfFieldObject* >( *o );
          if ( aPdfObject )
          {

            aPdfObject->setRenderingOptions(
                        _hasHiddenShadows,
                        _displacementMagnitudeDoubleSpinBox->value(),
                        _outputOrientationCountSpinBox->value(),
                        _resamplingNeighborCountSpinBox->value(),
                        ( float )_resamplingGaussianSigmaDoubleSpinBox->value(),
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
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::accept()" );

}


void gkg::APdfFieldRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::chooseObject()" );

}


void gkg::APdfFieldRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::APdfFieldRenderingOptionsWidget::runCommand()
{
}


void gkg::APdfFieldRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::APdfFieldRenderingOptionsWidget* aPdfFieldRenderingOptionsWidget =
    new gkg::APdfFieldRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::APdfFieldRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
