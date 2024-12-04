#include <gkg-graphics-X11-core/RequestErrorImplementation.h>
#include <gkg-graphics-X11-core/RequestError.h>


gkg::RequestErrorImplementation::RequestErrorImplementation(
                                               gkg::RequestError* requestError )
                                : _target( requestError )
{
}


gkg::RequestErrorImplementation::~RequestErrorImplementation()
{
}


gkg::RequestError* gkg::RequestErrorImplementation::getTarget() const
{

  return _target;

}
