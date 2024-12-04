#include <gkg-graphics-QtGL-plugin-functors/Voi/MorphologyW.h>
#include <gkg-graphics-QtGL-plugin-functors/Voi/Data.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QButtonGroup>


gkg::MorphologyW::MorphologyW( VoiObservable* d, QWidget* p )
                 : QGroupBox( "Mathematical morphology", p ),
                   _data( (gkg::VoiData*)d )
{

  QHBoxLayout* layout = new QHBoxLayout();

  QLabel* txt = new QLabel( "Radius:" );

  QDoubleSpinBox* sb = new QDoubleSpinBox();
  sb->setRange( 0.1, 20.0 );
  sb->setDecimals( 1 );
  sb->setSingleStep( 0.1 );
  sb->setValue( 2.0 );
  connect( sb, SIGNAL( valueChanged( double ) ),
           this, SLOT( radiusChange( double ) ) );

  QButtonGroup* bgroup = new QButtonGroup();
  bgroup->setExclusive( true );

  QPixmap erosionIcon( ":voi/icon/erosion.png" );
  QPushButton* b1 = new QPushButton( QIcon( erosionIcon ), "" );
  b1->setIconSize( QSize( 28, 28 ) );
  b1->setCheckable( true );
  b1->setToolTip( "Erosion" );
  QPixmap dilationIcon( ":voi/icon/dilation.png" );
  QPushButton* b2 = new QPushButton( QIcon( dilationIcon ), "" );
  b2->setIconSize( QSize( 28, 28 ) );
  b2->setCheckable( true );
  b2->setToolTip( "Dilation" );
  QPixmap openingIcon( ":voi/icon/opening.png" );
  QPushButton* b3 = new QPushButton( QIcon( openingIcon ), "" );
  b3->setIconSize( QSize( 28, 28 ) );
  b3->setCheckable( true );
  b3->setToolTip( "Opening" );
  QPixmap closingIcon( ":voi/icon/closing.png" );
  QPushButton* b4 = new QPushButton( QIcon( closingIcon ), "" );
  b4->setIconSize( QSize( 28, 28 ) );
  b4->setCheckable( true );
  b4->setChecked( true );
  b4->setToolTip( "Closing" );

  bgroup->addButton( b1, 1 );
  bgroup->addButton( b2, 2 );
  bgroup->addButton( b3, 3 );
  bgroup->addButton( b4, 4 );
  connect( bgroup, SIGNAL( buttonClicked( int ) ), 
           this, SLOT( setOperation( int ) ) );

  layout->addWidget( txt );
  layout->addWidget( sb );
  layout->addWidget( b1 );
  layout->addWidget( b2 );
  layout->addWidget( b3 );
  layout->addWidget( b4 );

  setLayout( layout );

}


void gkg::MorphologyW::radiusChange( double value )
{

  _data->setMorphologyRadius( value );

}


void gkg::MorphologyW::setOperation( int value )
{

  _data->setMorphologyOperation( value );

}
