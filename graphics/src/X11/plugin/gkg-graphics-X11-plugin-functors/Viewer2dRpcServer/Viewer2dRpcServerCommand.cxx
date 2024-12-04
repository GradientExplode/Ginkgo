#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/Viewer2dRpcServerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-rpc/RpcCommandReader.h>
#include <gkg-processing-rpc/RpcTransferVolumeCallback_i.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/Viewer2d.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcCommandId.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcExitCallback.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcResetCallback.h>
#include <gkg-graphics-X11-plugin-functors/Viewer2dRpcServer/RpcUpdateCallback.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cstdlib>


//
// Viewer service
//


class ViewerService : public gkg::SocketService
{

  public:

    ViewerService( int32_t port, bool doRgb, bool rainbowPalette );
    ~ViewerService();
 
    gkg::Viewer2d* getViewer2d();
   
  protected:
  
    void createReader( int32_t fd );
    
  private:
  
    gkg::RpcCommandReader* _reader;
    gkg::Viewer2d* _viewer;

};


ViewerService::ViewerService( int32_t port, bool doRgb, bool rainbowPalette )
              : gkg::SocketService( port ),
	        _reader( NULL )
{

  _viewer = new gkg::Viewer2d( doRgb, rainbowPalette );

}


ViewerService::~ViewerService()
{

  delete _reader;
  delete _viewer;
  
}


void ViewerService::createReader( int32_t fd )
{

  _reader = new gkg::RpcCommandReader( fd );
  
  if ( _reader )
  {
  
    gkg::RpcResetCallback* resetCbk = 
      new gkg::RpcResetCallback( gkg::RESET_VIEWER, ( void* )_viewer );
    gkg::RpcUpdateCallback* updateCbk = 
      new gkg::RpcUpdateCallback( gkg::UPDATE_VIEWER, ( void* )_viewer );
    gkg::RpcExitCallback* exitCbk = 
      new gkg::RpcExitCallback( gkg::EXIT_VIEWER, NULL );
    gkg::RpcTransferVolumeCallback< int16_t >* volInt16Cbk =
      new gkg::RpcTransferVolumeCallback< int16_t >( 
                                                    gkg::TRANSFER_VOLUME_INT16,
                                                    _viewer->getAverageVolume(),
						    ( void* )_viewer );
    gkg::RpcTransferVolumeCallback< float >* volFloatCbk =
      new gkg::RpcTransferVolumeCallback< float >( 
                                                 gkg::TRANSFER_VOLUME_FLOAT,
                                                 _viewer->getActivationVolume(),
                                                 ( void* )_viewer );

     gkg::RpcTransferVolumeCallback< gkg::RGBComponent >* volRGBCbk =
      new gkg::RpcTransferVolumeCallback< gkg::RGBComponent >( 
                                                       gkg::TRANSFER_VOLUME_RGB,
                                                       _viewer->getRGBVolume(),
                                                       ( void* )_viewer );
 
    _reader->add( resetCbk );
    _reader->add( updateCbk );
    _reader->add( exitCbk );
    _reader->add( volInt16Cbk );
    _reader->add( volFloatCbk );
    _reader->add( volRGBCbk );
   
  }

}


gkg::Viewer2d* ViewerService::getViewer2d()
{

  return _viewer;
  
}


//
//   Viewer2dRpcServerCommand
//


gkg::Viewer2dRpcServerCommand::Viewer2dRpcServerCommand( int32_t argc,
                                 			 char* argv[],
                                 			 bool loadPlugin,
                                 			 bool removeFirst )
                             : gkg::Command( argc, argv,
                                             loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Viewer2dRpcServerCommand::Viewer2dRpcServerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Viewer2dRpcServerCommand::Viewer2dRpcServerCommand( int32_t rpcPort,
                              				 int32_t windowX,
                              				 int32_t windowY,
                              				 bool doRgb,
                              				 bool rainbowPalette,
                              				 bool verbose,
                              				 bool doubleBuffering )
                             : gkg::Command()
{

  try
  {

    execute( rpcPort, windowX, windowY, doRgb, rainbowPalette, verbose,
             doubleBuffering );

  }
  GKG_CATCH( "gkg::Viewer2dRpcServerCommand::Viewer2dRpcServerCommand( "
             "int32_t rpcPort, int32_t windowX, int32_t windowY, "
             "bool doRgb, bool rainbowPalette, bool verbose, "
             "bool doubleBuffering )" );

}


gkg::Viewer2dRpcServerCommand::Viewer2dRpcServerCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rpcPort );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, windowX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, windowY );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, doRgb );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, rainbowPalette );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, doubleBuffering );

    execute( rpcPort, windowX, windowY, doRgb, rainbowPalette, verbose,
             doubleBuffering );

  }
  GKG_CATCH( "gkg::Viewer2dCommand::Viewer2dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Viewer2dRpcServerCommand::~Viewer2dRpcServerCommand()
{
}


std::string gkg::Viewer2dRpcServerCommand::getStaticName()
{

  try
  {

    return "Viewer2dRpcServer";

  }
  GKG_CATCH( "std::string gkg::Viewer2dRpcServerCommand::getStaticName()" );

}


void gkg::Viewer2dRpcServerCommand::parse()
{

  try
  {

    int32_t rpcPort = 8001;
    int32_t windowX = 512;
    int32_t windowY = 512;
    bool doRgb = false;
    bool rainbowPalette = false;
    bool verbose = false;
    bool doubleBuffering = false;
  
    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "2D Viewer", _loadPlugin );
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
    application.addSingleOption( "-rgb",
                                 "RGB map",
                                 doRgb,
                                 true );
    application.addSingleOption( "-rainbow",
                                 "Use rainbow palette",
                                 rainbowPalette,
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

    execute( rpcPort, windowX, windowY, doRgb, rainbowPalette, verbose,
             doubleBuffering );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Viewer2dRpcServerCommand::parse()" );

}


void gkg::Viewer2dRpcServerCommand::execute( int32_t rpcPort,
                             		     int32_t windowX,
                             		     int32_t windowY,
                             		     bool doRgb,
                             		     bool rainbowPalette,
                             		     bool verbose,
                             		     bool /* doubleBuffering */ )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // RPC section
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "starting RPC session : " << std::flush;

    }

    gkg::Session& session = gkg::Session::getInstance();
    session.initialize( "GkgTestViewer2dServer02", _argc, _argv );
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "starting RPC service : " << std::flush;

    }

    ViewerService* service = new ViewerService( rpcPort, 
                                                doRgb,
                                                rainbowPalette );

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

    session.runWindow( aWindow );

    delete service;

  }
  GKG_CATCH( "void gkg::Viewer2dRpcServerCommand::execute( "
             "int32_t rpcPort, int32_t windowX, int32_t windowY, "
             "bool doRgb, bool rainbowPalette, bool verbose, "
             "bool doubleBuffering )" );

}


RegisterCommandCreator(
    Viewer2dRpcServerCommand,
    DECLARE_INTEGER_PARAMETER_HELP( rpcPort ) +
    DECLARE_INTEGER_PARAMETER_HELP( windowX ) +
    DECLARE_INTEGER_PARAMETER_HELP( windowY ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( doRgb ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( rainbowPalette ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( doubleBuffering ) );
