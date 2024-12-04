#include <gkg-graphics-QtGL-plugin-functors/Voi/EvolInfoW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/SpinBox.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QGridLayout>


gkg::VoiEvolutionInfoW::VoiEvolutionInfoW( gkg::VoiObservable *d, QWidget *p )
                       : QGroupBox( "Region growing", p )
{

  data = (gkg::VoiData *)d;

  QVBoxLayout* layout = new QVBoxLayout();

  QHBoxLayout* hbox = new QHBoxLayout();
  QRadioButton *bt2D = new QRadioButton( "2D" );
  bt2D->setChecked( true );
  QRadioButton *bt3D = new QRadioButton( "3D" );
  QButtonGroup *btDim = new QButtonGroup();
  btDim->addButton( bt2D, 0 );
  btDim->addButton( bt3D, 1 );
  connect( btDim, SIGNAL( buttonPressed( int ) ), 
	   &data->evolInfo(), SLOT( setDim( int ) ) );
  connect( btDim, SIGNAL( buttonPressed( int ) ), 
           this, SLOT( dimChange( int ) ) );

  static const char *connex2D[] = { "4", "8", 0 };
  cbConnex = new gkg::VoiComboBox( false, this, "Connectivity " );
  cbConnex->insertStrList( connex2D, 2 );
  connect( cbConnex, SIGNAL( activated( int ) ),
	   &data->evolInfo(), SLOT( setConnex( int ) ) );
  hbox->addWidget( bt2D );
  hbox->addWidget( bt3D );
  hbox->addWidget( cbConnex );
  layout->addLayout( hbox );

  // Group box tolerance
  QGridLayout* g2 = new QGridLayout();

  QRadioButton *bt1 = new QRadioButton( "Tolerance (%) " );
  bt1->setChecked( true );

  sl = new gkg::VoiSlider();
  sl->setRange( 0, 100 );
  sl->setValue( 10 );
  data->evolInfo().setValue( 10 );
  connect( sl, SIGNAL( valueChanged( int ) ), 
           this, SLOT( pctChange( int ) ) );

  QRadioButton *bt2 = new QRadioButton( "Tolerance (value) " );

  sb = new QSpinBox();
  sb->setFixedWidth( 70 );
  sb->setValue( 10 );
  sb->setEnabled( false );
  sb->setRange( 0, 1000 );
  sb->setSingleStep( 1 );
  connect( sb, SIGNAL( valueChanged( int ) ), 
	   &data->evolInfo(), SLOT( setValue( int ) ) );

  QButtonGroup *bg = new QButtonGroup();
  bg->addButton( bt1, 0 );
  bg->addButton( bt2, 1 );

  connect( bg, SIGNAL( buttonPressed( int ) ), 
           this, SLOT( changeQRadio( int ) ) );

  g2->addWidget( bt1, 0, 0 );
  g2->addWidget( sl, 0, 1 );
  g2->addWidget( bt2, 1, 0 );
  g2->addWidget( sb, 1, 1 );
  layout->addLayout( g2 );

  // Group regularization
  QHBoxLayout* hb3 = new QHBoxLayout();

  QLabel* regLabel = new QLabel( "Regularization " );

  gkg::VoiSpinBox *vsb = new gkg::VoiSpinBox( 0, 200, 2, 1 );
  vsb->setValue( 100 );

  connect( vsb, SIGNAL( valueChanged( int ) ), 
	   vsb, SLOT( valChange( int ) ) );
  connect( vsb, SIGNAL( valueChanged( float ) ), 
	   &data->evolInfo(), SLOT( setIsing( float ) ) );
  hb3->addWidget( regLabel );
  hb3->addWidget( vsb );
  layout->addLayout( hb3 );

  setLayout( layout );
  
}


gkg::VoiEvolutionInfoW::~VoiEvolutionInfoW()
{
}


void gkg::VoiEvolutionInfoW::dimChange( int index )
{

  switch ( index )
  {
    
    case 0:
    {
      
      static const char *connex2D[] = { "4", "8", 0 };
      cbConnex->insertStrList( connex2D, 2 );
      break;
	
    }
    case 1:
    {
      
      static const char *connex3D[] = { "6", "18", "26", 0 };
      cbConnex->insertStrList( connex3D, 3 );
      break;
	
    }
      
  }

  data->evolInfo().setConnex( 0 );
  
}


void gkg::VoiEvolutionInfoW::changeQRadio( int id )
{

  switch ( id )
  {
    
    case 0:
    {
      
      sl->setEnabled( true );
      sb->setEnabled( false );
      pctChange( sl->value() );
      break;
	
    }
    case 1:
    {
      
      sb->setEnabled( true );
      sl->setEnabled( false );
      data->evolInfo().setValue( sb->value() );
      break;
	
    }
      
  }
    
}


void gkg::VoiEvolutionInfoW::pctChange( int val )
{

  data->evolInfo().setValue( val );

}
