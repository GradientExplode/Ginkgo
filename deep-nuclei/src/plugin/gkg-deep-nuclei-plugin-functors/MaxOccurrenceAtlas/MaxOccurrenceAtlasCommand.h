#ifndef _gkg_deep_nuclei_plugin_functors_MaxOccurrenceAtlas_MaxOccurrenceAtlasCommand_h_
#define _gkg_deep_nuclei_plugin_functors_MaxOccurrenceAtlas_MaxOccurrenceAtlasCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class MaxOccurrenceAtlasCommand : public Command,
                                  public Creator2Arg< MaxOccurrenceAtlasCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< MaxOccurrenceAtlasCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    MaxOccurrenceAtlasCommand( int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    MaxOccurrenceAtlasCommand( const std::string& atlasFileName,
                               const std::string& fileNameOut,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    MaxOccurrenceAtlasCommand( const Dictionary& parameters );
    virtual ~MaxOccurrenceAtlasCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MaxOccurrenceAtlasCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MaxOccurrenceAtlasCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& atlasFileName,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
