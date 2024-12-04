#ifndef _gkg_processing_plugin_functors_MeshMapSurfaceComputer_MeshMapSurfaceComputerCommand_h_
#define _gkg_processing_plugin_functors_MeshMapSurfaceComputer_MeshMapSurfaceComputerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MeshMapSurfaceComputerCommand :
                              public Command,
                              public Creator2Arg< MeshMapSurfaceComputerCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< MeshMapSurfaceComputerCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    MeshMapSurfaceComputerCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    MeshMapSurfaceComputerCommand(
                         const std::string& fileNameMeshMap,
                         const std::string& fileNameOut,
                         bool verbose );
    MeshMapSurfaceComputerCommand( const Dictionary& parameters );
    virtual ~MeshMapSurfaceComputerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MeshMapSurfaceComputerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MeshMapSurfaceComputerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMeshMap,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
