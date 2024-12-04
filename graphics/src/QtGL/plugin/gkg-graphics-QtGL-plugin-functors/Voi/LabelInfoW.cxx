#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfoW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/LabelInfo.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/CheckedHeader.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/ShowWidget.h>

#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QColorDialog>
#include <QIcon>
#include <QPixmap>
#include <QApplication>


gkg::VoiLabelInfoW::VoiLabelInfoW( gkg::VoiObservable *d, QWidget *p ) 
                   : QWidget( p ),
                     currentItem( 0 )
{

  data = (gkg::VoiData *)d;

  QHBoxLayout *lay1 = new QHBoxLayout();
  lay1->setContentsMargins( 0, 0, 0, 0 );
  lay1->setSpacing( 5 );

  QGroupBox* groupBox = new QGroupBox( "Labels" );

  QVBoxLayout *lay2 = new QVBoxLayout();

  lview = new QTableWidget( this );
  lview->setColumnCount( 6 );
  lview->setSelectionBehavior( QAbstractItemView::SelectRows );
  lview->setSelectionMode( QAbstractItemView::SingleSelection );
  lview->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
  lview->verticalHeader()->setVisible( false );
  lview->setSortingEnabled( false );

  QStringList header;
  header << "Show"
         << "Label"
         << "Name"
         << "R"
         << "G"
         << "B";
  lview->setHorizontalHeaderLabels( header );

  int32_t count = 0;
  std::map< int32_t, gkg::LabelInformation >::iterator 
    i = data->labelInfo().info().begin(),
    ie = data->labelInfo().info().end();

  while ( i != ie )
  {

    lview->insertRow( count );
    addRow( count, i->first, i->second );
    count++;
    ++i;
  
  }

  updateRows();

  CheckedHeader* headerView = new CheckedHeader( Qt::Horizontal, lview );
  lview->setHorizontalHeader( headerView );
  lview->setColumnWidth( 0, 92 );
  lview->setColumnWidth( 1, 64 );
  lview->setColumnWidth( 3, 60 );
  lview->setColumnWidth( 4, 60 );
  lview->setColumnWidth( 5, 60 );
#if QT_VERSION >= 0x050000
  lview->horizontalHeader()->setSectionResizeMode( 2, QHeaderView::Stretch );
#else
  lview->horizontalHeader()->setResizeMode( 2, QHeaderView::Stretch );
#endif
  connect( lview, SIGNAL( clicked( const QModelIndex& ) ), 
	   this, SLOT( selectionLabel( const QModelIndex& ) ) );
  connect( lview, SIGNAL( cellChanged( int, int ) ),
           this, SLOT( labelChanged( int, int ) ) );
  connect( headerView, SIGNAL( toggled( bool ) ),
           this, SLOT( headerChecked( bool ) ) );

  QHBoxLayout *param = new QHBoxLayout();

  QPushButton* btnShow = new QPushButton( QIcon( QPixmap( ":voi/icon/eye_opened.png" ) ), ""  );
  btnShow->setIconSize( QSize( 24, 24 ) );
  btnShow->setFixedSize( 40, 30 );
  connect( btnShow, SIGNAL( clicked() ), this, SLOT( showSelection() ) );

  QPushButton* btnHide = new QPushButton( QIcon( QPixmap( ":voi/icon/eye_closed.png" ) ), ""  );
  btnHide->setIconSize( QSize( 24, 24 ) );
  btnHide->setFixedSize( 40, 30 );
  connect( btnHide, SIGNAL( clicked() ), this, SLOT( hideSelection() ) );

  QPushButton *addL = new QPushButton( "+" );
  addL->setFixedSize( 40, 30 );
  addL->setFont( QFont( "helvetica", 20 ) );
  connect( addL, SIGNAL( clicked() ), this, SLOT( addLabel() ) );

  QPushButton *removeL = new QPushButton( "-" );
  removeL->setFixedSize( 40, 30 );
  removeL->setFont( QFont( "helvetica", 20 ) );
  connect( removeL, SIGNAL( clicked() ), this, SLOT( removeLabel() ) );

  QPushButton *chooseCol = new QPushButton( QIcon( QPixmap( ":voi/icon/color_wheel.png" ) ),
                                            "" );
  chooseCol->setFixedSize( 60, 30 );
  connect( chooseCol, SIGNAL( clicked() ), this, SLOT( chooseColor() ) );

  QPushButton *loadL = new QPushButton( "Load" );
  loadL->setFixedSize( 60, 30 );
  connect( loadL, SIGNAL( clicked() ), this, SLOT( loadLabel() ) );

  QPushButton *saveL = new QPushButton( "Save" );
  saveL->setFixedSize( 60, 30 );
  connect( saveL, SIGNAL( clicked() ), this, SLOT( saveLabel() ) );

  param->addWidget( btnShow );
  param->addWidget( btnHide );
  param->addWidget( addL );
  param->addWidget( removeL );
  param->addWidget( chooseCol );
  param->addWidget( loadL );
  param->addWidget( saveL );
  lay2->addWidget( lview );
  lay2->addLayout( param );

  groupBox->setLayout( lay2 );
  lay1->addWidget( groupBox );

  connect( &data->labelInfo(), SIGNAL( currentChanged( int ) ), 
  	   this, SLOT( setSelection( int ) ) );

  connect( &data->labelInfo(), SIGNAL( listCleared() ), 
	   this, SLOT( resetList() ) );

  setLayout( lay1 );
	   
}


