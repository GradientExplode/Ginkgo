#include <gkg-graphics-QtGL-plugin-functors/Voi/Block.h>


gkg::Block::Block()
          : _label( 0 )
{
}


gkg::Block::~Block()
{
}

int32_t gkg::Block::getLabel()
{

  return _label & 0xffff;

}


void gkg::Block::setLabel( int32_t label )
{

  _label = label | 0x20000;

}


void gkg::Block::setActive( bool show )
{

  if ( show )
  {

    _label |= 0x20000;

  }
  else
  {

    _label &= 0xffff;

  }

}


bool gkg::Block::isActive()
{

  return ( _label & 0x20000 ) ? true : false;

}
