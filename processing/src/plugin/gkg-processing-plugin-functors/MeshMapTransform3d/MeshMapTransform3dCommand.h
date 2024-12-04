#ifndef _gkg_processing_plugin_functors_MeshMapTransform3d_MeshMapTransform3dCommand_h_
#define _gkg_processing_plugin_functors_MeshMapTransform3d_MeshMapTransform3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MeshMapTransform3dCommand : public Command,
                                  public Creator2Arg< MeshMapTransform3dCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< MeshMapTransform3dCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    MeshMapTransform3dCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    MeshMapTransform3dCommand(
                         const std::string& fileNameMeshMap,
                         const std::string& fileNameOut,
                         const std::vector< std::string >& fileNameTransform3ds,
                         bool verbose );
    MeshMapTransform3dCommand( const Dictionary& parameters );
    virtual ~MeshMapTransform3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MeshMapTransform3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MeshMapTransform3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMeshMap,
                  const std::string& fileNameOut,
                  const std::vector< std::string >& fileNameTransform3ds,
                  bool verbose );

};


}


#endif
