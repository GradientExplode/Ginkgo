#ifndef _gkg_dmri_plugin_functors_DwiQSpaceSamplingRendering_DwiQSpaceSamplingRenderingCommand_h_
#define _gkg_dmri_plugin_functors_DwiQSpaceSamplingRendering_DwiQSpaceSamplingRenderingCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiQSpaceSamplingRenderingCommand :
                          public Command,
                          public Creator2Arg< DwiQSpaceSamplingRenderingCommand,
                                              Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< DwiQSpaceSamplingRenderingCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    DwiQSpaceSamplingRenderingCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin = false,
                                       bool removeFirst = true );
    DwiQSpaceSamplingRenderingCommand( const std::string& fileNameDW,
                                       const std::string& fileNameMesh,
                                       float sphereRadius,
                                       int32_t sphereVertexCount,
                                       bool symmetrical,
                                       const std::string& meshFormat,
                                       bool ascii,
                                       bool verbose );
    DwiQSpaceSamplingRenderingCommand( const Dictionary& parameters );
    virtual ~DwiQSpaceSamplingRenderingCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiQSpaceSamplingRenderingCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiQSpaceSamplingRenderingCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameDW,
                  const std::string& fileNameMesh,
                  float sphereRadius,
                  int32_t sphereVertexCount,
                  bool symmetrical,
                  const std::string& meshFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
