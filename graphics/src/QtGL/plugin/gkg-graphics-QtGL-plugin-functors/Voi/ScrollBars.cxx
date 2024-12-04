#include <gkg-graphics-QtGL-plugin-functors/Voi/ScrollBars.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-processing-container/Volume_i.h>

#include <QLabel>
#include <QScrollBar>
#include <QGridLayout>


gkg::ScrollBars::ScrollBars( QWidget* parent )
               : QWidget( parent ),
                 _parent( parent ),
                 _count( 0 )
{

  QFont font( "helvetica", 6 );
  _label = new QLabel( "0" );
  _label->setFont( font );
  _label->setAlignment( Qt::AlignRight );
  _labelT = new QLabel( "0" );
  _labelT->setFont( font );
  _labelT->setAlignment( Qt::AlignRight );
  _scrollBar = new QScrollBar( Qt::Vertical, this );
  _scrollBar->setSingleStep( 1 );
  _scrollBar->setPageStep( 10 );
  _scrollBarT = new QScrollBar( Qt::Vertical, this );
  _scrollBarT->setSingleStep( 1 );
  _scrollBarT->setPageStep( 10 );

  _label->hide();
  _labelT->hide();
  _scrollBar->hide();
  _scrollBarT->hide();

  QGridLayout* layout = new QGridLayout();
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->setSpacing( 1 );
  layout->addWidget( _label, 0, 0 );
  layout->addWidget( _labelT, 0, 1 );
  layout->addWidget( _scrollBar, 1, 0 );
  layout->addWidget( _scrollBarT, 1, 1 );

  connect( _scrollBar, SIGNAL( valueChanged( int ) ),
           _label, SLOT( setNum( int ) ) );
  connect( _scrollBarT, SIGNAL( valueChanged( int ) ),
           _labelT, SLOT( setNum( int ) ) );

  connect( _scrollBar, SIGNAL( valueChanged( int ) ),
           this, SLOT( changeSlice( int ) ) );
  connect( _scrollBarT, SIGNAL( valueChanged( int ) ),
           this, SLOT( changeFrame( int ) ) );

  setLayout( layout );

}



void gkg::ScrollBars::initialize( gkg::VoiData& data,
                                  gkg::VoiStrategy::StgyType type )
{

  gkg::Volume< float >& t1 = data.getBaseFusion().getT1();
  int32_t n = 0;
  QString txt;

  _count = 0;

  switch ( type )
  {

    case gkg::VoiStrategy::StgyAxial:
      n = t1.getSizeZ();
      break;

    case gkg::VoiStrategy::StgySagittal:
      n = t1.getSizeX();
      break;

    case gkg::VoiStrategy::StgyCoronal:
      n = t1.getSizeY();
      break;

    default:
      return;
      break;

  }

  if ( n > 1 )
  {

    _scrollBar->setRange( 0, n - 1 );
    _scrollBar->setValue( n / 2 );
    txt.setNum( n / 2 );
    _label->setText( txt );
    _label->show();
    _scrollBar->show();
    _count++;

  }
  else
  {

    _label->hide();
    _scrollBar->hide();

  }

  n = t1.getSizeT();

  if ( n > 1 )
  {

    _scrollBarT->setRange( 0, n - 1 );
    _scrollBarT->setValue( 0 );
    txt.setNum( 0 );
    _labelT->setText( txt );
    _labelT->show();
    _scrollBarT->show();
    _count++;

  }
  else
  {

    _labelT->hide();
    _scrollBarT->hide();

  }

  if ( _parent )
  {

    setSizes( _parent->width(), _parent->height() );

  }

}


void gkg::ScrollBars::setSizes( int32_t windowWidth, int32_t windowHeight )
{

  int32_t width = _count * 16;

  _scrollBar->setFixedSize( 16, windowHeight - 16 );
  _scrollBarT->setFixedSize( 16, windowHeight - 16 );

  setGeometry( windowWidth - width, 0, width, windowHeight );

}


void gkg::ScrollBars::updateSlice( gkg::Vector3d< int32_t >& pt, 
                                   gkg::VoiStrategy::StgyType type )
{

  disconnect( _scrollBar, SIGNAL( valueChanged( int ) ),
              this, SLOT( changeSlice( int ) ) );

  switch ( type )
  {

    case gkg::VoiStrategy::StgyAxial:
      _scrollBar->setValue( pt.z );
      break;

    case gkg::VoiStrategy::StgySagittal:
      _scrollBar->setValue( pt.x );
      break;

    case gkg::VoiStrategy::StgyCoronal:
      _scrollBar->setValue( pt.y );
      break;

    default:
      return;
      break;

  }

  connect( _scrollBar, SIGNAL( valueChanged( int ) ),
           this, SLOT( changeSlice( int ) ) );

}


void gkg::ScrollBars::deltaSlice( int32_t delta )
{

  _scrollBar->setValue( _scrollBar->value() + delta );

}


void gkg::ScrollBars::changeSlice( int value )
{

  emit valueChanged( value );

}


void gkg::ScrollBars::changeFrame( int value )
{

  emit frameChanged( value );

}
