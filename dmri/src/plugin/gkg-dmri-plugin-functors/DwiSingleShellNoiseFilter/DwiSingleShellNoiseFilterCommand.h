#ifndef _gkg_dmri_plugin_functors_DwiSingleShellNoiseFilter_DwiSingleShellNoiseFilterCommand_h_
#define _gkg_dmri_plugin_functors_DwiSingleShellNoiseFilter_DwiSingleShellNoiseFilterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiSingleShellNoiseFilterCommand :
                           public Command,
                           public Creator2Arg< DwiSingleShellNoiseFilterCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< DwiSingleShellNoiseFilterCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    DwiSingleShellNoiseFilterCommand( int32_t argc,
                                      char* argv[],
                                      bool loadPlugin = false,
                                      bool removeFirst = true );
    DwiSingleShellNoiseFilterCommand( const std::string& fileNameT2,
                                      const std::string& fileNameDW,
                                      const std::string& fileNameMaskOfSignal,
                                      const std::string& fileNameMaskOfNoise,
                                      const std::string& fileNameDenoisedDW,
                                      const std::string& fileNameCorrectedMask,
                                      float normalizedNoiseStdDev,
                                      float lambda,
                                      int32_t maximumSHOrder,
                                      const std::string& optimizerType,
                                      int32_t maximumIterationCount,
                                      double stepSize,
                                      double maximumTestGradient,
                                      double maximumTolerance,
                                      float deltaParameter,
                                      float radius,
                                      float thresholdRatio,
                                      bool intermediate,
                                      const std::string& removeNegativeValues,
                                      bool verbose );
    DwiSingleShellNoiseFilterCommand( const Dictionary& parameters );
    virtual ~DwiSingleShellNoiseFilterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiSingleShellNoiseFilterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiSingleShellNoiseFilterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMaskOfSignal,
                  const std::string& fileNameMaskOfNoise,
                  const std::string& fileNameDenoisedDW,
                  const std::string& fileNameCorrectedMask,
                  float normalizedNoiseStdDev,
                  float lambda,
                  int32_t maximumSHOrder,
                  const std::string& optimizerType,
                  int32_t maximumIterationCount,
                  double stepSize,
                  double maximumTestGradient,
                  double maximumTolerance,
                  float deltaParameter,
                  float radius,
                  float thresholdRatio,
                  bool intermediate,
                  const std::string& removeNegativeValues,
                  bool verbose );

};


}


#endif
