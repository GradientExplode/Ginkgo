#include <gkg-core-io/Eraser.h>



gkg::Eraser::Eraser( int32_t count )
{

  int32_t c;
  for ( c = 0; c < count; c++ )
  {

    _string += '\b';

  }
  for ( c = 0; c < count; c++ )
  {

    _string += ' ';

  }
  for ( c = 0; c < count; c++ )
  {

    _string += '\b';

  }

}


const std::string& gkg::Eraser::getString() const
{

  return _string;

}



std::ostream& std::operator << ( std::ostream& os, const gkg::Eraser& thing )
{

  os << thing.getString();
  return os;

}
