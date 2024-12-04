#include <gkg-graphics-QtGL-plugin-functors/Voi/SpinBox.h>

#include <cmath>


gkg::VoiSpinBox::VoiSpinBox( int32_t pr, QWidget *p, const std::string& )
                : QSpinBox( p )
{

  prec = (int32_t)pow( 10, pr );

  setRange( 0, 100 );
  setSingleStep( 1 );

}


gkg::VoiSpinBox::VoiSpinBox( int32_t min, int32_t max, int32_t pr, int32_t st, 
                             QWidget *p, const std::string& )
                : QSpinBox( p )
{

  prec = (int32_t)pow( 10, pr );

  setRange( min, max );
  setSingleStep( st );

}


void gkg::VoiSpinBox::valChange( int value )
{

  emit valueChanged( (float)value / (float)prec );

}
