#ifndef _gkg_processing_plugin_functors_AddRoiStatisticsToProbabilisticAtlas_AddRoiStatisticsToProbabilisticAtlasCommand_h_
#define _gkg_processing_plugin_functors_AddRoiStatisticsToProbabilisticAtlas_AddRoiStatisticsToProbabilisticAtlasCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class AddRoiStatisticsToProbabilisticAtlasCommand :
                public Command,
                public Creator2Arg< AddRoiStatisticsToProbabilisticAtlasCommand,
                                    Command,
                                    int32_t,
                                    char** >,
                public Creator2Arg< AddRoiStatisticsToProbabilisticAtlasCommand,
                                    Command,
                                    const std::string&,
                                    const Dictionary& >
{

  public:

    AddRoiStatisticsToProbabilisticAtlasCommand( int32_t argc,
                                                 char* argv[], 
                                                 bool loadPlugin = false,
                                                 bool removeFirst = true );
    AddRoiStatisticsToProbabilisticAtlasCommand(
                             const std::vector< std::string >& fileNameContrats,
                             const std::vector< std::string >& fileNameRois,
                             const std::string& fileNameAtlas,
                             const std::string& fileNameDictionary,
                             const std::string& fileNameOut,
                             const std::string& dictionaryName,
                             const std::string& contrastName,
                             bool verbose );
    AddRoiStatisticsToProbabilisticAtlasCommand( const Dictionary& parameters);
    virtual ~AddRoiStatisticsToProbabilisticAtlasCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AddRoiStatisticsToProbabilisticAtlasCommand, 
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AddRoiStatisticsToProbabilisticAtlasCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameContrats,
                  const std::vector< std::string >& fileNameRois,
                  const std::string& fileNameAtlas,
                  const std::string& fileNameDictionary,
                  const std::string& fileNameOut,
                  const std::string& dictionaryName,
                  const std::string& contrastName,
                  bool verbose );

};


}


#endif
