#include <gkg-graphics-X11-X11/X11RequestErrorImplementation.h>
#include <gkg-graphics-X11-core/RequestError.h>


//
// static local variables
//

static gkg::RequestError* errorHandler = 0;

//
// static functions
//

static int32_t doX11Error( XDisplay* errorXDisplay, XErrorEvent* xErrorEvent )
{

  gkg::RequestError* requestError = errorHandler;
  if ( requestError )
  {

    gkg::X11RequestErrorImplementation* x11RequestErrorImpl =
    gkg::X11RequestErrorImplementation::getImplementation( requestError );

    x11RequestErrorImpl->setMessageId( xErrorEvent->serial );
    x11RequestErrorImpl->setCode( xErrorEvent->error_code );
    x11RequestErrorImpl->setRequest( xErrorEvent->request_code );
    x11RequestErrorImpl->setDetail( xErrorEvent->minor_code );
    x11RequestErrorImpl->setId( ( char* )xErrorEvent->resourceid );
    XGetErrorText( errorXDisplay,
                   x11RequestErrorImpl->getCode(),
                   ( char* )x11RequestErrorImpl->getMessage().c_str(),
                   x11RequestErrorImpl->getMessage().length() );
    requestError->error();

  }
  return 0;

}


//
// class X11RequestErrorImplementation
//

gkg::X11RequestErrorImplementation::X11RequestErrorImplementation(
                                               gkg::RequestError* requestError )
                               : gkg::RequestErrorImplementation( requestError )
{

  // no constructor code currently necessary

}


gkg::X11RequestErrorImplementation::~X11RequestErrorImplementation()
{

  if ( errorHandler == _target )
  {

    errorHandler = 0;

  }

}


gkg::RequestError* gkg::X11RequestErrorImplementation::install()
{

  if ( !errorHandler )
  {

    XSetErrorHandler( doX11Error );

  }
  gkg::RequestError* requestError = errorHandler;
  errorHandler = _target;
  return requestError;

}


void gkg::X11RequestErrorImplementation::error()
{

  // default is to do nothing 

}


void 
gkg::X11RequestErrorImplementation::setMessageId( uint32_t messageId )
{

  _messageId = messageId;

}


uint32_t gkg::X11RequestErrorImplementation::getMessageId() const
{

  return _messageId;

}


void gkg::X11RequestErrorImplementation::setCode( int32_t code )
{

  _code = code;

}


int32_t gkg::X11RequestErrorImplementation::getCode() const
{

  return _code;

}


void gkg::X11RequestErrorImplementation::setRequest( int32_t request )
{

  _request = request;

}


int32_t gkg::X11RequestErrorImplementation::getRequest() const
{

  return _request;

}


void gkg::X11RequestErrorImplementation::setDetail( int32_t detail )
{

  _detail = detail;

}


int32_t gkg::X11RequestErrorImplementation::getDetail() const
{

  return _detail;

}


void gkg::X11RequestErrorImplementation::setId( char* id )
{

  _id = id;

}


char* gkg::X11RequestErrorImplementation::getId() const
{

  return _id;

}


void 
gkg::X11RequestErrorImplementation::setMessage( const std::string& message )
{

  _message = message;

}


const std::string& gkg::X11RequestErrorImplementation::getMessage() const
{

  return _message;

}


gkg::X11RequestErrorImplementation* 
gkg::X11RequestErrorImplementation::getImplementation( 
                                               gkg::RequestError* requestError )
{

  return static_cast< gkg::X11RequestErrorImplementation* >(
                                            requestError->getImplementation() );

}

