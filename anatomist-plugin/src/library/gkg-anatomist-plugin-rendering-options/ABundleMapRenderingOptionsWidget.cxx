#include <gkg-anatomist-plugin-rendering-options/ABundleMapRenderingOptionsWidget.h>
#include <gkg-anatomist-plugin-objects/ABundleMapObject.h>
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

#include <QTreeWidget>
#include <QTreeWidgetItem>

gkg::ABundleMapRenderingOptionsWidget::ABundleMapRenderingOptionsWidget(
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

      gkg::ABundleMapObject* 
        aBundleMapObject = dynamic_cast< gkg::ABundleMapObject* >( *o );
      if ( aBundleMapObject )
      {

        _fiberSubSamplingFactor = aBundleMapObject->getFiberSubSamplingFactor();
        _fiberLineWidth = aBundleMapObject->getFiberLineWidth();
        _fiberColorEncoding = aBundleMapObject->getFiberColorEncoding();
        _fileNameScalarFeatureVolume =
                             aBundleMapObject->getFileNameScalarFeatureVolume();
        _fileNameScalarFeatureVolumeToBundleTransform3d =
          aBundleMapObject->getFileNameScalarFeatureVolumeToBundleTransform3d();
        _lowerBoundString = QString::number( 
                                            aBundleMapObject->getLowerBound() );
        _upperBoundString = QString::number( 
                                            aBundleMapObject->getUpperBound() );
        
        //Ajout test
        classificationLabel = aBundleMapObject->getClassificationLabel_states();
        rightLabels_states = aBundleMapObject->getRightLabels_states();
        leftLabels_states = aBundleMapObject->getLeftLabels_states();
        interLabels_states = aBundleMapObject->getInterLabels_states();
        everyLabels_states = aBundleMapObject->getEveryLabels_states();
        
        
        
        
        break;

      }

      aBundleMapObject->addObserver( ( anatomist::Observer*)this );
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
  GKG_CATCH( "gkg::ABundleMapRenderingOptionsWidget::"
             "ABundleMapRenderingOptionsWidget( "
             "const std::set< anatomist::AObject* >& objects, "
             "QWidget* parent, "
             "const char* name )" );

}


gkg::ABundleMapRenderingOptionsWidget::~ABundleMapRenderingOptionsWidget()
{

  runCommand();
  cleanupObserver();

}


void gkg::ABundleMapRenderingOptionsWidget::update(
                                                   const anatomist::Observable*,
                                                   void* /* arg */ )
{
}


void gkg::ABundleMapRenderingOptionsWidget::unregisterObservable(
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
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::"
             "unregisterObservable( "
             "anatomist::Observable* observable )" );

}


void gkg::ABundleMapRenderingOptionsWidget::updateObjects()
{

  try
  {

    std::set< anatomist::AObject * >::const_iterator
      o =  _objects.begin(),
      oe = _objects.end();
    while ( o != oe )
    {

      gkg::ABundleMapObject*
        aBundleMapObject = dynamic_cast< gkg::ABundleMapObject* >( *o );
      if ( aBundleMapObject )
      {

        aBundleMapObject->setFiberSubSamplingFactor( _fiberSubSamplingFactor );
        aBundleMapObject->setFiberLineWidth( _fiberLineWidth );
        aBundleMapObject->setFiberColorEncoding(
                                _fiberColorEncoding,
                                _fileNameScalarFeatureVolume,
                                _fileNameScalarFeatureVolumeToBundleTransform3d,
                                _lowerBoundString.toDouble(),
                                _upperBoundString.toDouble() );
        aBundleMapObject->notifyObservers( this );
        aBundleMapObject->clearHasChangedFlags();

        //Ajout test
        aBundleMapObject->setSplitLabels(rightLabels_states,leftLabels_states,
                                         interLabels_states ,everyLabels_states);
        classificationLabel = aBundleMapObject->getClassificationLabel_states();
      ++ o;

      }

    }

  }
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::updateObjects()" );

}


