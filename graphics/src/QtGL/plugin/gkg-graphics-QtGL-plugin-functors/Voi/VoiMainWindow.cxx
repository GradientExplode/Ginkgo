#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiMainWindow.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SharedGLWidget.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/DrawA.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Strategy.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ModeFactory.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Dialog.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiStatistic.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Workspace.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiLoadWindow.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QShortcut>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include <QButtonGroup>
#include <QGLFormat>


gkg::VoiMainWindow::VoiMainWindow()
                   : QWidget( 0, Qt::FramelessWindowHint )
{

  setGeometry( gkg::Workspace::getInstance().getAvailableGeometry() );
  setFont( QFont( "helvetica", 10 ) );

  createWorkArea();

}


gkg::VoiMainWindow::~VoiMainWindow()
{
}


void gkg::VoiMainWindow::initializeArg( const std::string& anatName,
                                        const std::string& fusionName,
                                        const std::string& labelName,
                                        const std::string& transfoName )
{

  if ( !anatName.empty() || !labelName.empty() )
  {
  
    data.initialize( anatName, fusionName, transfoName, labelName );

  }

}


void gkg::VoiMainWindow::createWorkArea()
{

  QHBoxLayout* hbox = new QHBoxLayout();
  hbox->setContentsMargins( 0, 0, 0, 0 );
  hbox->setSpacing( 0 );

  QVBoxLayout* vbox = new QVBoxLayout();
  vbox->setSpacing( 10 );

  QLabel* title = new QLabel( "GkgVoi" );
  title->setFont( QFont( "Flubber", 20, 1, true ) );
  title->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
  title->setMargin( 5 );

  vbox->addWidget( title );

  // Control buttons
  QGridLayout *gridBut = new QGridLayout();
  gridBut->setContentsMargins( 10, 0, 10, 0 ); // left and right
  gridBut->setSpacing( 5 );

  QButtonGroup* bgroup = new QButtonGroup();
  bgroup->setExclusive( true );

  QSize iconSize = gkg::Workspace::getInstance().getIconSize();
  QPixmap openIcon( ":voi/icon/open_file.png" );
  QPushButton* b1 = new QPushButton( QIcon( openIcon ), "" );
  b1->setIconSize( iconSize );
  b1->setToolTip( "Load volumes" );
  connect( b1, SIGNAL( clicked() ), this, SLOT( loadVolumes() ) );
  QPixmap saveIcon( ":voi/icon/save_file.png" );
  QPushButton* b3 = new QPushButton( QIcon( saveIcon ), "" );
  b3->setIconSize( iconSize );
  b3->setToolTip( "Save label" );
  connect( b3, SIGNAL( clicked() ), this, SLOT( saveLabel() ) );
  QPixmap selectionIcon( ":voi/icon/arrow.png" );
  QPushButton* b6 = new QPushButton( QIcon( selectionIcon ), "" );
  b6->setIconSize( iconSize );
  b6->setToolTip( "Left Click : Linked cursor\n"
                  "Shift + Left Click : Pan\n"
                  "CTRL + Left Click : Zoom\n"
                  "Middle Click : Contrast for T1 image\n"
                  "Shift + Middle Click : Contrast for the added image\n"
                  "Right Click : Undo" );
  b6->setCheckable( true );
  b6->setChecked( true );

  QPixmap brushIcon( ":voi/icon/brush.png" );
  QPushButton* b7 = new QPushButton( QIcon( brushIcon ), "" );
  b7->setIconSize( iconSize );
  b7->setToolTip( "Left Click : Paint or Pick color\n"
                  "Right Click : Erase\n"
                  "CTRL + Left Click : Fill\n"
                  "CTRL + Right Click : Undo last fill\n"
                  "CTRL + C : copy ROI\n"
                  "CTRL + V : paste ROI" );
  b7->setCheckable( true );
  QPixmap evolIcon( ":voi/icon/magic_wand.png" );
  QPushButton* b9 = new QPushButton( QIcon( evolIcon ), "" );
  b9->setIconSize( iconSize );
  b9->setToolTip( "Left Click : Region growing\n"
                  "Right Click : Undo last growth\n"
                  "CTRL + C : copy ROI\n"
                  "CTRL + V : paste ROI" );
  b9->setCheckable( true );
  QPixmap fillIcon( ":voi/icon/fill_color.png" );
  QPushButton* b11 = new QPushButton( QIcon( fillIcon ), "" );
  b11->setIconSize( iconSize );
  b11->setToolTip( "Left Click : Fill\n"
                   "Right Click : Undo last fill\n"
                   "CTRL + C : copy ROI\n"
                   "CTRL + V : paste ROI" );
  b11->setCheckable( true );

  QPixmap pickerIcon( ":voi/icon/color_picker.png" );
  QPushButton* b14 = new QPushButton( QIcon( pickerIcon ), "" );
  b14->setIconSize( iconSize );
  b14->setToolTip( "Pick color" );
  b14->setCheckable( true );
  QPixmap statIcon( ":voi/icon/statistic.png" );
  QPushButton* b15 = new QPushButton( QIcon( statIcon ), "" );
  b15->setIconSize( iconSize );
  b15->setToolTip( "Statistics" );
  b15->setCheckable( true );
  QPixmap saveStatIcon( ":voi/icon/save_stat.png" );
  QPushButton* b16 = new QPushButton( QIcon( saveStatIcon ), "" );
  b16->setIconSize( iconSize );
  b16->setToolTip( "Save statistics" );
  connect( b16, SIGNAL( clicked() ), this, SLOT( saveStatistics() ) );
  QPixmap clearLabelIcon( ":voi/icon/clear_label.png" );
  QPushButton* b17 = new QPushButton( QIcon( clearLabelIcon ), "" );
  b17->setIconSize( iconSize );
  b17->setToolTip( "Erase selected label" );
  connect( b17, SIGNAL( clicked() ), this, SLOT( clearSelection() ) );
  QPixmap clearAllIcon( ":voi/icon/clear_all.png" );
  QPushButton* b18 = new QPushButton( QIcon( clearAllIcon ), "" );
  b18->setIconSize( iconSize );
  b18->setToolTip( "Clear all labels" );
  connect( b18, SIGNAL( clicked() ), this, SLOT( clearAll() ) );

  QPixmap splineIcon( ":voi/icon/spline.png" );
  QPushButton* b19 = new QPushButton( QIcon( splineIcon ), "" );
  b19->setIconSize( iconSize );
  b19->setToolTip( "Spline contour\n"
                   "* Step 1 - creation\n"
                   "    CTRL + V : past last spline\n"
                   "    ESC : remove last point\n"
                   "    Left click : add point\n"
                   "    Shift + Left click : move point\n"
                   "    Right click : undo last spline\n"
                   "    Mid click : close the spline\n"
                   "* Step 2 - modification\n" 
                   "    Left click : insert point\n"
                   "    Shift + Left click : move point\n"
                   "    Right click : remove point\n"
                   "    Mid click : validate the spline" );
  b19->setCheckable( true );
  QPixmap morphoIcon( ":voi/icon/distancemap.png" );
  QPushButton* b20 = new QPushButton( QIcon( morphoIcon ), "" );
  b20->setIconSize( iconSize );
  b20->setToolTip( "Mathematical morphology" );
  b20->setCheckable( true );

  QPixmap cylinderIcon( ":voi/icon/cylinder.png" );
  QPushButton* b21 = new QPushButton( QIcon( cylinderIcon ), "" );
  b21->setIconSize( iconSize );
  b21->setToolTip( "Cylinder mask" );
  b21->setCheckable( true );

  QPixmap contourIcon( ":voi/icon/contour.png" );
  b22 = new QPushButton( QIcon( contourIcon ), "" );
  b22->setIconSize( iconSize );
  b22->setToolTip( "Switch between surface \n"
                   "and contour display" );
  connect( b22, SIGNAL( clicked() ), this, SLOT( toggleContour() ) );

  QPixmap rulerIcon( ":voi/icon/ruler.png" );
  QPushButton* b2 = new QPushButton( QIcon( rulerIcon ), "" );
  b2->setIconSize( iconSize );
  b2->setToolTip( "Distance measurement" );
  b2->setCheckable( true );

  gridBut->addWidget( b1, 0, 0 );
  gridBut->addWidget( b3, 0, 1 );
  gridBut->addWidget( b16, 0, 2 );
  gridBut->addWidget( b17, 0, 3 );
  gridBut->addWidget( b18, 0, 4 );

  gridBut->addWidget( b6, 1, 0 );
  gridBut->addWidget( b15, 1, 1 );
  gridBut->addWidget( b14, 1, 2 );
  gridBut->addWidget( b2, 1, 3 ); // WIP
  gridBut->addWidget( b22, 1, 4 );

  gridBut->addWidget( b7, 2, 0 );
  gridBut->addWidget( b9, 2, 1 );
  gridBut->addWidget( b11, 2, 2 );
  gridBut->addWidget( b19, 2, 3 );
  gridBut->addWidget( b20, 2, 4 );

  //gridBut->addWidget( b21, 3, 0 ); // WIP

  vbox->addLayout( gridBut );

  bgroup->addButton( b6, int( gkg::VoiModeStrategy::SelectionMode ) );
  bgroup->addButton( b7, int( gkg::VoiModeStrategy::PaintMode ) );
  bgroup->addButton( b9, int( gkg::VoiModeStrategy::EvolutionMode ) );
  bgroup->addButton( b11, int( gkg::VoiModeStrategy::FillingMode ) );
  bgroup->addButton( b14, int( gkg::VoiModeStrategy::PickingColorMode ) );
  bgroup->addButton( b15, int( gkg::VoiModeStrategy::StatisticMode ) );
  bgroup->addButton( b19, int( gkg::VoiModeStrategy::SplineMode ) );
  bgroup->addButton( b20, int( gkg::VoiModeStrategy::MorphologyMode ) );
  bgroup->addButton( b2, int( gkg::VoiModeStrategy::DistanceMode ) );
  //bgroup->addButton( b21, int( gkg::VoiModeStrategy::CylinderMode ) ); // WIP

  // Feature parameters
  gkg::VoiDialog* control = new gkg::VoiDialog( &data );
  control->setFixedWidth( 
                     gkg::Workspace::getInstance().getPanelGeometry().width() );
  control->setContentsMargins( 10, 0, 10, 5 ); // left and right
  connect( bgroup, SIGNAL( buttonClicked( int ) ), 
           control, SLOT( show( int ) ) );
  connect( bgroup, SIGNAL( buttonClicked( int ) ), 
           this, SLOT( setModeStrategy( int ) ) );
  vbox->addWidget( control );

  // Exit button
  QPixmap exitIcon( ":voi/icon/exit.png" );
  QPushButton* button = new QPushButton( QIcon( exitIcon ), "" );
  button->setIconSize( gkg::Workspace::getInstance().getIconSize() );
  button->setToolTip( "Exit" );
  connect( button, SIGNAL( clicked() ), qApp, SLOT( quit() ) );
  vbox->addWidget( button );

  hbox->addLayout( vbox );

  QWidget* main = new QWidget();
  main->setGeometry( gkg::Workspace::getInstance().getViewerGeometry() );

  QGridLayout *grid = new QGridLayout();
  grid->setContentsMargins( 0, 0, 0, 0 );
  grid->setSpacing( 0 );

  // Shared GL widget
  QGLFormat glFormat( QGL::DoubleBuffer | QGL::Rgba |
                      QGL::DepthBuffer | QGL::SampleBuffers );
  SharedGLWidget* sharedWidget = new SharedGLWidget( glFormat, main );

  // Coronal drawing area
  gkg::VoiDrawA *daC = new gkg::VoiDrawA( gkg::VoiStrategy::StgyCoronal, 
                                          &data, glFormat, sharedWidget, main );
  daC->setGeometry( gkg::Workspace::getInstance().getHalfViewerGeometry() );
  QShortcut *cDown = new QShortcut( QKeySequence( Qt::Key_Down ), daC );
  QShortcut *cUp = new QShortcut( QKeySequence( Qt::Key_Up ), daC );
  QShortcut *cPlus = new QShortcut( QKeySequence( Qt::Key_Plus ), daC );
  QShortcut *cMinus = new QShortcut( QKeySequence( Qt::Key_Minus ), daC );
  connect( cDown, SIGNAL( activated() ), daC, SLOT( increase() ) );
  connect( cUp, SIGNAL( activated() ), daC, SLOT( decrease() ) );
  connect( cPlus, SIGNAL( activated() ), daC, SLOT( incFrame() ) );
  connect( cMinus, SIGNAL( activated() ), daC, SLOT( decFrame() ) );

  // Sagittal drawing area
  gkg::VoiDrawA *daS = new gkg::VoiDrawA( gkg::VoiStrategy::StgySagittal, 
                                          &data, glFormat, sharedWidget, main );
  daS->setGeometry( gkg::Workspace::getInstance().getHalfViewerGeometry() );
  QShortcut *sRight = new QShortcut( QKeySequence( Qt::Key_Right ), daS );
  QShortcut *sLeft = new QShortcut( QKeySequence( Qt::Key_Left ), daS );
  connect( sRight, SIGNAL( activated() ), daS, SLOT( increase() ) );
  connect( sLeft, SIGNAL( activated() ), daS, SLOT( decrease() ) );

  // Axial drawing area
  gkg::VoiDrawA *daA = new gkg::VoiDrawA( gkg::VoiStrategy::StgyAxial, 
                                          &data, glFormat, sharedWidget, main );
  daA->setGeometry( gkg::Workspace::getInstance().getHalfViewerGeometry() );
  QShortcut *aPageDown = new QShortcut( QKeySequence( Qt::Key_PageDown ), daA );
  QShortcut *aPageUp = new QShortcut( QKeySequence( Qt::Key_PageUp ), daA );
  connect( aPageDown, SIGNAL( activated() ), daA, SLOT( increase() ) );
  connect( aPageUp, SIGNAL( activated() ), daA, SLOT( decrease() ) );

  // 3D drawing area
  gkg::VoiDrawA *da3D = new gkg::VoiDrawA( gkg::VoiStrategy::Stgy3D, 
                                           &data, glFormat, sharedWidget,
                                           main );
  da3D->setGeometry( gkg::Workspace::getInstance().getHalfViewerGeometry() );

  grid->addWidget( daC, 0, 0 ); 
  grid->addWidget( daS, 0, 1 ); 
  grid->addWidget( daA, 1, 0 ); 
  grid->addWidget( da3D, 1, 1 );

  main->setLayout( grid );
  hbox->addWidget( main );

  setLayout( hbox );
  
}



