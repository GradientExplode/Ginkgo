#include <gkg-graphics-QtGL-plugin-functors/DicomDump/DicomDumpMainWindow.h>
#include <gkg-core-io/DicomDump.h>


gkg::DicomDumpMainWindow::DicomDumpMainWindow()
                        : QMainWindow()
{

  setWindowTitle( QString( tr( "Gkg Dicom Dump" ) ) );
  createMenu();

  gkg::DicomTagInfo tagInfo;
  QStringList header;

  header << QString( tagInfo.tag.c_str() )
         << QString( tagInfo.vr.c_str() )
         << QString( tagInfo.vm.c_str() )
         << QString( tagInfo.size.c_str() )
         << QString( tagInfo.name.c_str() )
         << QString( tagInfo.value.c_str() );

  _treeWidget = new QTreeWidget( this );
  _treeWidget->setHeaderLabels( header );

  setCentralWidget( _treeWidget );

}


void gkg::DicomDumpMainWindow::open()
{

  QString fileName = QFileDialog::getOpenFileName( this );

  if ( !fileName.isEmpty() )
  {

    loadFile( fileName.toStdString() );

  }

}


void gkg::DicomDumpMainWindow::createMenu()
{

  // Actions
  QAction* openAction = new QAction( tr( "&Open..." ), this );
  connect( openAction, SIGNAL( triggered() ), this, SLOT( open() ) );

  QAction* exitAction = new QAction( tr( "E&xit" ), this );
  connect( exitAction, SIGNAL( triggered() ), this, SLOT( close() ) );

  // Menu
  _fileMenu = menuBar()->addMenu( tr( "&File" ) );
  _fileMenu->addAction( openAction );
  _fileMenu->addSeparator();
  _fileMenu->addAction( exitAction );

}


void gkg::DicomDumpMainWindow::loadFile( const std::string& fileName )
{

  gkg::DicomDump dicomDump( fileName );

  createTree( dicomDump.getDicomTagInfo() );

}


void gkg::DicomDumpMainWindow::createTree( gkg::DicomTagInfo& tagInfo )
{

  QList< QTreeWidgetItem* > items;
  QStringList header;

  header << QString( tagInfo.tag.c_str() )
         << QString( tagInfo.vr.c_str() )
         << QString( tagInfo.vm.c_str() )
         << QString( tagInfo.size.c_str() )
         << QString( tagInfo.name.c_str() )
         << QString( tagInfo.value.c_str() );

  _treeWidget->setColumnCount( 6 );
  _treeWidget->setHeaderLabels( header );

  if ( !tagInfo.tagList.empty() )
  {

    std::list< gkg::DicomTagInfo >::iterator
      t = tagInfo.tagList.begin(),
      te = tagInfo.tagList.end();

    while ( t != te )
    {

      addItem( items, (QTreeWidgetItem*)0, *t );
      ++t;

    }

  }

  _treeWidget->clear();
  _treeWidget->addTopLevelItems( items );
  _treeWidget->expandAll();

  int n = _treeWidget->columnCount();
  int width = 0;

  while ( n-- )
  {

    _treeWidget->resizeColumnToContents( n );
    width += _treeWidget->columnWidth( n  );

  }

  setGeometry( 40, 40, width + 40, 700 );

}


void gkg::DicomDumpMainWindow::addItem( QList< QTreeWidgetItem* >& items, 
                                        QTreeWidgetItem* parent, 
                                        gkg::DicomTagInfo& tagInfo )
{

  QStringList text;
  QTreeWidgetItem* widgetItem;

  text << QString( tagInfo.tag.c_str() )
       << QString( tagInfo.vr.c_str() )
       << QString( tagInfo.vm.c_str() )
       << QString( tagInfo.size.c_str() )
       << QString( tagInfo.name.c_str() )
       << QString( tagInfo.value.c_str() );
  widgetItem = new QTreeWidgetItem( parent, text );

  if ( !tagInfo.tagList.empty() )
  {

    std::list< gkg::DicomTagInfo >::iterator
      t = tagInfo.tagList.begin(),
      te = tagInfo.tagList.end();

    while ( t != te )
    {

      addItem( items, widgetItem, *t );
      ++t;

    }

  }

  if ( !parent )
  {

    items.append( widgetItem );

  }

}
