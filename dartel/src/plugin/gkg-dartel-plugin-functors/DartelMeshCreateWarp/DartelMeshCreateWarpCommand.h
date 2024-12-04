#ifndef _gkg_dartel_plugin_functors_DartelMeshCreateWarp_DartelMeshCreateWarpCommand_h_
#define _gkg_dartel_plugin_functors_DartelMeshCreateWarp_DartelMeshCreateWarpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DartelMeshCreateWarpCommand : public Command,
                                    public Creator2Arg<
                                                    DartelMeshCreateWarpCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                    public Creator1Arg<
                                                    DartelMeshCreateWarpCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DartelMeshCreateWarpCommand( int32_t argc,
                                 char* argv[], 
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    DartelMeshCreateWarpCommand( const std::string& fileNameIn,
                                 const std::string& fileNameInTemplate,
                                 const std::string& fileNameFlowField,
                                 const std::string& fileNameOut,
                                 const std::string& fileNameTemplate,
                                 int32_t k,
                                 bool inverse,
                                 bool verbose );
    DartelMeshCreateWarpCommand( const Dictionary& parameters );
    virtual ~DartelMeshCreateWarpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DartelMeshCreateWarpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DartelMeshCreateWarpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameInTemplate,
                  const std::string& fileNameFlowField,
                  const std::string& fileNameOut,
                  const std::string& fileNameTemplate,
                  int32_t k,
                  bool inverse,
    		  bool verbose );

};


}


#endif