void gkg::VoiMainWindow::loadVolumes()
{

  gkg::VoiLoadWindow loadWindow( data, this );

  loadWindow.exec();

}


void gkg::VoiMainWindow::saveLabel()
{

  QString sfilt;

  QString filt = "16 bits GIS files (*.ima);;8 bits GIS files (*.ima);;";
  filt += "All files (*)";

  QString fname = QFileDialog::getSaveFileName( this, "Save label",
                                                QString(), filt, &sfilt );
  if ( !fname.isEmpty() ) 
  {

    data.saveLabel( fname.toStdString(), sfilt );

  }

}


void gkg::VoiMainWindow::saveStatistics()
{

  if ( data.dataLoaded() )
  {

    QString filt = "ROI Statistic (*.stat)";

    QString fname = QFileDialog::getSaveFileName( this, "Save statistics",
                                                  QString(), filt );
    if ( !fname.isEmpty() ) 
    {

      // sanity checks to check if we can write
      gkg::File file( fname.toStdString() );

      gkg::File baseDirectory( file.getDirectoryName() +
                               gkg::getDirectorySeparator() + "." );

      if ( !baseDirectory.isWritable() )
      {

        std::cout << "cannot write in '"
                  << file.getDirectoryName()
                  << "' directory"
                  << std::endl;
        return;

      }

      gkg::VoiStatistic stat;
      stat.compute( data );
      stat.save( fname.toStdString() );

    }

  }

}


