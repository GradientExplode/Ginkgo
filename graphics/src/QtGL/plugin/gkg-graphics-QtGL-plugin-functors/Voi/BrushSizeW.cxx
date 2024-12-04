#include <gkg-graphics-QtGL-plugin-functors/Voi/BrushSizeW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QImage>

#include <cmath>


gkg::VoiBrushSizeW::VoiBrushSizeW( gkg::VoiObservable *d, QWidget *p )
                   : QWidget( p )
{

  data = (gkg::VoiData *)d;

  QHBoxLayout* layout = new QHBoxLayout( this );
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->setSpacing( 5 );

  QGroupBox* groupBox = new QGroupBox( "Brush size" );

  QButtonGroup* bgroup = new QButtonGroup();
  bgroup->setExclusive( true );

  QGridLayout* lay = new QGridLayout();
  QImage img( 19, 19, QImage::Format_RGB32 );
  int32_t i, x, y;

  // square brushes
  img.fill( qRgb( 150, 150, 150 ) );

  for ( i = 0; i < 16; i++ )
  {

    int32_t b0 = ( i + 1 ) / 2;
    int32_t b1 = b0 + ( ( i + 1 ) % 2 );

    for ( y = -b0; y < b1; y++ )
    {

      for ( x = -b0; x < b1; x++ )
      {

        img.setPixel( 9 + x, 9 + y, 0 );

      }

    }

    QPushButton* btn = 
                      new QPushButton( QIcon( QPixmap::fromImage( img ) ), "" );
    btn->setFixedSize( 28, 28 );
    btn->setCheckable( true );

    if ( i < 1 )
    {

      btn->setChecked( true );

    }

    lay->addWidget( btn, i / 8, i % 8 );
    bgroup->addButton( btn, i + 1 );

  }

  // round brushes
  img.fill( qRgb( 150, 150, 150 ) );

  for ( i = 0; i < 8; i++ )
  {

    int32_t b0 = ( 2 * i + 1 ) / 2;

    for ( y = -b0; y <= b0; y++ )
    {

      for ( x = -b0; x <= b0; x++ )
      {

        int32_t r = int32_t( std::sqrt( x * x + y * y ) + 0.5 );

        if ( r <= b0 )
        {

          img.setPixel( 9 + x, 9 + y, 0 );

        }

      }

    }

    QPushButton* btn = 
                      new QPushButton( QIcon( QPixmap::fromImage( img ) ), "" );
    btn->setFixedSize( 28, 28 );
    btn->setCheckable( true );

    lay->addWidget( btn, ( i + 16 ) / 8, ( i + 16 ) % 8 );
    bgroup->addButton( btn, 2 * i + 17 );

  }

  groupBox->setLayout( lay );
  layout->addWidget( groupBox );

  connect( bgroup, SIGNAL( buttonClicked( int ) ), 
           this, SLOT( setSize( int ) ) );
  
}


void gkg::VoiBrushSizeW::setSize( int bsize )
{

  if ( bsize > 16 )
  {

    data->setCircleBrushSize( bsize % 16 );

  }
  else
  {

    data->setBrushSize( bsize );

  }

}
