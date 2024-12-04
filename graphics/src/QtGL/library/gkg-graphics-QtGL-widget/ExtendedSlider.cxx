#include <gkg-graphics-QtGL-widget/ExtendedSlider.h>
#include <gkg-core-exception/Exception.h>

#include <QLabel>
#include <QSlider>
#include <QGridLayout>

#include <sstream>
#include <iostream>
#include <iomanip>


gkg::ExtendedSlider::ExtendedSlider( Qt::Orientation orientation,
                                     QWidget* parent,
                                     const std::string& name,
                                     float resolution,
                                     float min, 
                                     float max, 
                                     float value )
                   : QWidget( parent ),
                     _minimum( min ),
                     _maximum( max ),
                     _value ( value ),
                     _resolution( resolution )
{

  try
  {

    QLabel* title = new QLabel( tr( name.c_str() ) );
    _slider = new QSlider( orientation );
    _labelMin = new QLabel();
    _labelMax = new QLabel();
    _labelValue = new QLabel();
    _precision = ( resolution <= 1.0f ) ? 0 : 2;

    setRange( min, max );
    setValue( value );

    connect( _slider, SIGNAL( valueChanged( int ) ), 
             this, SLOT( updateValue( int ) ) );

    QGridLayout* grid = new QGridLayout();
    grid->setContentsMargins( 0, 0, 0, 0 );
    grid->addWidget( title, 0, 0 );
    grid->addWidget( _labelValue, 0, 1 );
    grid->addWidget( _labelMin, 1, 1 );
    grid->addWidget( _slider, 1, 2 );
    grid->addWidget( _labelMax, 1, 3 );
 
    setLayout( grid );
  
  }
  GKG_CATCH( "gkg::ExtendedSlider::ExtendedSlider( "
             "Qt::Orientation orientation, "
             "QWidget* parent, const std::string& name, "
             "float min, float max, float val )" );

}


float gkg::ExtendedSlider::getResolution()
{

  return _resolution;

}


void gkg::ExtendedSlider::setRange( float min, float max )
{

  try
  {

   _slider->setRange( ( int32_t ) ( _resolution * min + 0.5f ),
                       ( int32_t ) ( _resolution * max + 0.5f ) );

    std::ostringstream oss;
    oss << std::fixed << std::setprecision( _precision ) << min;
    _labelMin->setText( oss.str().c_str() );

    oss.str( "" );
    oss << std::fixed << std::setprecision( _precision ) << max;
    _labelMax->setText( oss.str().c_str() );

  }
  GKG_CATCH( "void gkg::ExtendedSlider::setRange( float min, float max )" );

}


void gkg::ExtendedSlider::setPageStep( float step )
{

  _slider->setPageStep( ( int32_t )( _resolution * step ) );

}


void gkg::ExtendedSlider::setSingleStep( float step )
{

  _slider->setSingleStep( ( int32_t )( _resolution * step ) );

}


float gkg::ExtendedSlider::getSingleStep()
{

  return _slider->singleStep() / _resolution;

}


float gkg::ExtendedSlider::getMaximum()
{

  return _maximum;

}


float gkg::ExtendedSlider::getMinimum()
{

  return _minimum;

}


float gkg::ExtendedSlider::getValue() const
{

  return _value;

}


void gkg::ExtendedSlider::setSliderPosition( float val )
{

  setValue( val );

}


float gkg::ExtendedSlider::getSliderPosition()
{

  return getValue();

}


QSlider* gkg::ExtendedSlider::getSlider() const
{

  return _slider;

}


void gkg::ExtendedSlider::setEnabled( bool enable )
{

  _slider->setEnabled( enable );
  _labelValue->setEnabled( enable );
  _labelMin->setEnabled( enable );
  _labelMax->setEnabled( enable );

}


void gkg::ExtendedSlider::show()
{

  _slider->show();
  _labelValue->show();
  _labelMin->show();
  _labelMax->show();

}


void gkg::ExtendedSlider::updateValue( int value )
{

  try
  {

    _value = float( value ) / _resolution;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision( _precision ) << _value;
    _labelValue->setText( oss.str().c_str() );

    emit valueChanged( _value );

  }
  GKG_CATCH( "void gkg::ExtendedSlider::updateValue( int value )" );

}


void gkg::ExtendedSlider::setValue( float val )
{

  try
  {

    _value = val;
    _slider->setValue( ( int32_t ) ( _resolution * val + 0.5f ) );

    std::ostringstream oss;
    oss << std::fixed << std::setprecision( _precision ) << val;
    _labelValue->setText( oss.str().c_str() );

  }
  GKG_CATCH( "void gkg::ExtendedSlider::setValue( float val )" );

}
