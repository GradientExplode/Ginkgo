#include <gkg-graphics-QtGL-plugin-functors/Voi/ShowWidget.h>

#include <QHBoxLayout>
#include <QIcon>
#include <QPixmap>


gkg::ShowWidget::ShowWidget( QWidget *p ) 
               : QWidget( p ),
                 _show( true ),
                 _row( 0 )
{

  _btn = new QPushButton( QIcon( QPixmap( ":voi/icon/eye_opened.png" ) ), ""  );
  _btn->setIconSize( QSize( 24, 24 ) );
  _chkBox = new QCheckBox;
  _color = new QLabel;
  _color->setFixedWidth( 16 );

  QHBoxLayout* layout = new QHBoxLayout;
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->addWidget( _btn );
  layout->addWidget( _chkBox );
  layout->addWidget( _color );

  connect( _btn, SIGNAL( clicked() ), this, SLOT( buttonPressed() ) );
  connect( _chkBox, SIGNAL( stateChanged( int ) ), 
           this, SLOT( checkStateChanged( int ) ) );

  setLayout( layout );

}


void gkg::ShowWidget::setShow( bool state )
{

  _show = state;
  _btn->setIcon( QIcon( state ? QPixmap( ":voi/icon/eye_opened.png" )
                              : QPixmap( ":voi/icon/eye_closed.png" ) ) );

}


bool gkg::ShowWidget::isChecked()
{

  return _chkBox->isChecked();

}


void gkg::ShowWidget::setRow( int32_t row )
{

  _row = row;

}


void gkg::ShowWidget::setCheck( bool state )
{

  _chkBox->setChecked( state );

}


void gkg::ShowWidget::setColor( gkg::RGBComponent rgb )
{

  QString bkg = QString( "background-color: rgb( %1, %2, %3 )" ).arg( rgb.r ).
                                                                 arg( rgb.g ).
                                                                 arg( rgb.b );
  _color->setStyleSheet( bkg );

}


void gkg::ShowWidget::buttonPressed()
{

  bool state = !_show;
  setShow( state );
  emit buttonChange( _row );

}


void gkg::ShowWidget::checkStateChanged( int state )
{

  emit checkChange( _row, state );

}