gkg::VoiLabelInfoW::VoiLabelInfoW::~VoiLabelInfoW()
{
}


void gkg::VoiLabelInfoW::selectionLabel( const QModelIndex& current )
{

  currentItem = current.row();

  data->setCurrentLabel( 
                  lview->item( currentItem, 1 )->data( Qt::EditRole ).toInt() );

}


void gkg::VoiLabelInfoW::labelChanged( int row, int /* column */ )
{

  QTableWidgetItem* item = lview->item( row, 2 );

  if ( item )
  {

    std::string name( item->text().toStdString() );

    item = lview->item( row, 3 );

    if ( item )
    {

      int32_t red = item->text().toInt();

      item = lview->item( row, 4 );

      if ( item )
      {

        int32_t green = item->text().toInt();

        item = lview->item( row, 5 );

        if ( item )
        {

          int32_t blue = item->text().toInt();

          if ( red < 0 ) red = 0;
          if ( red > 255 ) red = 255;
          if ( green < 0 ) green = 0;
          if ( green > 255 ) green = 255;
          if ( blue < 0 ) blue = 0;
          if ( blue > 255 ) blue = 255;

          applyLabel( name, red, green, blue );

        }

      }

    }

  }

}


void gkg::VoiLabelInfoW::chooseColor()
{

  if ( currentItem )
  {
  
    QTableWidgetItem* item = lview->item( currentItem, 2 );
    std::string name( item->text().toStdString() );

    item = lview->item( currentItem, 3 );
    int32_t red = item->text().toInt();

    item = lview->item( currentItem, 4 );
    int32_t green = item->text().toInt();

    item = lview->item( currentItem, 5 );
    int32_t blue = item->text().toInt();

    if ( ( red + green + blue ) == 0 )
    {

      red = 255;
      green = 255;
      blue = 255;

    }

    QColor initColor( red, green, blue );

    QColor color = QColorDialog::getColor( initColor, this );

    if ( color.isValid() && ( color != initColor ) )
    {

      red = color.red();
      green = color.green();
      blue = color.blue();

      applyLabel( name, red, green, blue );
      
    }

  }
    
}


