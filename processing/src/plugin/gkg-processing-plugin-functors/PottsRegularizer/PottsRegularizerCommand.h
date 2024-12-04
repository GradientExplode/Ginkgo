#ifndef _gkg_processing_plugin_functors_PottsRegularizer_PottsRegularizerCommand_h_
#define _gkg_processing_plugin_functors_PottsRegularizer_PottsRegularizerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class PottsRegularizerCommand : public Command,
                                public Creator2Arg< PottsRegularizerCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< PottsRegularizerCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    PottsRegularizerCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin = false,
                             bool removeFirst = true );
    PottsRegularizerCommand( const std::string& fileNameIn,
                             const std::string& fileNameOut,
                             const std::string& fileNameOversampled,
                             const std::string& fileNameEnergy,
                             double oversamplingFactor,
                             double regularizationFactor,
                             const std::string& neighborhoodType,
                             const std::string& mode,
                             double threshold1,
                             double threshold2,
                             int32_t maximumIterationCount,
                             bool ascii,
                             const std::string& format,
                             bool verbose );
    PottsRegularizerCommand( const Dictionary& parameters );
    virtual ~PottsRegularizerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< PottsRegularizerCommand,
                              Command, int32_t, char** >;
    friend class Creator1Arg< PottsRegularizerCommand, 
                              Command, const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& fileNameOversampled,
                  const std::string& fileNameEnergy,
                  double oversamplingFactor,
                  double regularizationFactor,
                  const std::string& neighborhoodType,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  int32_t maximumIterationCount,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