void gkg::VoiMainWindow::toggleContour()
{

  b22->setIcon( QIcon( data.isContour() ?
                QPixmap( ":voi/icon/contour.png" ) :
                QPixmap( ":voi/icon/region.png" ) ) );
  data.toggleContour();

}


void gkg::VoiMainWindow::clearSelection()
{

  if ( data.dataLoaded() )
  {

    VoiLabelInfo& labelInfo = data.labelInfo();
    std::string name = labelInfo.getName( labelInfo.currentLabel() );
    std::string msg = "Do you really want to clear " + name + " ?";
    QMessageBox::StandardButton ret = QMessageBox::question(
                                             this,
                                             tr( "GkgVoi" ), 
                                             tr( msg.c_str() ),
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No );

    if ( ret == QMessageBox::Yes )
    {

      data.clearSelectedLabel();

    }

  }

}


void gkg::VoiMainWindow::clearAll()
{

  if ( data.dataLoaded() )
  {

    QMessageBox::StandardButton ret = QMessageBox::question(
                                 this,
                                 tr( "GkgVoi" ), 
                                 tr( "Do you really want to clear all ROIs ?" ),
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No );

    if ( ret == QMessageBox::Yes )
    {

      data.clearAllLabels();

    }

  }

}


void gkg::VoiMainWindow::setModeStrategy( int id )
{

  if ( !data.empty() )
  {

    gkg::VoiModeFactory factory;

    data.notifyMode( factory.create( (gkg::VoiModeStrategy::ModeId)id ) );

  }

}
