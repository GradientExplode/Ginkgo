#ifndef _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_Viewer2dRpcServerCommand_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2dRpcServer_Viewer2dRpcServerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Viewer2dRpcServerCommand : public Command,
                                 public Creator2Arg< Viewer2dRpcServerCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< Viewer2dRpcServerCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    Viewer2dRpcServerCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    Viewer2dRpcServerCommand( int32_t rpcPort,
                              int32_t windowX,
                              int32_t windowY,
                              bool doRgb,
                              bool rainbowPalette,
                              bool verbose,
                              bool doubleBuffering );
    Viewer2dRpcServerCommand( const Dictionary& parameters );
    virtual ~Viewer2dRpcServerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Viewer2dRpcServerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Viewer2dRpcServerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( int32_t rpcPort,
                  int32_t windowX,
                  int32_t windowY,
                  bool doRgb,
                  bool rainbowPalette,
                  bool verbose,
                  bool doubleBuffering );

};


}


#endif
