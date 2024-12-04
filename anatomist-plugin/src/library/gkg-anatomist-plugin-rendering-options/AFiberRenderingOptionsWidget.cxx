#include <gkg-anatomist-plugin-rendering-options/AFiberRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/AFiberObject.h>
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
#include <limits>


gkg::AFiberRenderingOptionsWidget::AFiberRenderingOptionsWidget(
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

      gkg::AFiberObject* 
        aFiberObject = dynamic_cast< gkg::AFiberObject* >( *o );
      if ( aFiberObject )
      {

        this->_minimumValue = aFiberObject->getMinimumValue();
        this->_maximumValue = aFiberObject->getMaximumValue();
        this->_minimumRange = aFiberObject->getMinimumRange();
        this->_maximumRange = aFiberObject->getMaximumRange();
        this->_samplingValue = aFiberObject->getSamplingValue();
        this->_fileNameData = aFiberObject->getFileNameData();
        this->_fileNameVolume = aFiberObject->getFileNameVolume();
        this->_dataType = aFiberObject->getDataType();

        break;

      }
      aFiberObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    this->_originalMinimumValue = this->_minimumValue;
    this->_originalMaximumValue = this->_maximumValue;
    this->_originalMinimumRange = this->_minimumRange;
    this->_originalMaximumRange = this->_maximumRange;
    this->_originalSamplingValue = this->_samplingValue;
    this->_originalFileNameData = this->_fileNameData;
    this->_originalFileNameVolume = this->_fileNameVolume;
    this->_originalDataType = this->_dataType;

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
  GKG_CATCH( "gkg::AFiberRenderingOptionsWidget::"
             "AFiberRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::AFiberRenderingOptionsWidget::~AFiberRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::AFiberRenderingOptionsWidget::update( const anatomist::Observable*,
                                                void* /* arg */ )
{
}


void gkg::AFiberRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::AFiberRenderingOptionsWidget::updateObjects()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::AFiberObject*
                     aFiberObject = dynamic_cast< gkg::AFiberObject * >( *o );
      if ( aFiberObject )
      {

        aFiberObject->setRenderingOptions( _minimumValue,
                                           _maximumValue,
                                           _minimumRange,
                                           _maximumRange,
                                           _samplingValue,
                                           _dataType,
                                           _fileNameData,
                                           _fileNameVolume );
        _minimumRange = aFiberObject->getMinimumRange();
        _maximumRange = aFiberObject->getMaximumRange();

        aFiberObject->notifyObservers( this );
        aFiberObject->clearHasChangedFlags();

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::updateObjects()" );

}


void gkg::AFiberRenderingOptionsWidget::drawContents()
{

  try
  {

    // main window and layout
    _mainWindow = new QWidget();
    QGridLayout* mainLayout = new QGridLayout( this );
    _mainWindow->setLayout( mainLayout );

    // selection object to modify
    _objectParamSelect = new ObjectParamSelect( _objects, this );

    // fiber options group box
    QGroupBox* fiberOptionsGroupBox = new QGroupBox( "Fiber options" );
    QGridLayout* fiberOptionsLayout = new QGridLayout();
    fiberOptionsGroupBox->setLayout( fiberOptionsLayout );

    QLabel* samplingValueLabel = new QLabel( "Fibers sampling value" );
    fiberOptionsLayout->addWidget( samplingValueLabel, 0, 0, 1, 1 );
    QSpinBox* _samplingValueSpinBox = new QSpinBox();
    _samplingValueSpinBox->setRange( 2, 1000 );
    _samplingValueSpinBox->setValue( _samplingValue );
    fiberOptionsLayout->addWidget( _samplingValueSpinBox, 0, 1, 1, 1 );

    // options group box
    QGroupBox* optionsGroupBox = new QGroupBox( "Options" );
    QGridLayout* optionsLayout = new QGridLayout();
    optionsGroupBox->setLayout( optionsLayout );

    QLabel* minLabel = new QLabel( "Min:" );
    optionsLayout->addWidget( minLabel, 0, 0, 1, 1 );
    QLabel* maxLabel = new QLabel( "Max:" );
    optionsLayout->addWidget( maxLabel, 1, 0, 1, 1 );
    QLabel* rangeLabel = new QLabel( "Range:" );
    optionsLayout->addWidget( rangeLabel, 2, 0, 1, 1 );

    _minimumValueSlider = new QSlider();
    optionsLayout->addWidget( _minimumValueSlider, 0, 1, 1, 2 );
    _minimumValueSlider->setSliderPosition( 0 );
    _minimumValueSlider->setOrientation(Qt::Horizontal);
    _maximumValueSlider = new QSlider();
    optionsLayout->addWidget( _maximumValueSlider, 1, 1, 1, 2 );
    _maximumValueSlider->setSliderPosition( 99 );
    _maximumValueSlider->setOrientation(Qt::Horizontal);

    _minimumValueLabel = new QLabel();
    optionsLayout->addWidget( _minimumValueLabel, 0, 3, 1, 1 );
    _minimumValueLabel->setText( QString::number( _minimumValue ) );
    _maximumValueLabel = new QLabel();
    optionsLayout->addWidget( _maximumValueLabel, 1, 3, 1, 1 );
    _maximumValueLabel->setText( QString::number( _maximumValue ) );

    _minimumRangeLineEdit = new QLineEdit();
    optionsLayout->addWidget( _minimumRangeLineEdit, 2, 1, 1, 1 );
    _minimumRangeLineEdit->setText( QString::number( _minimumRange ) );
    _maximumRangeLineEdit = new QLineEdit();
    optionsLayout->addWidget( _maximumRangeLineEdit, 2, 2, 1, 1 );
    _maximumRangeLineEdit->setText( QString::number( _maximumRange ) );

    _fileNameDataLineEdit = new QLineEdit();
    optionsLayout->addWidget( _fileNameDataLineEdit, 3, 2, 1, 2 );
    QLabel* fileNameDataLabel = new QLabel( "Data file name:" );
    optionsLayout->addWidget( fileNameDataLabel, 3, 0, 1, 2 );

    _fileNameVolumeLineEdit = new QLineEdit();
    optionsLayout->addWidget( _fileNameVolumeLineEdit, 4, 2, 1, 2 );
    QLabel* fileNameVolumeLabel = new QLabel( "Volume file name:" );
    optionsLayout->addWidget( fileNameVolumeLabel, 4, 0, 1, 2 );

    // data type group box
    QGroupBox* dataTypeGroupBox = new QGroupBox( "Data type" );
    QGridLayout* dataTypeLayout = new QGridLayout();
    dataTypeGroupBox->setLayout( dataTypeLayout );

    QRadioButton* _noDataRadioButton = new QRadioButton( "No Data" );
    dataTypeLayout->addWidget( _noDataRadioButton, 0, 0, 1, 1 );
    if ( _dataType == "no-data" )
    {

      _noDataRadioButton->setChecked( true );
      if ( _originalDataType == "no-data" )
      {

        _minimumValueSlider->setEnabled( false );
        _maximumValueSlider->setEnabled( false );
        _minimumValueLabel->setEnabled( false );
        _maximumValueLabel->setEnabled( false );
        _minimumRangeLineEdit->setEnabled( false );
        _maximumRangeLineEdit->setEnabled( false );
        _fileNameDataLineEdit->setEnabled( false );
        _fileNameVolumeLineEdit->setEnabled( false );

      }

    }
    else
    {

      _noDataRadioButton->setChecked( false );

    }
    QRadioButton* _orientationRadioButton = new QRadioButton( "Orientations" );
    dataTypeLayout->addWidget( _orientationRadioButton, 1, 0, 1, 1 );
    if ( _dataType == "orientation" )
    {

      _orientationRadioButton->setChecked( true );
      if ( _originalDataType == "orientation" )
      {

        _minimumValueSlider->setEnabled( false );
        _maximumValueSlider->setEnabled( false );
        _minimumValueLabel->setEnabled( false );
        _maximumValueLabel->setEnabled( false );
        _minimumRangeLineEdit->setEnabled( false );
        _maximumRangeLineEdit->setEnabled( false );
        _fileNameDataLineEdit->setEnabled( false );
        _fileNameVolumeLineEdit->setEnabled( false );

      }

    }
    else
    {

      _orientationRadioButton->setChecked( false );

    }
    QRadioButton* _lengthRadioButton = new QRadioButton( "Length" );
    dataTypeLayout->addWidget( _lengthRadioButton, 2, 0, 1, 1 );
    if ( _dataType == "length" )
    {

      _lengthRadioButton->setChecked( true );
      if ( _originalDataType == "length" )
      {

        float diffMaxMinRange = _originalMaximumRange - _originalMinimumRange;
        int32_t minimumValue = 99 * ( _minimumValue - _originalMinimumRange ) /
                               diffMaxMinRange;
        int32_t maximumValue = 99 * ( _maximumValue - _originalMaximumRange ) /
                               diffMaxMinRange;

        _minimumValueSlider->setEnabled( true );
        _minimumValueSlider->setSliderPosition( minimumValue );
        _maximumValueSlider->setEnabled( true );
        _maximumValueSlider->setSliderPosition( maximumValue );
        _minimumValueLabel->setEnabled( true );
        _minimumValueLabel->setText( QString::number( _originalMinimumRange ) );
        _maximumValueLabel->setEnabled( true );
        _maximumValueLabel->setText( QString::number( _originalMaximumRange ) );

        _minimumRangeLineEdit->setEnabled( true );
        _minimumRangeLineEdit->setText(
                                     QString::number( _originalMinimumRange ) );
        _maximumRangeLineEdit->setEnabled( true );
        _maximumRangeLineEdit->setText(
                                     QString::number( _originalMaximumRange ) );
        _fileNameDataLineEdit->setEnabled( false );
        _fileNameVolumeLineEdit->setEnabled( false );

      }

    }
    else
    {

      _lengthRadioButton->setChecked( false );

    }
    QRadioButton* _bundleProfileRadioButton =
                                     new QRadioButton( "Profile along bundle" );
    dataTypeLayout->addWidget( _bundleProfileRadioButton, 3, 0, 1, 1 );
    if ( _dataType == "profile-along-bundle" )
    {

      _bundleProfileRadioButton->setChecked( true );
      if ( _originalDataType == "profile-along-bundle" )
      {

        float diffMaxMinRange = _originalMaximumRange - _originalMinimumRange;
        int32_t minimumValue = 99 * ( _minimumValue - _originalMinimumRange ) /
                               diffMaxMinRange;
        int32_t maximumValue = 99 * ( _maximumValue - _originalMaximumRange ) /
                               diffMaxMinRange;

        _minimumValueSlider->setEnabled( true );
        _minimumValueSlider->setSliderPosition( minimumValue );
        _maximumValueSlider->setEnabled( true );
        _maximumValueSlider->setSliderPosition( maximumValue );
        _minimumValueLabel->setEnabled( true );
        _minimumValueLabel->setText( QString::number( _originalMinimumRange ) );
        _maximumValueLabel->setEnabled( true );
        _maximumValueLabel->setText( QString::number( _originalMaximumRange ) );

        _minimumRangeLineEdit->setEnabled( true );
        _minimumRangeLineEdit->setText( 
                                     QString::number( _originalMinimumRange ) );
        _maximumRangeLineEdit->setEnabled( true );
        _maximumRangeLineEdit->setText(
                                     QString::number( _originalMaximumRange ) );
        _fileNameDataLineEdit->setEnabled( true );
        _fileNameDataLineEdit->setText( 
                            QString::fromStdString( _originalFileNameData ) );
        _fileNameVolumeLineEdit->setEnabled( true );
        _fileNameVolumeLineEdit->setText( 
                            QString::fromStdString( _originalFileNameVolume ) );

      }

    }
    else
    {

      _lengthRadioButton->setChecked( false );

    }
    QRadioButton* _bundleColorRadioButton =
                                     new QRadioButton( "Color along bundle" );
    dataTypeLayout->addWidget( _bundleColorRadioButton,  4, 0, 1, 1 );
    if ( _dataType == "color-along-bundle" )
    {

      _bundleColorRadioButton->setChecked( true );
      if ( _originalDataType == "color-along-bundle" )
      {

        _minimumValueSlider->setEnabled( false );
        _maximumValueSlider->setEnabled( false );
        _minimumValueLabel->setEnabled( false );
        _maximumValueLabel->setEnabled( false );
        _minimumRangeLineEdit->setEnabled( false );
        _maximumRangeLineEdit->setEnabled( false );
        _fileNameDataLineEdit->setEnabled( true );
        _fileNameDataLineEdit->setText( 
                              QString::fromStdString( _originalFileNameData ) );
        _fileNameVolumeLineEdit->setEnabled( false );

      }

    }
    else
    {

      _lengthRadioButton->setChecked( false );

    }


    // validation group box
    QGroupBox* validationGroupBox = new QGroupBox();
    validationGroupBox->setFlat( true );
    QGridLayout* validationLayout = new QGridLayout();
    validationGroupBox->setLayout( validationLayout );
 
    QPushButton* cancelPushButton = new QPushButton( "Cancel" );
    QPushButton* okPushButton = new QPushButton( "Ok" );
 
    validationLayout->addWidget( cancelPushButton, 0, 0, 1, 1 );
    validationLayout->addWidget( okPushButton, 0, 1, 1, 1 );
 
    mainLayout->addWidget( fiberOptionsGroupBox, 0, 0, 1, 2 );
    mainLayout->addWidget( dataTypeGroupBox, 1, 0, 1, 2 );
    mainLayout->addWidget( optionsGroupBox, 1, 1, 1, 2 );
    mainLayout->addWidget( validationGroupBox, 2, 0, 2, 1 );

    // sampling changed
    connect( _samplingValueSpinBox, SIGNAL( valueChanged( int ) ),
             this, SLOT( samplingValueChanged( int ) ) );

    // data type toggled
    connect( _noDataRadioButton, SIGNAL( toggled( bool ) ),
             this, SLOT( noDataDataTypeChanged( bool ) ) );
    connect( _orientationRadioButton, SIGNAL( toggled( bool ) ),
             this, SLOT( orientationDataTypeChanged( bool ) ) );
    connect( _lengthRadioButton, SIGNAL( toggled(bool) ),
             this, SLOT( lengthDataTypeChanged( bool ) ) );
    connect( _bundleProfileRadioButton, SIGNAL( toggled(bool) ),
             this, SLOT( bundleProfileDataTypeChanged( bool ) ) );
    connect( _bundleColorRadioButton, SIGNAL( toggled( bool ) ),
             this, SLOT( colorBundleProfileDataTypeChanged( bool ) ) );

    // options changed
    connect( _minimumValueSlider, SIGNAL( sliderMoved( int ) ),
             this, SLOT( minimumSliderValueChanged( int ) ) );
    connect( _maximumValueSlider, SIGNAL( sliderMoved( int ) ),
             this, SLOT( maximumSliderValueChanged( int ) ) );
    connect( _minimumRangeLineEdit, SIGNAL( textChanged(const QString&) ),
             this, SLOT( minimumRangeChanged( const QString ) ) );
    connect( _maximumRangeLineEdit, SIGNAL( textChanged(const QString&) ),
             this, SLOT( maximumRangeChanged( const QString ) ) );
    connect( _fileNameDataLineEdit, SIGNAL( textChanged(const QString&) ),
             this, SLOT( fileNameDataChanged( const QString ) ) );
    connect( _fileNameVolumeLineEdit, SIGNAL( textChanged(const QString&) ),
             this, SLOT( fileNameVolumeChanged( const QString ) ) );

    // validation connection
    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( cancel() ) );
    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    _mainWindow->show();
 
  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::drawContents()" );

}


void gkg::AFiberRenderingOptionsWidget::noDataDataTypeChanged( bool value )
{

  try
  {

    if ( value )
    {

      _dataType = "no-data";
      _minimumValueSlider->setEnabled( false );
      _maximumValueSlider->setEnabled( false );
      _minimumValueLabel->setEnabled( false );
      _maximumValueLabel->setEnabled( false );
      _minimumRangeLineEdit->setEnabled( false );
      _maximumRangeLineEdit->setEnabled( false );
      _fileNameDataLineEdit->setEnabled( false );
      _fileNameVolumeLineEdit->setEnabled( false );

      updateObjects();

    }

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::noDataDataTypeChanged( "
             "bool value )" );

}


void gkg::AFiberRenderingOptionsWidget::orientationDataTypeChanged( bool value )
{

  try
  {

    if ( value )
    {

      _dataType = "orientation";
      _minimumValueSlider->setEnabled( false );
      _maximumValueSlider->setEnabled( false );
      _minimumValueLabel->setEnabled( false );
      _maximumValueLabel->setEnabled( false );
      _minimumRangeLineEdit->setEnabled( false );
      _maximumRangeLineEdit->setEnabled( false );
      _fileNameDataLineEdit->setEnabled( false );
      _fileNameVolumeLineEdit->setEnabled( false );

      updateObjects();

    }

  }
  GKG_CATCH(
          "void gkg::AFiberRenderingOptionsWidget::orientationDataTypeChanged( "
          "bool value )" );

}


void gkg::AFiberRenderingOptionsWidget::lengthDataTypeChanged( bool value )
{

  try
  {

    if ( value )
    {

      _dataType = "length";

      _minimumValueSlider->setEnabled( true );
      _maximumValueSlider->setEnabled( true );
      _minimumValueLabel->setEnabled( true );
      _maximumValueLabel->setEnabled( true );
      _minimumRangeLineEdit->setEnabled( true );
      _maximumRangeLineEdit->setEnabled( true );
      _fileNameDataLineEdit->setEnabled( true );
      _fileNameDataLineEdit->setText( 
                              QString::fromStdString( _originalFileNameData ) );
      _fileNameVolumeLineEdit->setEnabled( true );
      _fileNameVolumeLineEdit->setText( 
                            QString::fromStdString( _originalFileNameVolume ) );

      _minimumRange = std::numeric_limits< double >::lowest();
      _maximumRange = std::numeric_limits< double >::max();
      updateObjects();
      setRange( _minimumRange,
                _maximumRange );

    }

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::lengthDataTypeChanged( "
             "bool value )" );

}


void gkg::AFiberRenderingOptionsWidget::bundleProfileDataTypeChanged(
                                                                    bool value )
{

  try
  {

    if ( value )
    {

      _dataType = "profile-along-bundle";
      _minimumValueSlider->setEnabled( true );
      _maximumValueSlider->setEnabled( true );
      _minimumValueLabel->setEnabled( true );
      _maximumValueLabel->setEnabled( true );
      _minimumRangeLineEdit->setEnabled( true );
      _maximumRangeLineEdit->setEnabled( true );
      _fileNameDataLineEdit->setEnabled( true );
      _fileNameDataLineEdit->setEnabled( true );
      _fileNameVolumeLineEdit->setEnabled( true );

      _minimumRange = std::numeric_limits< double >::lowest();
      _maximumRange = std::numeric_limits< double >::max();
      _maximumValue = std::numeric_limits< double >::max();
      _minimumValue = std::numeric_limits< double >::lowest();
      updateObjects();
      setRange( _minimumRange, _maximumRange );

    }

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::"
             "bundleProfileDataTypeChanged( "
             "bool value )" );

}

void gkg::AFiberRenderingOptionsWidget::colorBundleProfileDataTypeChanged( 
                                                                    bool value )
{

  try
  {

    if ( value )
    {

      _dataType = "color-along-bundle";
      _minimumValueSlider->setEnabled( false );
      _maximumValueSlider->setEnabled( false );
      _minimumValueLabel->setEnabled( false );
      _maximumValueLabel->setEnabled( false );
      _minimumRangeLineEdit->setEnabled( false );
      _maximumRangeLineEdit->setEnabled( false );
      _fileNameDataLineEdit->setEnabled( false );
      _fileNameDataLineEdit->setEnabled( false );
      _fileNameVolumeLineEdit->setEnabled( false );

      updateObjects();

    }

  }
  GKG_CATCH(
   "void gkg::AFiberRenderingOptionsWidget::colorBundleProfileDataTypeChanged( "
   "bool value )" );

}


void 
gkg::AFiberRenderingOptionsWidget::minimumRangeChanged( const QString& value )
{

  _minimumRange = value.toDouble();
  setRange( _minimumRange, _maximumRange) ;
  updateObjects();

}


void 
gkg::AFiberRenderingOptionsWidget::maximumRangeChanged( const QString& value )
{

  _maximumRange = value.toDouble();
  setRange( _minimumRange, _maximumRange) ;
  updateObjects();

}


void 
gkg::AFiberRenderingOptionsWidget::minimumSliderValueChanged( int value )
{

  _minimumValue = ( ( _maximumRange - _minimumRange ) * value / 99 ) +
                  _minimumRange;

  _minimumValueLabel->setText( QString::number( _minimumValue ) );

  updateObjects();

}


void 
gkg::AFiberRenderingOptionsWidget::maximumSliderValueChanged( int value )
{

  _maximumValue = ( ( _maximumRange - _minimumRange ) * value / 99 ) +
                  _minimumRange;
  _maximumValueLabel->setText( QString::number( _maximumValue ) );
  updateObjects();

}


void 
gkg::AFiberRenderingOptionsWidget::samplingValueChanged( int value )
{

  _samplingValue = value;
  updateObjects();

}


void 
gkg::AFiberRenderingOptionsWidget::fileNameDataChanged( const QString text )
{

  _fileNameData = text.toUtf8().constData();
  updateObjects();
  setRange( _minimumRange, _maximumRange );

}


void 
gkg::AFiberRenderingOptionsWidget::fileNameVolumeChanged( const QString text )
{

  _fileNameVolume = text.toUtf8().constData();
  updateObjects();
  setRange( _minimumRange, _maximumRange );

}


void gkg::AFiberRenderingOptionsWidget::cancel()
{

  try
  {

    _dataType = _originalDataType;
    _minimumValue = _originalMinimumValue;
    _maximumValue = _originalMaximumValue;
    _fileNameData = _originalFileNameData;
    _fileNameVolume = _originalFileNameVolume;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::cancel()" );

}


void gkg::AFiberRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    std::set< anatomist::AObject * >::const_iterator 
    o = _objects.begin(),
    oe = _objects.end();

    while ( o != oe )
    {

      gkg::AFiberObject*
                     aFiberObject = dynamic_cast< gkg::AFiberObject* >( *o );
      if ( aFiberObject )
      {

        aFiberObject->setRenderingOptions( _minimumValue,
                                           _maximumValue,
                                           _minimumRange,
                                           _maximumRange,
                                           _samplingValue,
                                           _dataType,
                                           _fileNameData,
                                           _fileNameVolume );

      }

      ++ o;

    }
    updateObjects();

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::accept()" );

}


void gkg::AFiberRenderingOptionsWidget::setRange( const double minimumRange,
                                                  const double maximumRange )
{

  try
  {

    std::cout << "setRange!!" << minimumRange 
              << " " << maximumRange << std::endl << std::flush;

    _minimumRangeLineEdit->setEnabled( true );
    _minimumRangeLineEdit->setText( QString::number( minimumRange ) );
    _maximumRangeLineEdit->setEnabled( true );
    _maximumRangeLineEdit->setText( QString::number( maximumRange ) );

    float diffMaxMinRange = maximumRange - minimumRange;

    if ( ( _minimumValue < minimumRange ) || ( _minimumValue > _maximumValue ) )
    {

      _minimumValue = minimumRange;

    }
    if ( ( _maximumValue > maximumRange ) || ( _maximumValue < _minimumValue ) )
    {

      _maximumValue = maximumRange;

    }

    int32_t minimumValue = 99 * ( _minimumValue - minimumRange ) /
                           diffMaxMinRange;
    int32_t maximumValue = 99 * ( _maximumValue - minimumRange ) /
                           diffMaxMinRange;


    _minimumValueLabel->setEnabled( true );
    _minimumValueLabel->setText( QString::number( _minimumValue ) );
    _maximumValueLabel->setEnabled( true );
    _maximumValueLabel->setText( QString::number( _maximumValue ) );

    _minimumValueSlider->setEnabled( true );
    _minimumValueSlider->setSliderPosition( minimumValue );
    _maximumValueSlider->setEnabled( true );
    _maximumValueSlider->setSliderPosition( maximumValue );

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::setRange("
             "const double minimumRange, const double maximumRange )" );

}


void gkg::AFiberRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::chooseObject()" );

}


void gkg::AFiberRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::AFiberRenderingOptionsWidget::runCommand()
{
}


void gkg::AFiberRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::AFiberRenderingOptionsWidget* aFiberRenderingOptionsWidget =
    new gkg::AFiberRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::AFiberRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
