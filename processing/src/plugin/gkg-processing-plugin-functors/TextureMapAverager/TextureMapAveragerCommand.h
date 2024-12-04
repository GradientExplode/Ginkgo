#ifndef _gkg_processing_plugin_functors_TextureMapAverager_TextureMapAveragerCommand_h_
#define _gkg_processing_plugin_functors_TextureMapAverager_TextureMapAveragerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class TextureMapAveragerCommand : public Command,
                                  public Creator2Arg< TextureMapAveragerCommand,
                                         	      Command,
                                         	      int32_t,
                                         	      char** >,
                                  public Creator1Arg< TextureMapAveragerCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    TextureMapAveragerCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    TextureMapAveragerCommand( 
                            const std::vector< std::string >& fileNameInputList,
                            const std::string& fileNameOutputTexture,
                            const std::string& mode,
                            bool ascii,
                            const std::string& format,
                            bool verbose );
    TextureMapAveragerCommand( const Dictionary& parameters );
    virtual ~TextureMapAveragerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapAveragerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapAveragerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameInputList,
                  const std::string& fileNameOutputTexture,
                  const std::string& mode,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
