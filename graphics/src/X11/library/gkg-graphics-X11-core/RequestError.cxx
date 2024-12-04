#include <gkg-graphics-X11-core/RequestError.h>
#include <gkg-graphics-X11-core/RequestErrorImplementation.h>
#include <gkg-graphics-X11-core/GUIImplementationFactory.h>


gkg::RequestError::RequestError()
{

  _requestErrorImplementation =
  gkg::getGUIImplementationFactory()->createRequestErrorImplementation( this );

}


gkg::RequestError::~RequestError()
{

  delete _requestErrorImplementation;

}


gkg::RequestError* gkg::RequestError::install()
{

  return _requestErrorImplementation->install();

}


void gkg::RequestError::error()
{

  _requestErrorImplementation->error();

}


gkg::RequestErrorImplementation* gkg::RequestError::getImplementation() const
{

  return _requestErrorImplementation;

}
