#ifndef _gkg_processing_plugin_functors_MeshMapToObjAndMtlConverter_MeshMapToObjAndMtlConverterCommand_h_
#define _gkg_processing_plugin_functors_MeshMapToObjAndMtlConverter_MeshMapToObjAndMtlConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MeshMapToObjAndMtlConverterCommand :
                         public Command,
                         public Creator2Arg< MeshMapToObjAndMtlConverterCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< MeshMapToObjAndMtlConverterCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    MeshMapToObjAndMtlConverterCommand( int32_t argc,
                  			char* argv[], 		   
                  			bool loadPlugin = false,   
                  			bool removeFirst = true ); 
    MeshMapToObjAndMtlConverterCommand( const std::string& fileNameMeshMap,
                                        const std::string& fileNameObj,
                                        const std::string& fileNameMtl,
                                        bool verbose );
    MeshMapToObjAndMtlConverterCommand( const Dictionary& parameters );
    virtual ~MeshMapToObjAndMtlConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MeshMapToObjAndMtlConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MeshMapToObjAndMtlConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMeshMap,
                  const std::string& fileNameObj,
                  const std::string& fileNameMtl,
                  bool verbose );

};


}


#endif
