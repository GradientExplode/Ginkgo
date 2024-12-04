#include <gkg-graphics-QtGL-plugin-functors/Voi/VoiLoadWindow.h>
#include <gkg-communication-sysinfo/File.h>

#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>


gkg::VoiLoadWindow::VoiLoadWindow( gkg::VoiData& data, QWidget* parent )
                  : QDialog( parent ),
                    _data( data )
{

  QFormLayout* form = new QFormLayout( this );
  QLabel* labelT1 = new QLabel( "T1" );
  QLabel* labelVolB = new QLabel( "Volume B" );
  QLabel* labelTransfo = new QLabel( "Volume B => T1 3D Transf." );
  QLabel* labelLabel = new QLabel( "Label" );

  pathT1 = new QLineEdit( this );
  pathVolB = new QLineEdit( this );
  pathTransfo = new QLineEdit( this );
  pathLabel = new QLineEdit( this );

  QPushButton* browseT1 = new QPushButton( "Browse", this );
  QPushButton* browseVolB = new QPushButton( "Browse", this );
  QPushButton* browseTransfo = new QPushButton( "Browse", this );
  QPushButton* browseLabel = new QPushButton( "Browse", this );

  QHBoxLayout* layoutT1 = new QHBoxLayout;
  layoutT1->addWidget( pathT1 );
  layoutT1->addWidget( browseT1 );

  QHBoxLayout* layoutVolB = new QHBoxLayout;
  layoutVolB->addWidget( pathVolB );
  layoutVolB->addWidget( browseVolB );

  QHBoxLayout* layoutTransfo = new QHBoxLayout;
  layoutTransfo->addWidget( pathTransfo );
  layoutTransfo->addWidget( browseTransfo );

  QHBoxLayout* layoutLabel = new QHBoxLayout;
  layoutLabel->addWidget( pathLabel );
  layoutLabel->addWidget( browseLabel );

  form->addRow( labelT1, layoutT1 );
  form->addRow( labelVolB, layoutVolB );
  form->addRow( labelTransfo, layoutTransfo );
  form->addRow( labelLabel, layoutLabel );

  connect( browseT1, SIGNAL( clicked() ), this, SLOT( loadVolT1() ) );
  connect( browseVolB, SIGNAL( clicked() ), this, SLOT( loadVolB() ) );
  connect( browseTransfo, SIGNAL( clicked() ), this, SLOT( loadTransfo() ) );
  connect( browseLabel, SIGNAL( clicked() ), this, SLOT( loadLabel() ) );

  QDialogButtonBox* buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok |
                                                      QDialogButtonBox::Cancel | 
                                                      QDialogButtonBox::Reset );
  buttonBox->button( QDialogButtonBox::Reset )->setText( "Clear" );

  connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) ); 
  connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) ); 
  connect( buttonBox->button( QDialogButtonBox::Reset ), SIGNAL( clicked() ),
           this, SLOT( clear() ) );

  form->addRow( buttonBox );
  this->close();

}


gkg::VoiLoadWindow::~VoiLoadWindow()
{
}


void gkg::VoiLoadWindow::loadVolT1()
{

  QString filt = "Volume files (*.vimg *.img *.ima);;Vida files (*.vimg);;";
  filt += "Gis files (*.ima);;Analyze files (*.img);;DICOM (*);;All files (*)";

  QString fVolT1Name = QFileDialog::getOpenFileName( this, "Open one volume",
                                                     QString(), filt );

  if ( !fVolT1Name.isEmpty() )  
  {

    pathT1->setText( fVolT1Name );

  }

}


void gkg::VoiLoadWindow::loadVolB()
{

  QString filt = "Volume files (*.vimg *.img *.ima);;Vida files (*.vimg);;";
  filt += "Gis files (*.ima);;Analyze files (*.img);;DICOM (*);;All files (*)";

  QString fVolBName = QFileDialog::getOpenFileName( this, "Open one volume",
                                                    QString(), filt );
  if ( !fVolBName.isEmpty() )  
  {

    pathVolB->setText( fVolBName );

  }

}


void gkg::VoiLoadWindow::loadTransfo()
{

  QString filt = "Volume files (*.trm)";
  QString fTransfoName = QFileDialog::getOpenFileName(
                                                   this, 
                                                   "Open one 3D Transformation",
                                                   QString(),
                                                   filt );

  if ( !fTransfoName.isEmpty() )  
  {

    pathTransfo->setText( fTransfoName );

  }

}


void gkg::VoiLoadWindow::loadLabel()
{

  QString filt = "Volume files (*.vimg *.img *.ima);;Vida files (*.vimg);;";
  filt += "Gis files (*.ima);;Analyze files (*.img);;DICOM (*);;All files (*)";

  QString fVolLabelName = QFileDialog::getOpenFileName( this, "Open one volume",
                                                        QString(), filt );
  if ( !fVolLabelName.isEmpty() )  
  {

    pathLabel->setText( fVolLabelName );

  }

}


void gkg::VoiLoadWindow::clear()
{

  pathT1->clear();
  pathVolB->clear();
  pathTransfo->clear();
  pathLabel->clear();

}

void gkg::VoiLoadWindow::accept()
{

  if ( pathT1->text().isEmpty() && pathLabel->text().isEmpty() )  
  {

    QMessageBox::information( this, "Information", 
                              "Please set a T1 or a Label image" );

  }
  else  
  {

    _data.initialize( pathT1->text().toStdString(),
                      pathVolB->text().toStdString(),
                      pathTransfo->text().toStdString(),
                      pathLabel->text().toStdString() );

    QDialog::accept();

  }

}
