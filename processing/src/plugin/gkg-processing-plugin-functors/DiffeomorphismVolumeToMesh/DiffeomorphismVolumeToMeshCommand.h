#ifndef _gkg_processing_plugin_functors_DiffeomorphismVolumeToMesh_DiffeomorphismVolumeToMeshCommand_h_
#define _gkg_processing_plugin_functors_DiffeomorphismVolumeToMesh_DiffeomorphismVolumeToMeshCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DiffeomorphismVolumeToMeshCommand :
                          public Command,
                          public Creator2Arg< DiffeomorphismVolumeToMeshCommand,
                                              Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< DiffeomorphismVolumeToMeshCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    DiffeomorphismVolumeToMeshCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin = false,
                                       bool removeFirst = true );
    DiffeomorphismVolumeToMeshCommand( const std::string& fileNameIn,
                                       const std::string& fileNameOut,
                                       float scale,
                                       bool ascii,
                                       const std::string& format,
                                       bool verbose );
    DiffeomorphismVolumeToMeshCommand( const Dictionary& parameters );
    virtual ~DiffeomorphismVolumeToMeshCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DiffeomorphismVolumeToMeshCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DiffeomorphismVolumeToMeshCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  float scale,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
