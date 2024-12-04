#include <gkg-graphics-QtGL-plugin-functors/Voi/ComboBox.h>

#include <QHBoxLayout>
#include <QLabel>


gkg::VoiComboBox::VoiComboBox( bool rw, QWidget *parent, 
                               const std::string& text )
                 : QWidget( parent )
{

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setSpacing( 5 );

  QLabel* label = new QLabel( text.c_str(), this );

  cbox = new QComboBox( this );
  cbox->setEditable( rw );

  layout->addWidget( label );
  layout->addWidget( cbox );

  connect( cbox, SIGNAL( activated( int ) ), 
           this, SLOT( activate( int ) ) );
  connect( cbox, SIGNAL( activated( const QString& ) ),
	   this, SLOT( activate( const QString& ) ) );

  setLayout( layout );

}


void gkg::VoiComboBox::insertStrList( const char **lst, int32_t index )
{

  if ( cbox->count() > 0 )
  {
  
    cbox->clear();
    
  }
  
  int32_t i;
  
  for ( i = 0; i < index; i++ )
  {

    cbox->addItem( QString( lst[ i ] ) );

  }
  
}


void gkg::VoiComboBox::activate( int index )
{

  emit activated( index );

}


void gkg::VoiComboBox::activate( const QString& str )
{

  emit activated( str );

}