void gkg::ABundleMapRenderingOptionsWidget::drawContents()
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
                                              "BundleMapRenderingOptions.ui"  );

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
    _fiberLineWidthDoubleSpinBox->setRange( 1.0, 100.0 );
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

    QPushButton* updatePushButton = new QPushButton("Update");
    QPushButton* cancelPushButton = new QPushButton( "Cancel" );
    QPushButton* okPushButton = new QPushButton( "Ok" );
 
    validationLayout->addWidget( cancelPushButton, 0, 0, 1, 1 );
    validationLayout->addWidget( okPushButton, 0, 1, 1, 1 );
    validationLayout->addWidget(updatePushButton, 0, 2, 1, 1);

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


     // Ajouter un QTreeWidget à votre interface
    treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(1);  // une seule colonne pour l'arborescence

    // Créer des QTreeWidgetItems avec des checkboxes
    QTreeWidgetItem* rootItem = new QTreeWidgetItem(treeWidget,
                                                         QStringList("Brain"));


    if(classificationLabel == true)
    {
        // Créer des QCheckBox pour les éléments de rightLabels
      QTreeWidgetItem* rightItem = new QTreeWidgetItem(rootItem, QStringList());
      QCheckBox* rightCheckBox = new QCheckBox(treeWidget);
      rightCheckBox->setText("Right Hemisphere");
      rightCheckBox->setChecked(rightLabels_states["Right Hemisphere"]);
      rightItem->addChild(new QTreeWidgetItem(rightItem, QStringList(), 0));
      treeWidget->setItemWidget(rightItem, 0, rightCheckBox);
      rightCheckBoxes_states.push_back(rightCheckBox);
      QObject::connect(rightCheckBox, &QCheckBox::stateChanged, this
                      , [=](int state) {updateChildCheckboxes("right", state);});

    
      // Créer des QCheckBox pour les éléments de leftLabels
      QTreeWidgetItem* leftItem = new QTreeWidgetItem(rootItem, QStringList());
      QCheckBox* leftCheckBox = new QCheckBox(treeWidget);
      leftCheckBox->setText("Left Hemisphere");
      leftCheckBox->setChecked(leftLabels_states["Left Hemisphere"]);
      leftItem->addChild(new QTreeWidgetItem(leftItem, QStringList(), 0));
      treeWidget->setItemWidget(leftItem, 0, leftCheckBox);
      leftCheckBoxes_states.push_back(leftCheckBox);
      QObject::connect(leftCheckBox, &QCheckBox::stateChanged, this
                      , [=](int state) {updateChildCheckboxes("left", state);});

    
    
      // Créer des QCheckBox pour les éléments de interLabels
      QTreeWidgetItem* interItem = new QTreeWidgetItem(rootItem, QStringList());
      QCheckBox* interCheckBox = new QCheckBox(treeWidget);
      interCheckBox->setText("Inter Hemisphere");
      interCheckBox->setChecked(interLabels_states["Inter Hemisphere"]);
      interItem->addChild(new QTreeWidgetItem(interItem, QStringList(), 0));
      treeWidget->setItemWidget(interItem, 0, interCheckBox);
      interCheckBoxes_states.push_back(interCheckBox);
      QObject::connect(interCheckBox, &QCheckBox::stateChanged, this,
                        [=](int state) {updateChildCheckboxes("inter", state);}); 
    
    
      // Ajouter des checkboxes pour les éléments de rightLabels
      for (const auto& rightLabelPair : rightLabels_states) 
      {
          
          const std::string& rightLabel = rightLabelPair.first;
          // Vérifier si le texte contient "Hemisphere"
          if (rightLabel.find("Hemisphere") == std::string::npos)
          {

            QCheckBox* checkBox = new QCheckBox(treeWidget);
            checkBox->setText(QString::fromStdString(rightLabel));
            checkBox->setChecked(rightLabelPair.second); 
            rightItem->addChild(new QTreeWidgetItem(rightItem, QStringList(), 0));
            treeWidget->setItemWidget(rightItem->child(
                                      rightItem->childCount() - 1), 0, checkBox);

            // Stocker le pointeur de la checkbox dans la liste
            rightCheckBoxes_states.push_back(checkBox);


          // Connecter le signal stateChanged pour mettre à jour le parent
          QObject::connect(checkBox, &QCheckBox::stateChanged, this, [=] {
              updateParentCheckbox(rightItem);
          });

          }

      }
      // Ajouter des checkboxes pour les éléments de leftLabels
      for (const auto& leftLabelPair : leftLabels_states) 
      {
        
          const std::string& leftLabel = leftLabelPair.first;
              // Vérifier si le texte contient "Hemisphere"
          if (leftLabel.find("Hemisphere") == std::string::npos) 
          {

            QCheckBox* checkBox = new QCheckBox(treeWidget);
            checkBox->setText(QString::fromStdString(leftLabel));
            checkBox->setChecked(leftLabelPair.second);
            leftItem->addChild(new QTreeWidgetItem(leftItem, QStringList(), 0));
            treeWidget->setItemWidget(leftItem->child(
                                        leftItem->childCount() - 1), 0, checkBox);

            // Stocker le pointeur de la checkbox dans la liste
            leftCheckBoxes_states.push_back(checkBox);
              // Connecter le signal stateChanged pour mettre à jour le parent
            QObject::connect(checkBox, &QCheckBox::stateChanged, this, [=] {
                updateParentCheckbox(leftItem);
            });

          }

      }

      // Ajouter des checkboxes pour les éléments de interLabels
      for (const auto& interLabelPair : interLabels_states) 
      {
      
          const std::string& interLabel = interLabelPair.first;
              // Vérifier si le texte contient "Hemisphere"
          if (interLabel.find("Hemisphere") == std::string::npos) 
          {
      
            QCheckBox* checkBox = new QCheckBox(treeWidget);
            checkBox->setText(QString::fromStdString(interLabel));
            checkBox->setChecked(interLabelPair.second);
            interItem->addChild(new QTreeWidgetItem(interItem, QStringList(), 0));
            treeWidget->setItemWidget(interItem->child(
                                      interItem->childCount() - 1), 0, checkBox);

            // Stocker le pointeur de la checkbox dans la liste
            interCheckBoxes_states.push_back(checkBox);
              // Connecter le signal stateChanged pour mettre à jour le parent
            QObject::connect(checkBox, &QCheckBox::stateChanged, this, [=] {
                updateParentCheckbox(interItem);
            });
      
          }
      
      }
    }
    else
    {
        // Créer des QCheckBox pour les éléments de everyLabels
      QTreeWidgetItem* everyItem = new QTreeWidgetItem(rootItem, QStringList());
      QCheckBox* everyCheckBox = new QCheckBox(treeWidget);
      everyCheckBox->setText("All Bundle");
      everyCheckBox->setChecked(everyLabels_states["Every Bundle Label"]);
      everyItem->addChild(new QTreeWidgetItem(everyItem, QStringList(), 0));
      treeWidget->setItemWidget(everyItem, 0, everyCheckBox);
      everyCheckBoxes_states.push_back(everyCheckBox);
      QObject::connect(everyCheckBox, &QCheckBox::stateChanged, this,
                        [=](int state) {updateChildCheckboxes("every", state);}); 
    
    
      // Ajouter des checkboxes pour les éléments de every
      for (const auto& everyLabelPair : everyLabels_states) 
      {
      
          const std::string& everyLabel = everyLabelPair.first;
      
            QCheckBox* checkBox = new QCheckBox(treeWidget);
            if (everyLabel.find("Every") == std::string::npos)
          {

            checkBox->setText(QString::fromStdString(everyLabel));
            checkBox->setChecked(everyLabelPair.second);
            everyItem->addChild(new QTreeWidgetItem(everyItem, QStringList(), 0));
            treeWidget->setItemWidget(everyItem->child(
                                      everyItem->childCount() - 1), 0, checkBox);

            // Stocker le pointeur de la checkbox dans la liste
            everyCheckBoxes_states.push_back(checkBox);
              // Connecter le signal stateChanged pour mettre à jour le parent
            QObject::connect(checkBox, &QCheckBox::stateChanged, this, [=] {
                updateParentCheckbox(everyItem);
            });
          }
      }
      
   }
    
    // Ajouter l'arbre à votre mise en page existante
    mainLayout->addWidget(treeWidget, 0, 0, 1, 3);
    mainLayout->setAlignment(treeWidget, Qt::AlignBottom); // Aligner en bas

    connect(updatePushButton, SIGNAL( clicked() ),
             this, SLOT( update() ) );
    connect( cancelPushButton, SIGNAL( clicked() ),
             this, SLOT( cancel() ) );
    connect( okPushButton, SIGNAL( clicked() ),
             this, SLOT( accept() ) );

    _mainWindow->show();
 
  }

  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::drawContents()" );

}

