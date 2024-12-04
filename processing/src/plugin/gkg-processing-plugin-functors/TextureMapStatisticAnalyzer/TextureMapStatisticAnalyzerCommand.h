#ifndef _gkg_processing_plugin_functors_TextureMapStatisticAnalyzer_TextureMapStatisticAnalyzerCommand_h_
#define _gkg_processing_plugin_functors_TextureMapStatisticAnalyzer_TextureMapStatisticAnalyzerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class TextureMapStatisticAnalyzerCommand :
                         public Command,
                         public Creator2Arg< TextureMapStatisticAnalyzerCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< TextureMapStatisticAnalyzerCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    TextureMapStatisticAnalyzerCommand( int32_t argc,
                  			char* argv[], 
                  			bool loadPlugin = false,
                  			bool removeFirst = true );
    TextureMapStatisticAnalyzerCommand( 
                               const std::vector< std::string >& fileNameInputs,
                               const std::string& fileNameOutputTexture,
                               const std::string& fileNameRoiTexture,
                               const std::string& fileNameRoiPairs,
                               const std::string& fileNameCsvPyDistribution,
                               const std::string& mode,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    TextureMapStatisticAnalyzerCommand( const Dictionary& parameters );
    virtual ~TextureMapStatisticAnalyzerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TextureMapStatisticAnalyzerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TextureMapStatisticAnalyzerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameInputs,
                  const std::string& fileNameOutputTexture,
                  const std::string& fileNameRoiTexture,
                  const std::string& fileNameRoiPairs,
                  const std::string& fileNameCsvPyDistribution,
                  const std::string& mode,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
