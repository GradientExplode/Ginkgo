#include <gkg-graphics-QtGL-widget/LabelledComboBox.h>
#include <gkg-core-exception/Exception.h>

#include <QHBoxLayout>


gkg::LabelledComboBox::LabelledComboBox( QWidget* parent, 
                                         const std::string& name )
                     : QWidget( parent )
{

  try
  {

    QLabel* title = new QLabel( tr( name.c_str() ) );

    _combobox = new QComboBox();
    _combobox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
  
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins( 0, 0, 0, 0 );
    hLayout->addWidget( title );
    hLayout->addWidget( _combobox );

    connect( _combobox, SIGNAL( activated( const QString& ) ),
             this, SLOT( activate( const QString& ) ) );

    setLayout( hLayout );

  }
  GKG_CATCH( "gkg::LabelledComboBox::LabelledComboBox( "
             "QWidget* parent, const std::string& name )" );

}


void gkg::LabelledComboBox::insertItem( const std::string& text )
{

  _combobox->insertItem( _combobox->count(), text.c_str() );
  
}


void gkg::LabelledComboBox::clear()
{

  _combobox->clear();

}


int32_t gkg::LabelledComboBox::count()
{

  return _combobox->count();

}


void gkg::LabelledComboBox::activate( const QString& str )
{

  emit activated( str );

}


std::string gkg::LabelledComboBox::currentText() const
{

  return _combobox->currentText().toStdString();

}


int32_t gkg::LabelledComboBox::currentIndex() const
{

  return _combobox->currentIndex();

}


void gkg::LabelledComboBox::setCurrentIndex( uint32_t index )
{

  _combobox->setCurrentIndex( index );

}


void gkg::LabelledComboBox::setCurrentText( const QString& text )
{

  _combobox->setCurrentText( text );
  emit activated( text );

}


QComboBox* gkg::LabelledComboBox::getComboBox() const
{

  return _combobox;

}
