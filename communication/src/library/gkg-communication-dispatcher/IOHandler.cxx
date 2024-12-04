#include <gkg-communication-dispatcher/IOHandler.h>


// implement conceptually abstract virtual functions in the base class
// so derived classes don't have to implement unused ones.

gkg::IOHandler::IOHandler()
{
}


gkg::IOHandler::~IOHandler()
{
}


int32_t gkg::IOHandler::isInputReady( int32_t )
{

  return -1;

}


int32_t gkg::IOHandler::isOutputReady( int32_t )
{

  return -1;

}


int32_t gkg::IOHandler::isExceptionRaised( int32_t )
{

  return -1;

}


void gkg::IOHandler::setTimerExpired( int64_t, int64_t )
{
}


void gkg::IOHandler::setChildStatus( pid_t, int32_t )
{
}
