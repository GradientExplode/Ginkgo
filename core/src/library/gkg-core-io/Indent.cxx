#include <gkg-core-io/Indent.h>


#define INDENT_SPACE_COUNT  2U


gkg::Indent::Indent()
{

  reset();

}


void gkg::Indent::reset()
{

  _count = 0;

}


void gkg::Indent::increment()
{

  _count ++;

}


void gkg::Indent::decrement()
{

  if ( _count != 0U )
  {

    _count --;

  }

}


std::string gkg::Indent::getSpaces() const
{

  return std::string( _count * INDENT_SPACE_COUNT, ' ' );

}


#undef INDENT_SPACE_COUNT