void gkg::VoiLabelInfoW::applyLabel( const std::string& name, 
                                     int32_t r, int32_t g, int32_t b )
{

  QString tmp( name.c_str() );

  lview->item( currentItem, 2 )->setText( tmp );
  tmp.setNum( r );
  lview->item( currentItem, 3 )->setText( tmp );
  tmp.setNum( g );
  lview->item( currentItem, 4 )->setText( tmp );
  tmp.setNum( b );
  lview->item( currentItem, 5 )->setText( tmp );

  int32_t which = lview->item( currentItem, 1 )->text().toInt();

  data->modifyLabelInfo( which, name, r, g, b );

  if ( currentItem )
  {

    ShowWidget* widget = dynamic_cast< ShowWidget* >( 
                                          lview->cellWidget( currentItem, 0 ) );
    widget->setColor( gkg::RGBComponent( r, g, b ) );

  }

}


void gkg::VoiLabelInfoW::setSelection( int index )
{

  QString text;

  text.setNum( index );

  QList< QTableWidgetItem* > items = lview->findItems( text, Qt::MatchExactly );
  QList< QTableWidgetItem* >::iterator
    i = items.begin(),
    ie = items.end();
  int32_t value = -1, row = -1;

  while ( ( i != ie ) && ( value != index ) )
  {

    value = (*i)->data( Qt::EditRole ).toInt();
    row = (*i)->row();
    ++i;

  }

  if ( value == index )
  {

    lview->selectRow( row );

  }

}


void gkg::VoiLabelInfoW::applySelection( bool state )
{

  std::set< int32_t >::iterator
    i = selectedROI.begin(),
    ie = selectedROI.end();

  while ( i != ie )
  {

    int32_t label = lview->item( *i, 1 )->text().toInt();
    ShowWidget* widget = dynamic_cast< ShowWidget* >( 
                                                   lview->cellWidget( *i, 0 ) );

    widget->setShow( state );
    data->labelInfo().setShow( label, state );

    ++i;

  }

  data->notifyPalette();

}


void gkg::VoiLabelInfoW::showSelection()
{

  applySelection( true );

}


void gkg::VoiLabelInfoW::hideSelection()
{

  applySelection( false );

}


void gkg::VoiLabelInfoW::resetList()
{

  int32_t rowCount = lview->rowCount();
  int32_t labelCount = data->labelInfo().count();
  int32_t n = rowCount;

  disconnect( lview, SIGNAL( cellChanged( int, int ) ),
              this, SLOT( labelChanged( int, int ) ) );

  lview->clearContents();

  if ( rowCount < labelCount )
  {

    while ( n < labelCount )
    {

      lview->insertRow( n );
      n++;

    }

  }
  else if ( labelCount < rowCount )
  {

    while ( n >= labelCount )
    {

      lview->removeRow( n );
      n--;

    }

  }

  std::map< int32_t, gkg::LabelInformation >::iterator
    i = data->labelInfo().info().begin(),
    ie = data->labelInfo().info().end();

  n = 0;

  while ( i != ie )
  {

    addRow( n, i->first, i->second );
    n++;
    ++i;

  }
  
  updateRows();

  connect( lview, SIGNAL( cellChanged( int, int ) ),
           this, SLOT( labelChanged( int, int ) ) );

  if ( !data->empty() )
  {

    data->notifyData();

  }

}


void gkg::VoiLabelInfoW::addLabel()
{

  int32_t l = data->labelInfo().getFirstAvailable();

  if ( l > 0 )
  {

    gkg::LabelInformation newLabel( "New label" );

    lview->insertRow( l );
    int32_t row = addRow( l, l, newLabel );
    lview->selectRow( row );
    updateRows();

  }

}


