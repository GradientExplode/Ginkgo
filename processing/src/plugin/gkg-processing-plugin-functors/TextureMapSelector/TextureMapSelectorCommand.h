#ifndef _gkg_processing_plugin_functors_TextureMapSelector_TextureMapSelectorCommand_h_
#define _gkg_processing_plugin_functors_TextureMapSelector_TextureMapSelectorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class TextureMapSelectorCommand : public Command,
                                  public Creator2Arg< TextureMapSelectorCommand,
                                        	      Command,
                                        	      int32_t,
                                        	      char** >,
                                  public Creator1Arg< TextureMapSelectorCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    TextureMapSelectorCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    TextureMapSelectorCommand( const std::string& fileNameInputTexture,
                               const std::string& fileNameOutputTexture,
                               const std::vector< double >& labels,
                               double background,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    TextureMapSelectorCommand( const Dictionary& parameters );
    virtual ~TextureMapSelectorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapSelectorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapSelectorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputTexture,
                  const std::string& fileNameOutputTexture,
                  const std::vector< double >& labels,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
