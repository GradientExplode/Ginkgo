#include <gkg-anatomist-plugin-rendering-options/AInteractiveBundleMapRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/AInteractiveBundleMapObject.h>
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


gkg::AInteractiveBundleMapRenderingOptionsWidget::
                                    AInteractiveBundleMapRenderingOptionsWidget(
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

    // color encoding attributes
    _fiberLineWidth = 1.0;
    _fiberColorEncoding = 0;
    _fileNameScalarFeatureVolume = "";
    _fileNameScalarFeatureVolumeToBundleTransform3d = "";
    _lowerBoundString = "";
    _upperBoundString = "";
    
    // level of detail attributes
    _minRatioPercent = 1.0;
    _maxRatioPercent = 100;
    _maxLevelOfDetail = 5;
    _numberOfCubesPerAxis = 8;
    _currentLevelOfDetail = 1;

    // resampling attributes
    _fiberSubSamplingFactor = 1;
    _enableResampling = false;
    _resamplingMaximumStep = 10;
    _resamplingErrorThreshold = 0.20;

    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::AInteractiveBundleMapObject* 
        aInteractiveBundleMapObject =
                        dynamic_cast< gkg::AInteractiveBundleMapObject* >( *o );
      if ( aInteractiveBundleMapObject )
      {

        // get color encoding attributes
        _fiberLineWidth =
          aInteractiveBundleMapObject->getFiberLineWidth();
        _fiberColorEncoding =
          aInteractiveBundleMapObject->getFiberColorEncoding();
        _fileNameScalarFeatureVolume =
          aInteractiveBundleMapObject->getFileNameScalarFeatureVolume();
        _fileNameScalarFeatureVolumeToBundleTransform3d =
          aInteractiveBundleMapObject->
                            getFileNameScalarFeatureVolumeToBundleTransform3d();
        _lowerBoundString = 
          QString::number(  aInteractiveBundleMapObject->getLowerBound() );
        _upperBoundString = 
          QString::number( aInteractiveBundleMapObject->getUpperBound() );
        
        // get level of detail attributes
        _minRatioPercent =
          aInteractiveBundleMapObject->getMinRatioPercent();
        _maxRatioPercent =
          aInteractiveBundleMapObject->getMaxRatioPercent();
        _maxLevelOfDetail =
          aInteractiveBundleMapObject->getMaxLevelOfDetail();
        _numberOfCubesPerAxis =
          aInteractiveBundleMapObject->getNumberOfCubesPerAxis();
        _currentLevelOfDetail =
          aInteractiveBundleMapObject->getCurrentLevelOfDetail();
        
        setRatiosPercent();
        _currentPercent = _ratiosPercent[ _currentLevelOfDetail ];
        _currentPercentInt = (int32_t) ( _currentPercent + 0.5 );

        // get resampling attributes
        _fiberSubSamplingFactor =
          aInteractiveBundleMapObject->getFiberSubSamplingFactor();
        _enableResampling =
          aInteractiveBundleMapObject->getEnableResampling();
        _resamplingMaximumStep =
          aInteractiveBundleMapObject->getResamplingMaximumStep();
        _resamplingErrorThreshold =
          aInteractiveBundleMapObject->getResamplingErrorThreshold();
        
        break;

      }
      aInteractiveBundleMapObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    // original color encoding attributes
    _originalFiberLineWidth = _fiberLineWidth;
    _originalFiberColorEncoding = _fiberColorEncoding;
    _originalFileNameScalarFeatureVolume = _fileNameScalarFeatureVolume;
    _originalFileNameScalarFeatureVolumeToBundleTransform3d = 
                                _fileNameScalarFeatureVolumeToBundleTransform3d;
    _originalLowerBoundString = _lowerBoundString;
    _originalUpperBoundString = _upperBoundString;
    
    // original level of detail attributes
    _originalMinRatioPercent = _minRatioPercent;
    _originalMaxRatioPercent = _maxRatioPercent;
    _originalMaxLevelOfDetail = _maxLevelOfDetail;
    _originalNumberOfCubesPerAxis = _numberOfCubesPerAxis;
    _originalCurrentLevelOfDetail = _currentLevelOfDetail;

    // original resampling attributes
    _originalFiberSubSamplingFactor = _fiberSubSamplingFactor;
    _originalEnableResampling = _enableResampling;
    _originalResamplingMaximumStep = _resamplingMaximumStep;
    _originalResamplingErrorThreshold = _resamplingErrorThreshold;

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
  GKG_CATCH( "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "AInteractiveBundleMapRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::AInteractiveBundleMapRenderingOptionsWidget::
                                  ~AInteractiveBundleMapRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::update(
                                                   const anatomist::Observable*,
                                                   void* /* arg */ )
{
}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::updateLevelOfDetailPreload()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::AInteractiveBundleMapObject*
        aInteractiveBundleMapObject = 
                        dynamic_cast< gkg::AInteractiveBundleMapObject* >( *o );
      if ( aInteractiveBundleMapObject )
      {

        aInteractiveBundleMapObject->setMinRatioPercent( _minRatioPercent );
        aInteractiveBundleMapObject->setMaxRatioPercent( _maxRatioPercent );
        aInteractiveBundleMapObject->setMaxLevelOfDetail( _maxLevelOfDetail );
        aInteractiveBundleMapObject->setNumberOfCubesPerAxis( 
                                                        _numberOfCubesPerAxis );
        aInteractiveBundleMapObject->fillIndicesInCubesVector();

        _minRatioPercent = aInteractiveBundleMapObject->getMinRatioPercent();
        _maxRatioPercent = aInteractiveBundleMapObject->getMaxRatioPercent();

        aInteractiveBundleMapObject->notifyObservers( this );
        aInteractiveBundleMapObject->clearHasChangedFlags();

      }
      ++ o;

    }

    setRatiosPercent();

    _currentPercent = _ratiosPercent[ _currentLevelOfDetail ];
    _currentPercentInt = (int32_t) ( _currentPercent + 0.5 );

    _currentLevelOfDetailSpinBox->setMaximum( _maxLevelOfDetail );
    _currentLevelOfDetailSlider->setMaximum( _maxLevelOfDetail );
    _currentLevelOfDetailSlider->setValue( _currentLevelOfDetail );
    QString currentPercentStr = QString::number( _currentPercent, 'f', 1 );
    _currentLevelOfDetailProgressBar->setValue( _currentPercentInt );
    _currentLevelOfDetailProgressBar->setFormat( currentPercentStr + "%");

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "updateLevelOfDetailPreload()" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::updateResamplingPreload()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::AInteractiveBundleMapObject*
        aInteractiveBundleMapObject = 
                        dynamic_cast< gkg::AInteractiveBundleMapObject* >( *o );
      if ( aInteractiveBundleMapObject )
      {

        aInteractiveBundleMapObject->setEnableResampling( _enableResampling );
        aInteractiveBundleMapObject->setResamplingMaximumStep( 
                                                       _resamplingMaximumStep );
        aInteractiveBundleMapObject->setResamplingErrorThreshold(
                                                    _resamplingErrorThreshold );
        aInteractiveBundleMapObject->fillPointsToKeepVector();

        aInteractiveBundleMapObject->notifyObservers( this );
        aInteractiveBundleMapObject->clearHasChangedFlags();

      }
      ++ o;

    }

    _enableResamplingCheckBox->setChecked( _enableResampling );

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "updateResamplingPreload()" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::updateObjects()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::AInteractiveBundleMapObject*
        aInteractiveBundleMapObject = 
                        dynamic_cast< gkg::AInteractiveBundleMapObject* >( *o );
      if ( aInteractiveBundleMapObject )
      {

        // set color encoding attributes
        aInteractiveBundleMapObject->setFiberLineWidth( _fiberLineWidth );
        aInteractiveBundleMapObject->setFiberColorEncoding(
                                _fiberColorEncoding,
                                _fileNameScalarFeatureVolume,
                                _fileNameScalarFeatureVolumeToBundleTransform3d,
                                _lowerBoundString.toDouble(),
                                _upperBoundString.toDouble() );
        
        // set level of detail attributes
        if ( _currentLevelOfDetail != _originalCurrentLevelOfDetail )
        {
          aInteractiveBundleMapObject->setCurrentLevelOfDetail( 
                                                        _currentLevelOfDetail );
        }

        // set resampling attributes
        aInteractiveBundleMapObject->setFiberSubSamplingFactor( 
                                                      _fiberSubSamplingFactor );
        aInteractiveBundleMapObject->setEnableResampling( _enableResampling );

        aInteractiveBundleMapObject->notifyObservers( this );
        aInteractiveBundleMapObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "updateObjects()" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::drawContents()
{

  try
  {

    // main window and layout
    _mainWindow = new QWidget();
    QGridLayout* mainLayout = new QGridLayout( this );
    _mainWindow->setLayout( mainLayout );

    // moving main window to screen center
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = ( screenGeometry.width()-_mainWindow->width() ) / 2;
    int y = ( screenGeometry.height()-_mainWindow->height() ) / 2;
    _mainWindow->move( x, y );
    _mainWindow->show();

    // selection object to modify
    _objectParamSelect = new ObjectParamSelect( _objects, this );


    // building rendering widget from designer .ui file
    std::string uiPath = gkg::ConfigurationInfo::getInstance().getUiPath(
                                    "anatomist-plugin",
                                    "InteractiveBundleMapRenderingOptions.ui" );

    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();

    ////////////////////////////////////////////////////////////////////////////
    // color encoding widgets
    ////////////////////////////////////////////////////////////////////////////

    // fiber line width
    _fiberLineWidthDoubleSpinBox = qRootWidget->findChild< QDoubleSpinBox* >( 
                                               "doubleSpinBox_FiberLineWidth" );
    _fiberLineWidthDoubleSpinBox->setRange( 1.0, 10.0 );
    _fiberLineWidthDoubleSpinBox->setSingleStep( 0.1 );
    _fiberLineWidthDoubleSpinBox->setValue( _fiberLineWidth );

    // fiber color encoding
    _fiberColorEncodingComboBox = qRootWidget->findChild< QComboBox* >( 
                                                "comboBox_FiberColorEncoding" );
    _fiberColorEncodingComboBox->setEnabled( true );
    _fiberColorEncodingComboBox->addItem( "Fiber label" );
    _fiberColorEncodingComboBox->addItem( "Fiber direction" );
    _fiberColorEncodingComboBox->addItem( "Constant material" );
    _fiberColorEncodingComboBox->addItem( "Scalar feature from volume" );
    _fiberColorEncodingComboBox->setCurrentIndex( _fiberColorEncoding );

    _fiberColorEncodingStackedWidget =
                                      qRootWidget->findChild< QStackedWidget* >( 
                                           "stackedWidget_FiberColorEncoding" );
    _fiberColorEncodingStackedWidget->setCurrentIndex( _fiberColorEncoding );

    // filename scalar feature volume
    _fileNameScalarFeatureVolumeLineEdit = qRootWidget->findChild< QLineEdit* >(
                                       "lineEdit_FileNameScalarFeatureVolume" );
    _fileNameScalarFeatureVolumeLineEdit->setText(
                   QString::fromStdString( _fileNameScalarFeatureVolume ) );
    _fileNameScalarFeatureVolumePushButton = 
                                        qRootWidget->findChild< QPushButton* >(
                                     "pushButton_FileNameScalarFeatureVolume" );

    // filename scalar feature volume to bundle 3D tranformation
    _fileNameScalarFeatureVolumeToBundleTransform3dLineEdit =
                                           qRootWidget->findChild< QLineEdit* >(
                    "lineEdit_FileNameScalarFeatureVolumeToBundleTransform3d" );
    _fileNameScalarFeatureVolumeToBundleTransform3dLineEdit->setText(
                            QString::fromStdString( 
                            _fileNameScalarFeatureVolumeToBundleTransform3d ) );
    _fileNameScalarFeatureVolumeToBundleTransform3dPushButton = 
                                        qRootWidget->findChild< QPushButton* >(
                  "pushButton_FileNameScalarFeatureVolumeToBundleTransform3d" );

    // lower bound
    _lowerBoundLineEdit = qRootWidget->findChild< QLineEdit* >(
                                                        "lineEdit_LowerBound" );
    _lowerBoundLineEdit->setText( _lowerBoundString );

    // upper bound
    _upperBoundLineEdit = qRootWidget->findChild< QLineEdit* >(
                                                        "lineEdit_UpperBound" );
    _upperBoundLineEdit->setText( _upperBoundString );

    ////////////////////////////////////////////////////////////////////////////
    // level of detail widgets
    ////////////////////////////////////////////////////////////////////////////
    
    // min ratio percent
    _minRatioPercentCounter = qRootWidget->findChild< QwtCounter* >( 
                                                    "counter_MinRatioPercent" );
    _minRatioPercentCounter->setRange( 0.1, 100 );
    _minRatioPercentCounter->setSingleStep( 0.1 );
    _minRatioPercentCounter->setValue( _minRatioPercent );
    _minRatioPercentCounter->setNumButtons( 2 );
    _minRatioPercentCounter->setIncSteps( QwtCounter::Button1, 1 );
    _minRatioPercentCounter->setIncSteps( QwtCounter::Button2, 20 );

    // max ratio percent
    _maxRatioPercentCounter = qRootWidget->findChild< QwtCounter* >( 
                                                    "counter_MaxRatioPercent" );
    _maxRatioPercentCounter->setRange( 0.1, 100 );
    _maxRatioPercentCounter->setSingleStep( 0.1 );
    _maxRatioPercentCounter->setValue( _maxRatioPercent );
    _maxRatioPercentCounter->setNumButtons( 2 );
    _maxRatioPercentCounter->setIncSteps( QwtCounter::Button1, 1 );
    _maxRatioPercentCounter->setIncSteps( QwtCounter::Button2, 20 );

    // max level of detail
    _maxLevelOfDetailSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                                   "spinBox_MaxLevelOfDetail" );
    _maxLevelOfDetailSpinBox->setRange( 1, 10 );
    _maxLevelOfDetailSpinBox->setSingleStep( 1 );
    _maxLevelOfDetailSpinBox->setValue( _maxLevelOfDetail );

    // number of cubes per axis
    _numberOfCubesPerAxisSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                               "spinBox_NumberOfCubesPerAxis" );
    _numberOfCubesPerAxisSpinBox->setRange( 1, 30 );
    _numberOfCubesPerAxisSpinBox->setSingleStep( 1 );
    _numberOfCubesPerAxisSpinBox->setValue( _numberOfCubesPerAxis );

    // preload new level of detail
    _preloadNewLevelOfDetailPushButton = qRootWidget->findChild< QPushButton* >( 
                                         "pushButton_PreloadNewLevelOfDetail" );

    // current level of detail
    _currentLevelOfDetailSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                               "spinBox_CurrentLevelOfDetail" );
    _currentLevelOfDetailSpinBox->setRange( 1, _maxLevelOfDetail );
    _currentLevelOfDetailSpinBox->setSingleStep( 1 );
    _currentLevelOfDetailSpinBox->setValue( _currentLevelOfDetail );

    _currentLevelOfDetailSlider = qRootWidget->findChild< QSlider* >( 
                                                "slider_CurrentLevelOfDetail" );
    _currentLevelOfDetailSlider->setMinimum( 1 );
    _currentLevelOfDetailSlider->setMaximum( _maxLevelOfDetail );
    _currentLevelOfDetailSlider->setSingleStep( 1 );
    _currentLevelOfDetailSlider->setValue( _currentLevelOfDetail );

    QString currentPercentStr = QString::number( _currentPercent, 'f', 2 );
    _currentLevelOfDetailProgressBar = qRootWidget->findChild< QProgressBar* >( 
                                           "progressBar_CurrentLevelOfDetail" );
    _currentLevelOfDetailProgressBar->setRange( 0, 100 );
    _currentLevelOfDetailProgressBar->setValue( _currentPercentInt );
    _currentLevelOfDetailProgressBar->setFormat( currentPercentStr + "%");

    ////////////////////////////////////////////////////////////////////////////
    // resampling widgets
    ////////////////////////////////////////////////////////////////////////////

    // fiber sub-sampling factor
    _fiberSubSamplingFactorSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                             "spinBox_FiberSubSamplingFactor" );
    _fiberSubSamplingFactorSpinBox->setRange( 1, 25 );
    _fiberSubSamplingFactorSpinBox->setSingleStep( 1 );
    _fiberSubSamplingFactorSpinBox->setValue( _fiberSubSamplingFactor );

    // enable on the fly resampling
    _enableResamplingCheckBox = qRootWidget->findChild< QCheckBox* >( 
                                                  "checkBox_EnableResampling" );
    _enableResamplingCheckBox->setChecked( _enableResampling );

    // resampling maximum step
    _resamplingMaximumStepDoubleSpinBox
        = qRootWidget->findChild< QDoubleSpinBox* >( 
                                        "doubleSpinBox_ResamplingMaximumStep" );
    _resamplingMaximumStepDoubleSpinBox->setRange( 0.0, 100.0 );
    _resamplingMaximumStepDoubleSpinBox->setSingleStep( 1.0 );
    _resamplingMaximumStepDoubleSpinBox->setValue( _resamplingMaximumStep );

    // resampling error threshold
    _resamplingErrorThresholdDoubleSpinBox
        = qRootWidget->findChild< QDoubleSpinBox* >( 
                                     "doubleSpinBox_ResamplingErrorThreshold" );
    _resamplingErrorThresholdDoubleSpinBox->setRange( 0.0, 2.0 );
    _resamplingErrorThresholdDoubleSpinBox->setSingleStep( 0.05 );
    _resamplingErrorThresholdDoubleSpinBox->setValue(
                                                    _resamplingErrorThreshold );

    // preload new level of detail
    _preloadNewResamplingPushButton = qRootWidget->findChild< QPushButton* >( 
                                            "pushButton_PreloadNewResampling" );
    
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

    ////////////////////////////////////////////////////////////////////////////
    // color encoding links
    ////////////////////////////////////////////////////////////////////////////

    connect( _fiberLineWidthDoubleSpinBox,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( fiberLineWidthChanged( double ) ) );

    connect( _fiberColorEncodingComboBox,
             SIGNAL( currentIndexChanged( int ) ),
             this,
             SLOT( fiberColorEncodingChanged( int ) ) );

    connect( _fiberColorEncodingComboBox,
             SIGNAL( currentIndexChanged( int ) ),
             _fiberColorEncodingStackedWidget,
             SLOT( setCurrentIndex( int ) ) );

    connect( _fileNameScalarFeatureVolumeLineEdit,
             SIGNAL( textChanged( const QString& ) ),
             this,
             SLOT( fileNameScalarFeatureVolumeChanged( const QString& ) ) );

    connect( _fileNameScalarFeatureVolumePushButton,
             SIGNAL( clicked() ),
             this,
             SLOT( selectFileNameScalarFeatureVolume() ) );

    connect( _fileNameScalarFeatureVolumeToBundleTransform3dLineEdit,
             SIGNAL( textChanged( const QString& ) ),
             this,
             SLOT( fileNameScalarFeatureVolumeToBundleTransform3dChanged(
                                                           const QString& ) ) );

    connect( _fileNameScalarFeatureVolumeToBundleTransform3dPushButton,
             SIGNAL( clicked() ),
             this,
             SLOT( selectFileNameScalarFeatureVolumeToBundleTransform3d() ) );

    connect( _lowerBoundLineEdit,
             SIGNAL( textChanged( const QString& ) ),
             this,
             SLOT( lowerBoundChanged( const QString& ) ) );

    connect( _upperBoundLineEdit,
             SIGNAL( textChanged( const QString& ) ),
             this,
             SLOT( upperBoundChanged( const QString& ) ) );

    ////////////////////////////////////////////////////////////////////////////
    // level of detail links
    ////////////////////////////////////////////////////////////////////////////
    
    connect( _minRatioPercentCounter,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( minRatioPercentChanged( double ) ) );

    connect( _maxRatioPercentCounter,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( maxRatioPercentChanged( double ) ) );

    connect( _maxLevelOfDetailSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( maxLevelOfDetailChanged( int ) ) );
    
    connect( _numberOfCubesPerAxisSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( numberOfCubesPerAxisChanged( int ) ) );
    
    connect( _preloadNewLevelOfDetailPushButton,
             SIGNAL( clicked() ),
             this,
             SLOT( preloadNewLevelOfDetailDone() ) );
    
    connect( _currentLevelOfDetailSpinBox,
             SIGNAL( valueChanged( int ) ),
             _currentLevelOfDetailSlider,
             SLOT( setValue( int ) ) );
    
    connect( _currentLevelOfDetailSlider,
             SIGNAL( valueChanged( int ) ),
             _currentLevelOfDetailSpinBox,
             SLOT( setValue( int ) ) );
    
    connect( _currentLevelOfDetailSlider,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( currentLevelOfDetailChanged( int ) ) );
    
    ////////////////////////////////////////////////////////////////////////////
    // resampling links
    ////////////////////////////////////////////////////////////////////////////

    connect( _fiberSubSamplingFactorSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( fiberSubSamplingFactorChanged( int ) ) );

    connect( _enableResamplingCheckBox,
             SIGNAL( stateChanged( int ) ),
             this,
             SLOT( enableResamplingChanged( int ) ) );

    connect( _resamplingMaximumStepDoubleSpinBox,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( resamplingMaximumStepChanged( double ) ) );

    connect( _resamplingErrorThresholdDoubleSpinBox,
             SIGNAL( valueChanged( double ) ),
             this,
             SLOT( resamplingErrorThresholdChanged( double ) ) );

    connect( _preloadNewResamplingPushButton,
             SIGNAL( clicked() ),
             this,
             SLOT( preloadNewResamplingDone() ) );

    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( cancel() ) );
    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( accept() ) );


    _mainWindow->show();
 
  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "drawContents()" );

}


