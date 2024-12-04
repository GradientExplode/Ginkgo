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

    _fiberSubSamplingFactor = 1;
    _fiberLineWidth = 1.0;
    _fiberColorEncoding = 0;
    _fileNameScalarFeatureVolume = "";
    _fileNameScalarFeatureVolumeToBundleTransform3d = "";
    _lowerBoundString = "";
    _upperBoundString = "";

    std::set< anatomist::AObject* >::const_iterator o = _objects.begin(),
                                                    oe = _objects.end();
    while ( o != oe )
    {

      gkg::AInteractiveBundleMapObject* 
        aInteractiveBundleMapObject =
                        dynamic_cast< gkg::AInteractiveBundleMapObject* >( *o );
      if ( aInteractiveBundleMapObject )
      {

        _fiberSubSamplingFactor = 
          aInteractiveBundleMapObject->getFiberSubSamplingFactor();
        _fiberLineWidth =
          aInteractiveBundleMapObject->getFiberLineWidth();
        _fiberColorEncoding =
          aInteractiveBundleMapObject->getFiberColorEncoding();
        _fileNameScalarFeatureVolume =
          aInteractiveBundleMapObject->getFileNameScalarFeatureVolume();
        _fileNameScalarFeatureVolumeToBundleTransform3d =
          aInteractiveBundleMapObject->getFileNameScalarFeatureVolumeToBundleTransform3d();
        _lowerBoundString = 
          QString::number(  aInteractiveBundleMapObject->getLowerBound() );
        _upperBoundString = 
          QString::number( aInteractiveBundleMapObject->getUpperBound() );
        break;

      }
      aInteractiveBundleMapObject->addObserver( ( anatomist::Observer*)this );
      ++ o;

    }

    _originalFiberSubSamplingFactor = _fiberSubSamplingFactor;
    _originalFiberLineWidth = _fiberLineWidth;
    _originalFiberColorEncoding = _fiberColorEncoding;
    _originalFileNameScalarFeatureVolume = _fileNameScalarFeatureVolume;
    _originalFileNameScalarFeatureVolumeToBundleTransform3d = 
                                _fileNameScalarFeatureVolumeToBundleTransform3d;
    _originalLowerBoundString = _lowerBoundString;
    _originalUpperBoundString = _upperBoundString;

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


gkg::AInteractiveBundleMapRenderingOptionsWidget::~AInteractiveBundleMapRenderingOptionsWidget()
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

        aInteractiveBundleMapObject->setFiberSubSamplingFactor(
                                                      _fiberSubSamplingFactor );
        aInteractiveBundleMapObject->setFiberLineWidth( _fiberLineWidth );
        aInteractiveBundleMapObject->setFiberColorEncoding(
                                _fiberColorEncoding,
                                _fileNameScalarFeatureVolume,
                                _fileNameScalarFeatureVolumeToBundleTransform3d,
                                _lowerBoundString.toDouble(),
                                _upperBoundString.toDouble() );

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

    // selection object to modify
    _objectParamSelect = new ObjectParamSelect( _objects, this );


    // buidling rendering widget from designer .ui file
    std::string uiPath = gkg::ConfigurationInfo::getInstance().getUiPath(
                                   "anatomist-plugin",
                                   "InteractiveBundleMapRenderingOptions.ui"  );

    QFile file( uiPath.c_str() );
    file.open( QFile::ReadOnly );
    QUiLoader qUiLoader;
    QWidget* qRootWidget = qUiLoader.load( &file );
    file.close();

    // fiber sub-sampling factor
    _fiberSubSamplingFactorSpinBox = qRootWidget->findChild< QSpinBox* >( 
                                             "spinBox_FiberSubSamplingFactor" );
    _fiberSubSamplingFactorSpinBox->setRange( 1, 100 );
    _fiberSubSamplingFactorSpinBox->setSingleStep( 1 );
    _fiberSubSamplingFactorSpinBox->setValue( _fiberSubSamplingFactor );

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

    connect( _fiberSubSamplingFactorSpinBox,
             SIGNAL( valueChanged( int ) ),
             this,
             SLOT( fiberSubSamplingFactorChanged( int ) ) );

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


    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( cancel() ) );
    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    _mainWindow->show();
 
  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "drawContents()" );

}


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
gkg::AInteractiveBundleMapRenderingOptionsWidget::fiberLineWidthChanged(
                                                                  double value )
{

  try
  {

    _fiberLineWidth = ( float )value;

  }
  GKG_CATCH( "void "
             "gkg::AInteractiveBundleMapRenderingOptionsWidget::"
             "fiberLineWidthChanged( float value )" );

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


void gkg::AInteractiveBundleMapRenderingOptionsWidget::cancel()
{

  try
  {


    _fiberSubSamplingFactor = _originalFiberSubSamplingFactor;
    _fiberLineWidth = _originalFiberLineWidth;
    _fiberColorEncoding = _originalFiberColorEncoding;
    _fileNameScalarFeatureVolume = _originalFileNameScalarFeatureVolume;
    _fileNameScalarFeatureVolumeToBundleTransform3d = 
                                _fileNameScalarFeatureVolumeToBundleTransform3d;
    _lowerBoundString = _lowerBoundString;
    _upperBoundString = _upperBoundString;

    updateObjects();
    _mainWindow->close();

  }
  GKG_CATCH( "void gkg::AInteractiveBundleMapRenderingOptionsWidget::cancel()" );

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