void gkg::ABundleMapRenderingOptionsWidget::updateParentCheckbox(
                                                    QTreeWidgetItem* parentItem)
{

     QCheckBox* parentCheckBox = qobject_cast<QCheckBox*>(
                                        treeWidget->itemWidget(parentItem, 0));
    setChildModificationCheckbox(true);
    if (parentCheckBox)
    {

        // Vérifier si au moins un enfant est coché
        bool atLeastOneChildChecked = false;

        for (int i = 0; i < parentItem->childCount(); ++i)
        {

            QTreeWidgetItem* childItem = parentItem->child(i);
            QCheckBox* childCheckBox = qobject_cast<QCheckBox*>(
                                          treeWidget->itemWidget(childItem, 0));

            if (childCheckBox && childCheckBox->isChecked())
            {

                // Au moins un enfant est coché, cocher le parent
                atLeastOneChildChecked = true;
                break;

            }
        }

        // Si au moins un enfant est coché, cocher le parent
        // Sinon, décocher le parent
        parentCheckBox->setChecked(atLeastOneChildChecked);

        setChildModificationCheckbox(false);
    }
}

void gkg::ABundleMapRenderingOptionsWidget::updateChildCheckboxes(
                                          const QString& hemisphere, int state) 
{

    std::vector<QCheckBox*>* checkBoxes = nullptr;

    bool canUpdateChild = getChildModificationCheckbox();
    if(!canUpdateChild)
    {
      if(classificationLabel)
      {

        if (hemisphere == "right") 
        {

            checkBoxes = &rightCheckBoxes_states;
        
        } 
        else if (hemisphere == "left") 
        {
        
            checkBoxes = &leftCheckBoxes_states;
        
        } 
        else if (hemisphere == "inter")
        {
        
            checkBoxes = &interCheckBoxes_states;
        
        }
      }
      else
      {
        checkBoxes = &everyCheckBoxes_states;
      }
      if (checkBoxes) 
      {
      
          for (QCheckBox* checkBox : *checkBoxes) 
      
          {
      
              checkBox->setChecked(state == Qt::Checked);
      
          }
      
      }
      
    
    }

}

