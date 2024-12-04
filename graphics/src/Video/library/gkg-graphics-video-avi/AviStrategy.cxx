#include <gkg-graphics-video-avi/AviStrategy.h>


gkg::AviStrategy::AviStrategy()
                : _pos( 0 ),
		  _used( 0 )
{
}


gkg::AviStrategy::~AviStrategy()
{
}


void gkg::AviStrategy::close()
{
}


uint32_t gkg::AviStrategy::getBufferSize()
{

  return _used;

}


char* gkg::AviStrategy::getBuffer()
{

  return 0;

}
