#include <gkg-communication-thread/Task.h>


gkg::Task::Task()
         : _id( 0 )
{
}


gkg::Task::~Task()
{
}


void gkg::Task::setId( int32_t id )
{

  _id = id;

}


int32_t gkg::Task::getId()
{

  return _id;

}
