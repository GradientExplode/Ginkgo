#ifndef _gkg_dmri_plugin_functors_DwiGeometrySimulator_DwiGeometrySimulatorCommand_h_
#define _gkg_dmri_plugin_functors_DwiGeometrySimulator_DwiGeometrySimulatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiGeometrySimulatorCommand :
                                public Command,
                                public Creator2Arg< DwiGeometrySimulatorCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiGeometrySimulatorCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiGeometrySimulatorCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    DwiGeometrySimulatorCommand( const std::string& fileNameMembraneGeometry,
                                 const std::string& fileNameMembraneMesh,
                                 bool verbose );
    DwiGeometrySimulatorCommand( const Dictionary& parameters );
    virtual ~DwiGeometrySimulatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiGeometrySimulatorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiGeometrySimulatorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMembraneGeometry,
                  const std::string& fileNameMembraneMesh,
                  bool verbose );

};


}


#endif
