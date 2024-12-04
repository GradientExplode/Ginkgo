#include <gkg-communication-thread/ThreadConditionImplementation.h>
#include <iostream>



gkg::ThreadConditionImplementation::ThreadConditionImplementation(
                                         gkg::ThreadCondition* threadCondition )
                                   : _target( threadCondition )
{
}


gkg::ThreadConditionImplementation::~ThreadConditionImplementation()
{
}


gkg::ThreadCondition* gkg::ThreadConditionImplementation::getTarget() const
{

  return _target;

}
