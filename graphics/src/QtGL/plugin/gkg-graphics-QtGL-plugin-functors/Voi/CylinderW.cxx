#include <gkg-graphics-QtGL-plugin-functors/Voi/CylinderW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Slider.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>


gkg::CylinderW::CylinderW( VoiObservable* d, QWidget* p )
                 : QGroupBox( "Cylinder mask", p ),
                   _data( (gkg::VoiData*)d )
{

  QHBoxLayout* layout = new QHBoxLayout();

  gkg::VoiSlider* sl = new gkg::VoiSlider( this, "Diameter (mm): ");
  sl->setRange( 10, 100 );
  sl->setSteps( 1, 1 );
  sl->setValue( _data->getCylinderDiameter() );
  connect( sl, SIGNAL( valueChanged( int ) ),
           this, SLOT( diameterChange( int ) ) );

  QButtonGroup* bgroup = new QButtonGroup();
  bgroup->setExclusive( true );

  QPixmap axialIcon( ":voi/icon/cylinder_axial.png" );
  QPushButton* b1 = new QPushButton( QIcon( axialIcon ), "" );
  b1->setIconSize( QSize( 28, 28 ) );
  b1->setCheckable( true );
  QPixmap sagittalIcon( ":voi/icon/cylinder_sagittal.png" );
  QPushButton* b2 = new QPushButton( QIcon( sagittalIcon ), "" );
  b2->setIconSize( QSize( 28, 28 ) );
  b2->setCheckable( true );
  QPixmap coronalIcon( ":voi/icon/cylinder_coronal.png" );
  QPushButton* b3 = new QPushButton( QIcon( coronalIcon ), "" );
  b3->setIconSize( QSize( 28, 28 ) );
  b3->setCheckable( true );

  switch ( _data->getCylinderOrientation() )
  {

    case 2:
      b2->setChecked( true );
      break;

    case 3:
      b3->setChecked( true );
      break;

    case 1:
    default:
      b1->setChecked( true );
      break;
      
  }

  bgroup->addButton( b1, 1 );
  bgroup->addButton( b2, 2 );
  bgroup->addButton( b3, 3 );
  connect( bgroup, SIGNAL( buttonClicked( int ) ), 
           this, SLOT( setCylinderOrientation( int ) ) );

  layout->addWidget( sl );
  layout->addWidget( b1 );
  layout->addWidget( b2 );
  layout->addWidget( b3 );

  setLayout( layout );

}


void gkg::CylinderW::diameterChange( int value )
{

  _data->setCylinderDiameter( value );

}


void gkg::CylinderW::setCylinderOrientation( int value )
{

  _data->setCylinderOrientation( value );

}
