#include <gkg-graphics-QtGL-plugin-functors/Voi/Slider.h>

#include <QHBoxLayout>


gkg::VoiSlider::VoiSlider( QWidget *p, const std::string& ltext ) 
               : QWidget( p )
{

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins( 0, 0, 0, 0 );
  layout->setSpacing( 5 );

  if ( !ltext.empty() )
  {
  
    QLabel* label = new QLabel( ltext.c_str() );
    layout->addWidget( label );
      
  }

  sld = new QSlider( Qt::Horizontal, this );
  sld->setTracking( false );
  sld->setSingleStep( 1 );
  sld->setPageStep( 1 );

  lcd = new QLabel( this );

  layout->addWidget( sld );
  layout->addWidget( lcd );

  connect( sld, SIGNAL( valueChanged( int ) ), 
           lcd, SLOT( setNum( int ) ) );
  connect( sld, SIGNAL( valueChanged( int ) ), 
           this, SLOT( change( int ) ) );
  connect( sld, SIGNAL( sliderMoved( int ) ), 
           lcd, SLOT( setNum( int ) ) );
  connect( sld, SIGNAL( sliderMoved( int ) ), 
           this, SLOT( change( int ) ) );

  setLayout( layout );

}


void gkg::VoiSlider::setRange( int32_t deb, int32_t fin )
{

  sld->setRange( deb, fin );

}


void gkg::VoiSlider::setValue( int32_t val )
{

  sld->setValue( val );

}


void gkg::VoiSlider::setSteps( int32_t lineStep, int32_t pageStep )
{

  sld->setSingleStep( lineStep );
  sld->setPageStep( pageStep );

}


void gkg::VoiSlider::updateValue( int32_t val )
{

  disconnect( sld, SIGNAL( valueChanged( int ) ), 
	      this, SLOT( change( int ) ) );
  setValue( val );
  connect( sld, SIGNAL( valueChanged( int ) ), 
           this, SLOT( change( int ) ) );

}


void gkg::VoiSlider::change( int slice )
{

  emit valueChanged( slice );

}
