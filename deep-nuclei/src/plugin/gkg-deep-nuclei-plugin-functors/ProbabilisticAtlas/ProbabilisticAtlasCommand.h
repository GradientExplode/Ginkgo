#ifndef _gkg_deep_nuclei_plugin_functors_ProbabilisticAtlas_ProbabilisticAtlasCommand_h_
#define _gkg_deep_nuclei_plugin_functors_ProbabilisticAtlas_ProbabilisticAtlasCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class ProbabilisticAtlasCommand : public Command,
                                  public Creator2Arg< ProbabilisticAtlasCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< ProbabilisticAtlasCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    ProbabilisticAtlasCommand( int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    ProbabilisticAtlasCommand( const std::string& atlasFileName,
                               const std::string& fileNameOut,
                               bool dilate,
                               float dilationRadius,
                               int16_t occurencyPercentThreshold,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    ProbabilisticAtlasCommand( const Dictionary& parameters );
    virtual ~ProbabilisticAtlasCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< ProbabilisticAtlasCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< ProbabilisticAtlasCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& atlasFileName,
                  const std::string& fileNameOut,
                  bool dilate,
                  float dilationRadius,
                  int16_t occurencyPercentThreshold,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
