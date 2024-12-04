#ifndef _gkg_processing_plugin_functors_TextureMapMasker_TextureMapMaskerCommand_h_
#define _gkg_processing_plugin_functors_TextureMapMasker_TextureMapMaskerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class TextureMapMaskerCommand : public Command,
                                public Creator2Arg< TextureMapMaskerCommand,
                                                    Command,
                                         	    int32_t,
                                         	    char** >,
                                public Creator1Arg< TextureMapMaskerCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    TextureMapMaskerCommand( int32_t argc,
                 	     char* argv[], 
                 	     bool loadPlugin = false,
                 	     bool removeFirst = true );
    TextureMapMaskerCommand( const std::string& fileNameInputTextureMap,
                             const std::string& fileNameMaskTextureMap,
                             const std::string& fileNameOutputTextureMap,
                             double background,
                             bool ascii,
                             const std::string& format,
                             bool verbose );
    TextureMapMaskerCommand( const Dictionary& parameters );
    virtual ~TextureMapMaskerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapMaskerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapMaskerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputTextureMap,
                  const std::string& fileNameMaskTextureMap,
                  const std::string& fileNameOutputTextureMap,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
