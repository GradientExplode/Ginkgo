#ifndef _gkg_dmri_plugin_functors_MeshMapSlicer_MeshMapSlicerCommand_h_
#define _gkg_dmri_plugin_functors_MeshMapSlicer_MeshMapSlicerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MeshMapSlicerCommand : public Command,
                             public Creator2Arg< MeshMapSlicerCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< MeshMapSlicerCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    MeshMapSlicerCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    MeshMapSlicerCommand( const std::string& fileNameMeshMap,
                          const std::string& fileNameOut,
                          const std::string& bundleMapFormat,
                          float epsilon,
                          float minimumThickness,
                          float maximumThickness,
                          bool adaptive,
                          bool clockwise,
                          bool verbose );
    MeshMapSlicerCommand( const Dictionary& parameters );
    virtual ~MeshMapSlicerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MeshMapSlicerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MeshMapSlicerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMeshMap,
                  const std::string& fileNameOut,
                  const std::string& bundleMapFormat,
                  float epsilon,
                  float minimumThickness,
                  float maximumThickness,
                  bool adaptive,
                  bool clockwise,
                  bool verbose );

};


}


#endif
