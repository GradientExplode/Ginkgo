#ifndef _gkg_deep_nuclei_plugin_functors_OccurrenceAtlas_OccurrenceAtlasCommand_h_
#define _gkg_deep_nuclei_plugin_functors_OccurrenceAtlas_OccurrenceAtlasCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class OccurrenceAtlasCommand : public Command,
                               public Creator2Arg< OccurrenceAtlasCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< OccurrenceAtlasCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    OccurrenceAtlasCommand( int32_t argc,
                            char* argv[], 
                            bool loadPlugin = false,
                            bool removeFirst = true );
    OccurrenceAtlasCommand( const std::vector< std::string >& labelFileNameIns,
                            const std::string& roiDictionaryFileName,
                            bool talairach,
                            const std::string& fileNameOut,
                            const std::string& fileNameRoiNameToRank,
                            bool ascii,
                            const std::string& format,
                            bool verbose );
    OccurrenceAtlasCommand( const Dictionary& parameters );
    virtual ~OccurrenceAtlasCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< OccurrenceAtlasCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< OccurrenceAtlasCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& labelFileNameIns,
                  const std::string& roiDictionaryFileName,
                  bool talairach,
                  const std::string& fileNameOut,
                  const std::string& fileNameRoiNameToRank,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
