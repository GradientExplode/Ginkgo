#ifndef _gkg_processing_plugin_functors_TextureMapBinarizer_TextureMapBinarizerCommand_h_
#define _gkg_processing_plugin_functors_TextureMapBinarizer_TextureMapBinarizerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class TextureMapBinarizerCommand :
                                 public Command,
                     		 public Creator2Arg< TextureMapBinarizerCommand,
                     		   		     Command,
                     		   		     int32_t,
                     		   		     char** >,
                                 public Creator1Arg< TextureMapBinarizerCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    TextureMapBinarizerCommand( int32_t argc,
                  		char* argv[], 
                  		bool loadPlugin = false,
                  		bool removeFirst = true );
    TextureMapBinarizerCommand( const std::string& fileNameInputTextureMap,
                                const std::string& fileNameInputMeshMap,
                                const std::string& fileNameOutputTextureMap,
                                const std::string& fileNameOutputSurface,
                                const std::string& outType,
                                const std::string& mode,
                                double threshold1,
                                double threshold2,
                                double background,
                                double foreground,
                                bool ascii,
                                const std::string& format,
                                bool verbose );
    TextureMapBinarizerCommand( const Dictionary& parameters );
    virtual ~TextureMapBinarizerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapBinarizerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapBinarizerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputTextureMap,
                  const std::string& fileNameInputMeshMap,
                  const std::string& fileNameOutputTextureMap,
                  const std::string& fileNameOutputSurface,
                  const std::string& outType,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  double background,
                  double foreground,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