int32_t gkg::VoiLabelInfoW::addRow( int32_t index, 
                                    int32_t label, 
                                    gkg::LabelInformation& lInfo )
{

  QString tmp;
  ShowWidget* widget = new ShowWidget();
  QTableWidgetItem* labelItem = new QTableWidgetItem();

  widget->setColor( lInfo.rgb );
  lview->setCellWidget( index, 0, widget );
  labelItem->setData( Qt::EditRole, label );
  lview->setItem( index, 1, labelItem );
  int32_t row = labelItem->row();
  lview->setItem( row, 2, new QTableWidgetItem( lInfo.name.c_str() ) );
  tmp.setNum( (int32_t)lInfo.rgb.r );
  lview->setItem( row, 3, new QTableWidgetItem( tmp ) );
  tmp.setNum( (int32_t)lInfo.rgb.g );
  lview->setItem( row, 4, new QTableWidgetItem( tmp ) );
  tmp.setNum( (int32_t)lInfo.rgb.b );
  lview->setItem( row, 5, new QTableWidgetItem( tmp ) );
  connect( widget, SIGNAL( buttonChange( int ) ), 
           this, SLOT( buttonClicked( int ) ) );
  connect( widget, SIGNAL( checkChange( int, int ) ),
           this, SLOT( checkChanged( int, int ) ) );

  return row;

}


void gkg::VoiLabelInfoW::updateRows()
{

  int32_t i, n = lview->rowCount();

  selectedROI.clear();

  for ( i = 1; i < n; i++ )
  {

    ShowWidget* widget = dynamic_cast< ShowWidget* >( 
                                                    lview->cellWidget( i, 0 ) );

    widget->setRow( i );

    if ( widget->isChecked() )
    {

      selectedROI.insert( i );

    }

  }

}


void gkg::VoiLabelInfoW::removeLabel()
{

  if ( currentItem )
  {

    ShowWidget* widget = dynamic_cast< ShowWidget* >( 
                                          lview->cellWidget( currentItem, 0 ) );
    int32_t label = lview->item( currentItem, 1 )->text().toInt();
    
    data->labelInfo().removeLabel( label );
    disconnect( widget, SIGNAL( buttonChange( int ) ), 
                this, SLOT( buttonClicked( int ) ) );
    disconnect( widget, SIGNAL( checkChange( int, int ) ),  
                this, SLOT( checkChanged( int, int ) ) );
    lview->removeRow( currentItem );
    updateRows();

  }

}


void gkg::VoiLabelInfoW::loadLabel()
{

  QString filename = QFileDialog::getOpenFileName(
                                                 this,
                                                 tr( "Open label definitions" ),
                                                 QString(),
                                                 tr( "Labels (*.labels)" ) );

  if ( !filename.isEmpty() )
  {

    if ( data->labelInfo().read( filename.toStdString() ) )
    {

      resetList();

    }

  }

}


void gkg::VoiLabelInfoW::saveLabel()
{

  QString filename = QFileDialog::getSaveFileName(
                                                 this,
                                                 tr( "Save label definitions" ),
                                                 QString(),
                                                 tr( "Labels (*.labels)" ) );

  if ( !filename.isEmpty() )
  {

    data->labelInfo().write( filename.toStdString() );

  }

}


void gkg::VoiLabelInfoW::buttonClicked( int row )
{

  int32_t label = lview->item( row, 1 )->text().toInt();

  data->setCurrentLabel( label );
  data->labelInfo().toggleShow( label );
  data->notifyPalette();

}


void gkg::VoiLabelInfoW::checkChanged( int row, int state )
{

  if ( row )
  {

    if ( state == Qt::Checked )
    {

      selectedROI.insert( row );

    }
    else
    {

      selectedROI.erase( row );

    }

  }

}


void gkg::VoiLabelInfoW::headerChecked( bool checked )
{

  int32_t i, n = lview->rowCount();

  for ( i = 1; i < n; i++ )
  {

    ShowWidget* widget = dynamic_cast< ShowWidget* >( 
                                                    lview->cellWidget( i, 0 ) );

    if ( widget->isChecked() != checked )
    {

      widget->setCheck( checked );

    }

  }

}
