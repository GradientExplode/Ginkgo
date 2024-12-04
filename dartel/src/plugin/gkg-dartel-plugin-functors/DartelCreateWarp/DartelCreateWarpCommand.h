#ifndef _gkg_dartel_plugin_functors_DartelCreateWarp_DartelCreateWarpCommand_h_
#define _gkg_dartel_plugin_functors_DartelCreateWarp_DartelCreateWarpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DartelCreateWarpCommand : public Command,
                                public Creator2Arg< DartelCreateWarpCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DartelCreateWarpCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DartelCreateWarpCommand( int32_t argc,
                             char* argv[], 
                             bool loadPlugin = false,
                             bool removeFirst = true );
    DartelCreateWarpCommand( const std::string& fileNameIn,
                             const std::string& fileNameFlowField,
                             const std::string& fileNameOut,
                             const std::string& fileNameTemplate,
                             int32_t k,
                             int32_t degree,
                             bool inverse,
                             bool verbose );
    DartelCreateWarpCommand( const Dictionary& parameters );
    virtual ~DartelCreateWarpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DartelCreateWarpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DartelCreateWarpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameFlowField,
                  const std::string& fileNameOut,
                  const std::string& fileNameTemplate,
                  int32_t k,
                  int32_t degree,
                  bool inverse,
    		  bool verbose );

};


}


#endif
