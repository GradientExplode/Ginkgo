#include <gkg-graphics-QtGL-plugin-functors/Voi/FusionSliders.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Slider.h>

#include <QVBoxLayout>


gkg::FusionSliders::FusionSliders( VoiObservable* d, QWidget* parent ) 
                  : QWidget( parent )
{

  data = (gkg::VoiData *)d;

  QVBoxLayout* layout = new QVBoxLayout();
  layout->setContentsMargins( 5, 10, 5, 0 );
  layout->setSpacing( 5 );

  gkg::VoiSlider* sliderL = new gkg::VoiSlider( this, "Fusion T1-Label (%): " );
  sliderL->setRange( 0, 100 );
  sliderL->setSteps( 1, 1 );
  sliderL->setValue( data->fusionLabel() );
  connect( sliderL, SIGNAL( valueChanged( int ) ), 
           this, SLOT( setFusionLabel( int ) ) );

  gkg::VoiSlider* sliderO = new gkg::VoiSlider( this, "Fusion T1-Other (%): " );
  sliderO->setRange( 0, 100 );
  sliderO->setSteps( 1, 1 );
  sliderO->setValue( data->fusionBaseVolume() );
  connect( sliderO, SIGNAL( valueChanged( int ) ), 
           this, SLOT( setFusionBaseVolume( int ) ) );

  layout->addWidget( sliderL );
  layout->addWidget( sliderO );

  setLayout( layout );

}


void gkg::FusionSliders::setFusionLabel( int val )
{

  if ( !data->empty() )
  {

    data->setFusionLabel( val );

  }

}


void gkg::FusionSliders::setFusionBaseVolume( int val )
{

  if ( !data->empty() )
  {

    data->setFusionBaseVolume( val );

  }

}
