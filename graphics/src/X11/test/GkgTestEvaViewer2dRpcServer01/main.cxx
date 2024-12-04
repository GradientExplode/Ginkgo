#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include "Viewer2d.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>


class ViewerService;


class Reader : public gkg::RpcReader
{

  public:

    Reader( int32_t port, ViewerService* service );
    ~Reader();

    ViewerService* getService();

  protected:

    enum
    {

      STRING,
      INTEGER,
      CHAR_ARRAY,
      IMAGE,
      NFCNS

    };

    static void receiveString( gkg::SocketReader* reader,
                               gkg::SocketHeader& header,
                               gkg::SocketStream& stream );
    static void receiveInt( gkg::SocketReader* reader,
                            gkg::SocketHeader& header,
                            gkg::SocketStream& stream );
    static void receiveCharArray( gkg::SocketReader* reader,
                                  gkg::SocketHeader& header,
                                  gkg::SocketStream& stream );
    static void receiveImage( gkg::SocketReader* reader,
                              gkg::SocketHeader& header,
                              gkg::SocketStream& stream );

    void connectionClosed( int32_t fd );
    bool getBinary();

    ViewerService* _service;

};


class ViewerService : public gkg::SocketService 
{

  public:

    ViewerService( int32_t port );
    ~ViewerService();
 
    gkg::Viewer2d* getViewer2d();

  protected:

    void createReader( int32_t fd );

  protected:

    Reader* _reader;
    gkg::Viewer2d* _viewer;

};


//
// class Reader
//

// the reader is set to be blocking, because huge amount of memory may
// be transfered from client to server and none should be missed!

Reader::Reader( int32_t port, ViewerService* service )
       : gkg::RpcReader( port, getBinary(), false ),
         _service(service)
{

  setProlog( "EvaRpcWriter" );
  setEpilog( "EvaRpcWriter" );
  addFunction( STRING, &Reader::receiveString );
  addFunction( INTEGER, &Reader::receiveInt );
  addFunction( CHAR_ARRAY, &Reader::receiveCharArray );
  addFunction( IMAGE, &Reader::receiveImage );

}


Reader::~Reader()
{
}


ViewerService* Reader::getService()
{

  return _service;

}


void Reader::connectionClosed( int32_t )
{

  _service->quitRunning();

}


// The user must supply this routine.
void Callback( gkg::SocketReader*, char* text );


void Reader::receiveString( gkg::SocketReader* rpcReader,
                            gkg::SocketHeader&,
                            gkg::SocketStream& stream )
{

  char text[ 80 ];
  stream >> text;
  Callback( rpcReader, text );

}


void Reader::receiveInt( gkg::SocketReader*,
                         gkg::SocketHeader&,
                         gkg::SocketStream& stream )
{

  int32_t i;
  stream >> i;
  std::cerr << "receiveInt: " << i << std::endl;

}


void Reader::receiveCharArray( gkg::SocketReader*,
                               gkg::SocketHeader&,
                               gkg::SocketStream& stream )
{

  int32_t count = 0;
  stream >> count;

  std::cout << "count=" << count << " : ";

  char* array = new char[ count ];

  stream.readPackets( array, count );

  std::cout << "{"
            << std::setw( 3 ) << ( int32_t )array[ 0 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 1 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 2 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 3 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 4 ] << " "
            << std::setw( 3 ) << ( int32_t )array[ 5 ] << " "
            << " ... "
            << std::setw( 3 ) << ( int32_t )array[ count - 1 ] << " "
            << "}" << std::endl;

  delete [] array;

}


void Reader::receiveImage( gkg::SocketReader* rpcReader,
                           gkg::SocketHeader&,
                           gkg::SocketStream& stream )
{

  Reader* reader = static_cast< Reader* >( rpcReader );

  if ( reader )
  {

    int32_t count = 0, dimX, dimY, bpp;
    gkg::Viewer2d* viewer = reader->getService()->getViewer2d();

    stream >> dimX;
    stream >> dimY;
    stream >> bpp;

    count = dimX * dimY * bpp;

    viewer->getAverageVolume().reallocate( dimX, dimY, 1, 1 );
    viewer->getAverageVolume().getHeader().addAttribute( "resolutionX", 1.0 );
    viewer->getAverageVolume().getHeader().addAttribute( "resolutionY", 1.0 );
    viewer->getAverageVolume().getHeader().addAttribute( "resolutionZ", 1.0 );
    viewer->getAverageVolume().getHeader().addAttribute( "resolutionT", 1.0 );
    stream.readPackets( ( char* )&viewer->getAverageVolume()( 0 ), count );
    viewer->update();

  }

}


bool Reader::getBinary()
{

  return true;

}


//
// class Service
//

ViewerService::ViewerService( int32_t port )
              : gkg::SocketService( port ),
                _reader( NULL )
{

  _viewer = new gkg::Viewer2d;

}


ViewerService::~ViewerService()
{

  delete _reader;
  delete _viewer;

}


void ViewerService::createReader( int32_t fd )
{

  _reader = new Reader( fd, this );

}


gkg::Viewer2d* ViewerService::getViewer2d()
{

  return _viewer;
  
}


//
// callback
//

void Callback( gkg::SocketReader* rpcReader, char* msg )
{

  Reader* reader = static_cast< Reader* >( rpcReader );

  if ( reader )
  {

    if ( strcmp( msg, "exit" ) == 0 )
    {

      gkg::Session::getInstance().quit();

    }
    else if ( strcmp( msg, "reset" ) == 0 )
    {

      reader->getService()->getViewer2d()->reset( true );

    }
    else
    {

      std::cout << "msg=" << msg << std::endl;

    } 

  }

}


//
// main procedure
//
int main( int argc, char* argv[] ) 
{

  int32_t result = EXIT_SUCCESS;

  try
  {
  
    int32_t rpcPort = 8001;
    int32_t windowX = 512;
    int32_t windowY = 512;
    bool verbose = false;
    bool doubleBuffering = false;
  
    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( argc, argv, "2D Viewer for EVA functor" );
    application.addSingleOption( "-port",
                                 "Service Port number to listen on"
				 "(default=8001)",
                                 rpcPort,
				 true );
    application.addSingleOption( "-wx",
                                 "Window width"
				 "(default=512)",
                                 windowX,
				 true );
    application.addSingleOption( "-wy",
                                 "Window height"
				 "(default=512)",
                                 windowY,
				 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.addSingleOption( "-dbuf",
                                 "Double buffering",
                                 doubleBuffering,
                                 true );

    application.initialize();
  
    ////////////////////////////////////////////////////////////////////////////
    // RPC section
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "starting RPC session : " << std::flush;

    }

    gkg::Session& session = gkg::Session::getInstance();
    session.initialize( "GkgTestEvaViewer2dRpcServer01", argc, argv );
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "starting RPC service : " << std::flush;

    }

    ViewerService* service = new ViewerService( rpcPort );

    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

    gkg::MonoGlyph* 
      layoutFixed = layoutFactory.createFixed( service->getViewer2d(),
                                               windowX,
                                               windowY );

    gkg::ApplicationWindow* aWindow = new gkg::ApplicationWindow( layoutFixed );
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    result = session.runWindow( aWindow );

    delete service;
  
  }
  GKG_CATCH_COMMAND( result );
  
  return result;

}
