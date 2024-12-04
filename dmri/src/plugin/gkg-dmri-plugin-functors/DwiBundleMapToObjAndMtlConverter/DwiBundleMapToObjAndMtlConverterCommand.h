#ifndef _gkg_dmri_plugin_functors_DwiBundleMapToObjAndMtlConverter_DwiBundleMapToObjAndMtlConverterCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapToObjAndMtlConverter_DwiBundleMapToObjAndMtlConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapToObjAndMtlConverterCommand :
                    public Command,
                    public Creator2Arg< DwiBundleMapToObjAndMtlConverterCommand,
                                        Command,
                                        int32_t,
                                        char** >,
                    public Creator1Arg< DwiBundleMapToObjAndMtlConverterCommand,
                                        Command,
                                        const Dictionary& >
{

  public:

    DwiBundleMapToObjAndMtlConverterCommand( int32_t argc,
                                             char* argv[],
                                             bool loadPlugin = false,
                                             bool removeFirst = true );
    DwiBundleMapToObjAndMtlConverterCommand(
                                           const std::string& fileNameBundleMap,
                                           const std::string& fileNameObj,
                                           const std::string& fileNameMtl,
                                           float step,
                                           float radius,
                                           int32_t vertexCountPerPlane,
                                           bool verbose );
    DwiBundleMapToObjAndMtlConverterCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapToObjAndMtlConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapToObjAndMtlConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapToObjAndMtlConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                  const std::string& fileNameObj,
                  const std::string& fileNameMtl,
                  float step,
                  float radius,
                  int32_t vertexCountPerPlane,
                  bool verbose );

};


}


#endif