////////////////////////////////////////////////////////////////////////////////
// color encoding methods
////////////////////////////////////////////////////////////////////////////////

void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::fiberLineWidthChanged(
                                                                  double value )
{

  try
  {

    _fiberLineWidth = ( float )value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "fiberLineWidthChanged( double value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::fiberColorEncodingChanged(
                                                                     int value )
{

  try
  {

    _fiberColorEncoding = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "fiberColorEncodingChanged( int value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::
                      fileNameScalarFeatureVolumeChanged( const QString& value )
{

  try
  {

    _fileNameScalarFeatureVolume = value.toUtf8().constData();

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "fileNameScalarFeatureVolumeChanged( "
             "const QString& value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::
                                             selectFileNameScalarFeatureVolume()
{

  try
  {

    QString fileName = QFileDialog::getOpenFileName(
                                        this,
                                        tr("Open File"),
                                        ".",
                                        tr("Volumes (*.ima *.nii.gz *.nii)" ) );

    _fileNameScalarFeatureVolume = fileName.toUtf8().constData();
    _fileNameScalarFeatureVolumeLineEdit->setText( fileName );
 
    

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "selectFileNameScalarFeatureVolume()" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::
                         fileNameScalarFeatureVolumeToBundleTransform3dChanged(
                                                          const QString& value )
{

  try
  {

    _fileNameScalarFeatureVolumeToBundleTransform3d =
                                                     value.toUtf8().constData();

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "fileNameScalarFeatureVolumeToBundleTransform3dChanged( "
             "const QString& value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::
                          selectFileNameScalarFeatureVolumeToBundleTransform3d()
{

  try
  {

    QString fileName = QFileDialog::getOpenFileName(
                                            this,
                                            tr("Open File"),
                                            ".",
                                            tr("3D Transformations (*.trm)" ) );

    _fileNameScalarFeatureVolumeToBundleTransform3d =
                                                  fileName.toUtf8().constData();
    _fileNameScalarFeatureVolumeToBundleTransform3dLineEdit->setText(
                                                                     fileName );

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "selectFileNameScalarFeatureVolumeToBundleTransform3d()" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::lowerBoundChanged(
                                                          const QString& value )
{

  try
  {

    _lowerBoundString = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "lowerBoundChanged( "
             "const QString& value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::upperBoundChanged(
                                                          const QString& value )
{

  try
  {

    _upperBoundString = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "upperBoundChanged( "
             "const QString& value )" );

}


////////////////////////////////////////////////////////////////////////////////
// level of detail methods
////////////////////////////////////////////////////////////////////////////////

void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::minRatioPercentChanged(
                                                                  double value )
{

  try
  {

    _minRatioPercent = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "minRatioPercentChanged( double value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::maxRatioPercentChanged(
                                                                  double value )
{

  try
  {

    _maxRatioPercent = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "maxRatioPercentChanged( double value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::maxLevelOfDetailChanged(
                                                                     int value )
{

  try
  {

    _maxLevelOfDetail = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "maxLevelOfDetailChanged( int value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::numberOfCubesPerAxisChanged(
                                                                     int value )
{

  try
  {

    _numberOfCubesPerAxis = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "numberOfCubesPerAxisChanged( int value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::preloadNewLevelOfDetailDone()
{

  try
  {

    updateLevelOfDetailPreload();

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "preloadNewLevelOfDetailDone()" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::currentLevelOfDetailChanged(
                                                                     int value )
{

  try
  {

    _currentLevelOfDetail = value;
    _currentPercent = _ratiosPercent[ _currentLevelOfDetail ];
    _currentPercentInt = (int32_t) ( _currentPercent + 0.5 );
    QString currentPercentStr = QString::number( _currentPercent, 'f', 1 );
    _currentLevelOfDetailProgressBar->setValue( _currentPercentInt );
    _currentLevelOfDetailProgressBar->setFormat( currentPercentStr + "%");

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "currentLevelOfDetailChanged( int value )" );

}


////////////////////////////////////////////////////////////////////////////////
// resampling methods
////////////////////////////////////////////////////////////////////////////////

void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::fiberSubSamplingFactorChanged(
                                                                     int value )
{

  try
  {

    _fiberSubSamplingFactor = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "fiberSubSamplingFactorChanged( int value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::enableResamplingChanged( 
                                                                     int value )
{

  try
  {

    _enableResampling = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "enableResamplingChanged( int value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::resamplingMaximumStepChanged(
                                                                  double value )
{

  try
  {

    _resamplingMaximumStep = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "resamplingMaximumStepChanged( double value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::
                                 resamplingErrorThresholdChanged( double value )
{

  try
  {

    _resamplingErrorThreshold = value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "resamplingErrorThresholdChanged( double value )" );

}


void 
gkg::AInteractiveBundleMapRenderingOptionsWidget::preloadNewResamplingDone()
{

  try
  {

    _enableResampling = true;
    updateResamplingPreload();

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "preloadNewResamplingDone()" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::cancel()
{

  try
  {

    _fiberLineWidth = _originalFiberLineWidth;
    _fiberColorEncoding = _originalFiberColorEncoding;
    _fileNameScalarFeatureVolume = _originalFileNameScalarFeatureVolume;
    _fileNameScalarFeatureVolumeToBundleTransform3d = 
                                _fileNameScalarFeatureVolumeToBundleTransform3d;
    _lowerBoundString = _lowerBoundString;
    _upperBoundString = _upperBoundString;
    
    _currentLevelOfDetail = _originalCurrentLevelOfDetail;

    _fiberSubSamplingFactor = _originalFiberSubSamplingFactor;
    _enableResampling = _originalEnableResampling;
    _resamplingMaximumStep = _originalResamplingMaximumStep;
    _resamplingErrorThreshold = _originalResamplingErrorThreshold;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::cancel()" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    updateObjects();

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "accept()" );

}


////////////////////////////////////////////////////////////////////////////////
// other methods
////////////////////////////////////////////////////////////////////////////////

void gkg::AInteractiveBundleMapRenderingOptionsWidget::setRatiosPercent()
{

  try
  {

    _ratiosPercent = std::vector < double > ( _maxLevelOfDetail + 1 );
    _ratiosPercent[ 0 ] = 0;

    if ( _maxLevelOfDetail > 1 )
    {


      for ( int levelOfDetail = 1;
            levelOfDetail <= _maxLevelOfDetail;
            ++ levelOfDetail )
      {

        double ratio = _minRatioPercent
                       * pow( _maxRatioPercent / _minRatioPercent,
                              ( (double)levelOfDetail - 1 ) 
                              / ( (double)_maxLevelOfDetail - 1 ) );
        _ratiosPercent[ levelOfDetail ] = ratio;

      }

    }
    else if ( _maxLevelOfDetail == 1 )
    {

      _ratiosPercent[ 1 ] = _maxRatioPercent;

    }

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "setRatiosPercent() " );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "chooseObject()" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::runCommand()
{
}


void gkg::AInteractiveBundleMapRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::AInteractiveBundleMapRenderingOptionsWidget*
    //  aOdfFieldRenderingOptionsWidget =
    new gkg::AInteractiveBundleMapRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}