#ifndef _gkg_graphics_X11_plugin_functors_Viewer2d_Viewer2dCommand_h_
#define _gkg_graphics_X11_plugin_functors_Viewer2d_Viewer2dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Viewer2dCommand : public Command,
                        public Creator2Arg< Viewer2dCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< Viewer2dCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    Viewer2dCommand( int32_t argc,
                     char* argv[], 
                     bool loadPlugin = false,
                     bool removeFirst = true );
    Viewer2dCommand( const std::string& fileNameIn,
                     float scale,
                     bool verbose,
                     bool doubleBuffering );
    Viewer2dCommand( const Dictionary& parameters );
    virtual ~Viewer2dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Viewer2dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Viewer2dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  float scale,
                  bool verbose,
                  bool doubleBuffering );

};


}


#endif
