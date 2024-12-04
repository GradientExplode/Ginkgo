#ifndef _gkg_dmri_plugin_functors_DwiOdfLMSDifference_DwiOdfLMSDifferenceCommand_h_
#define _gkg_dmri_plugin_functors_DwiOdfLMSDifference_DwiOdfLMSDifferenceCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiOdfLMSDifferenceCommand :
                                 public Command,
                                 public Creator2Arg< DwiOdfLMSDifferenceCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< DwiOdfLMSDifferenceCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DwiOdfLMSDifferenceCommand( int32_t argc,
                                char* argv[],
                                bool loadPlugin = false,
                                bool removeFirst = true );
    DwiOdfLMSDifferenceCommand( const std::string& fileNameOdfTextureMap1,
                                const std::string& fileNameOdfTextureMap2,
                                const std::string& fileNameSiteMap1,
                                const std::string& fileNameSiteMap2,
                                const std::string& fileNameLMSDifference,
                                const std::vector< int32_t >& sizes,
                                bool ascii,
                                const std::string& volumeFormat,
                                bool displaySitesAndValues,
                                bool verbose );
    DwiOdfLMSDifferenceCommand( const Dictionary& parameters );
    virtual ~DwiOdfLMSDifferenceCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiOdfLMSDifferenceCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiOdfLMSDifferenceCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameOdfTextureMap1,
                  const std::string& fileNameOdfTextureMap2,
                  const std::string& fileNameSiteMap1,
                  const std::string& fileNameSiteMap2,
                  const std::string& fileNameLMSDifference,
                  const std::vector< int32_t >& sizes,
                  bool ascii,
                  const std::string& volumeFormat,
                  bool displaySitesAndValues,
                  bool verbose  );

};


}


#endif