void gkg::ABundleMapRenderingOptionsWidget::printCheckBoxStates()
{

  // Affichage des résultats
    std::cout << "Right Labels:" << std::endl;
    for (const auto& pair : rightLabels_states) 
    {

        std::cout << pair.first << " (" << (pair.second ? "true" : "false") 
                                                            << ")" << std::endl;

    }

    std::cout << "\nLeft Labels:" << std::endl;
    for (const auto& pair : leftLabels_states) 
    {

        std::cout << pair.first << " (" << (pair.second ? "true" : "false") 
                                                            << ")" << std::endl;

    }

    std::cout << "\nInter Labels:" << std::endl;
    for (const auto& pair : interLabels_states) 
    {

        std::cout << pair.first << " (" << (pair.second ? "true" : "false") 
                                                            << ")" << std::endl;

    }

}

void gkg::ABundleMapRenderingOptionsWidget::saveCheckBoxStates() {
    // Mettre à jour les éléments de rightLabels
    for (QCheckBox* checkBox : rightCheckBoxes_states) 
    {
    
        const std::string labelText = checkBox->text().toStdString();
        rightLabels_states[labelText] = checkBox->isChecked();

    }

    // Mettre à jour les éléments de leftLabels
    for (QCheckBox* checkBox : leftCheckBoxes_states) 
    {
    
        const std::string labelText = checkBox->text().toStdString();
        leftLabels_states[labelText] = checkBox->isChecked();
    
    }

    // Mettre à jour les éléments de interLabels
    for (QCheckBox* checkBox : interCheckBoxes_states) 
    {
    
        const std::string labelText = checkBox->text().toStdString();
        interLabels_states[labelText] = checkBox->isChecked();
    
    }

    // Mettre à jour les éléments de interLabels
    for (QCheckBox* checkBox : everyCheckBoxes_states) 
    {
    
        const std::string labelText = checkBox->text().toStdString();
        everyLabels_states[labelText] = checkBox->isChecked();
    
    }

}

