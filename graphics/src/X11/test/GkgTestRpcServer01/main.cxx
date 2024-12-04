// RPC inter-process communication channel : receiver
// Open a terminal, and launch GkgRpcServer01
// Open a new terminal, and launch GkgRpcClient01


#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/Color.h>
#include <gkg-graphics-X11-core/Label.h>
#include <gkg-graphics-X11-core/Background.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>

#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcWriter.h>

#include <iostream>
#include <cstring>


//
// Reader
//

class Reader : public gkg::RpcReader
{

  public:

    Reader( int32_t fd, gkg::SocketService* service );
    virtual ~Reader();

  protected:

    virtual void connectionClosed( int32_t port );
    bool getBinary();

    static void receive( gkg::SocketReader*,
                         gkg::SocketHeader&,
                         gkg::SocketStream& );

    gkg::SocketService* _service;

};


Reader::Reader( int32_t fd, gkg::SocketService* service )
       : gkg::RpcReader( fd, 1, getBinary() ),
         _service( service )
{

  setProlog( "GkgTestRpcServer01" );
  setEpilog( "GkgTestRpcServer01" );
  addFunction( 0, &Reader::receive );

}


Reader::~Reader()
{
}


void Reader::connectionClosed( int32_t )
{

  _service->quitRunning();

}


bool Reader::getBinary()
{

  return true;

}


void Reader::receive( gkg::SocketReader*,
                      gkg::SocketHeader&,
                      gkg::SocketStream& rs )
{

  char text[ 80 ];
  rs >> text;

  if ( strcmp( text, "exit" ) == 0 ) 
  {

    std::cout << "Me GkgRpcServer01, I received : [" << text << "]"
              << std::endl;
    gkg::Session::getInstance().quit();

  }
  else
    std::cout << "Me GkgRpcServer01, I received : [" << text << "]"
              << std::endl;

}


//
// Service
//

class Service : public gkg::SocketService
{

  public:

    Service( int32_t port );
    virtual ~Service();

  protected:

    virtual void createReader( int32_t fd );

  protected:

    Reader* _reader;

};


Service::Service( int32_t port )
        : gkg::SocketService( port ),
          _reader( 0 )
{
}


Service::~Service()
{

  delete _reader;

}


void Service::createReader( int32_t fd )
{

  _reader = new Reader( fd, this );

}



int main( int argc, char* argv[] )
{

  Service* service = new Service( 8001 );

  gkg::Session& session = gkg::Session::getInstance();
  session.initialize( "GkgRpcServer01", argc, argv );

  gkg::WidgetFactory* 
    widgetFactory = gkg::LookAndFeel::getInstance().getWidgetFactory();

  const gkg::Color* color = widgetFactory->getBackground();
  gkg::Glyph* label = widgetFactory->createLabel( "hello world!" );

  gkg::Background* background = new gkg::Background( label, color );

  gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( background );

  int result = session.runWindow( aWindow );

  delete service;

  return result;

}
