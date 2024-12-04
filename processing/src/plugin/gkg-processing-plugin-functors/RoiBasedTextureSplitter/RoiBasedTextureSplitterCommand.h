#ifndef _gkg_processing_plugin_functors_RoiBasedTextureSplitter_RoiBasedTextureSplitterCommand_h_
#define _gkg_processing_plugin_functors_RoiBasedTextureSplitter_RoiBasedTextureSplitterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class RoiBasedTextureSplitterCommand :
                             public Command,
                             public Creator2Arg< RoiBasedTextureSplitterCommand,
                                         	 Command,
                                         	 int32_t,
                                         	 char** >,
                             public Creator1Arg< RoiBasedTextureSplitterCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    RoiBasedTextureSplitterCommand( int32_t argc,
                  		    char* argv[], 
                  		    bool loadPlugin = false,
                  		    bool removeFirst = true );
    RoiBasedTextureSplitterCommand( const std::string& fileNameTextureMap,
                                    const std::string& fileNameRois,
                                    const std::string& fileNameOut,
                                    bool ascii,
                                    const std::string& format,
                                    bool verbose );
    RoiBasedTextureSplitterCommand( const Dictionary& parameters );
    virtual ~RoiBasedTextureSplitterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RoiBasedTextureSplitterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< RoiBasedTextureSplitterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameTextureMap,
                  const std::string& fileNameRois,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