void gkg::ABundleMapRenderingOptionsWidget::setChildModificationCheckbox(
                                                                  bool newstate)
{

  child_modification_checkbox = newstate;

}
    
bool gkg::ABundleMapRenderingOptionsWidget::getChildModificationCheckbox()
{
 
  return child_modification_checkbox;

}

void 
gkg::ABundleMapRenderingOptionsWidget::fiberSubSamplingFactorChanged(
                                                                     int value )
{

  try
  {

    _fiberSubSamplingFactor = value;

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "fiberSubSamplingFactorChanged( int value )" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::fiberLineWidthChanged( double value )
{

  try
  {

    _fiberLineWidth = ( float )value;

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "fiberLineWidthChanged( float value )" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::fiberColorEncodingChanged( int value )
{

  try
  {

    _fiberColorEncoding = value;

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "fiberColorEncodingChanged( int value )" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::fileNameScalarFeatureVolumeChanged(
                                                          const QString& value )
{

  try
  {

    _fileNameScalarFeatureVolume = value.toUtf8().constData();

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "fileNameScalarFeatureVolumeChanged( "
             "const QString& value )" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::selectFileNameScalarFeatureVolume()
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
             "gkg::ABundleMapRenderingOptionsWidget::"
             "selectFileNameScalarFeatureVolume()" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::
                         fileNameScalarFeatureVolumeToBundleTransform3dChanged(
                                                          const QString& value )
{

  try
  {

    _fileNameScalarFeatureVolumeToBundleTransform3d =
                                                     value.toUtf8().constData();

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "fileNameScalarFeatureVolumeToBundleTransform3dChanged( "
             "const QString& value )" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::
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
             "gkg::ABundleMapRenderingOptionsWidget::"
             "selectFileNameScalarFeatureVolumeToBundleTransform3d()" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::lowerBoundChanged( const QString& value )
{

  try
  {

    _lowerBoundString = value;

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "lowerBoundChanged( "
             "const QString& value )" );

}


void 
gkg::ABundleMapRenderingOptionsWidget::upperBoundChanged( const QString& value )
{

  try
  {

    _upperBoundString = value;

  }
  GKG_CATCH( "void "
             "gkg::ABundleMapRenderingOptionsWidget::"
             "upperBoundChanged( "
             "const QString& value )" );

}


void gkg::ABundleMapRenderingOptionsWidget::cancel()
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
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::cancel()" );

}


void gkg::ABundleMapRenderingOptionsWidget::accept()
{

  try
  {

    _mainWindow->close();
    saveCheckBoxStates();
    updateObjects();
  //  printCheckBoxStates();

  }
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::accept()" );


}
void gkg::ABundleMapRenderingOptionsWidget::update()
{

  try
  {
    saveCheckBoxStates();
    updateObjects();

 //   printCheckBoxStates();

  }
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::update()" );

}


void gkg::ABundleMapRenderingOptionsWidget::chooseObject()
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
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::chooseObject()" );

}


void gkg::ABundleMapRenderingOptionsWidget::objectsChosen(
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
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::objectsChosen( "
             "const std::set< anatomist::AObject* >& objects )" );

}


void gkg::ABundleMapRenderingOptionsWidget::runCommand()
{
}


void gkg::ABundleMapRenderingOptionsWidget::create( 
                                const std::set< anatomist::AObject* >& objects )
{

  try
  {

    //gkg::ABundleMapRenderingOptionsWidget* aOdfFieldRenderingOptionsWidget =
    new gkg::ABundleMapRenderingOptionsWidget( objects );

  }
  GKG_CATCH( "void gkg::ABundleMapRenderingOptionsWidget::create( "
             "const std::set< anatomist::AObject* >& objects )" );

}
