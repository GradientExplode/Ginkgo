#ifndef _gkg_dmri_plugin_functors_DwiOutlierFilter_DwiOutlierFilterCommand_h_
#define _gkg_dmri_plugin_functors_DwiOutlierFilter_DwiOutlierFilterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiOutlierFilterCommand : public Command,
                                public Creator2Arg< DwiOutlierFilterCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiOutlierFilterCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiOutlierFilterCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin = false,
                             bool removeFirst = true );
    DwiOutlierFilterCommand(
                        const std::string& fileNameT2,
                        const std::string& fileNameDW,
                        const std::string& fileNameMask,
                        const std::string& sliceAxis,
                        const std::string& fileNameCorrectedDW,
                        const std::string& fileNameMI,
                        const std::string& fileNameOutliers,
                        double outlierFactor,
                        float radius,
                        float thresholdRatio,
                        const std::vector< double >& outlierFilterParameters,
                        const std::string& volumeFormat,
                        bool ascii,
                        bool verbose );
    DwiOutlierFilterCommand( const Dictionary& parameters );
    virtual ~DwiOutlierFilterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiOutlierFilterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiOutlierFilterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::string& sliceAxis,
                  const std::string& fileNameCorrectedDW,
                  const std::string& fileNameMI,
                  const std::string& fileNameOutliers,
                  double outlierFactor,
                  float radius,
                  float thresholdRatio,
                  const std::vector< double >& outlierFilterParameters,
                  const std::string& volumeFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
