#ifndef _gkg_dmri_plugin_functors_DwiQSpaceResampler_DwiQSpaceResamplerCommand_h_
#define _gkg_dmri_plugin_functors_DwiQSpaceResampler_DwiQSpaceResamplerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiQSpaceResamplerCommand : public Command,
                                  public Creator2Arg< DwiQSpaceResamplerCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< DwiQSpaceResamplerCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    DwiQSpaceResamplerCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    DwiQSpaceResamplerCommand(
                              const std::string& fileNameT2,
                              const std::string& fileNameDW,
                              const std::string& fileNameMask,
                              const std::string& fileNameResampledDW,
                              const std::string& fileNameInputOrientations,
                              double bValue,
                              const std::string& fileNameOutputOrientations,
                              int32_t outputOrientationCount,
                              const std::string& outputOrientationScheme,
                              const std::vector< double >& resamplingParameters,
                              float radius,
                              float thresholdRatio,
                              const std::string& volumeFormat,
                              bool ascii,
                              bool verbose );
    DwiQSpaceResamplerCommand( const Dictionary& parameters );
    virtual ~DwiQSpaceResamplerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiQSpaceResamplerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiQSpaceResamplerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::string& fileNameResampledDW,
                  const std::string& fileNameInputOrientations,
                  double bValue,
                  const std::string& fileNameOutputOrientations,
                  int32_t outputOrientationCount,
                  const std::string& outputOrientationScheme,
                  const std::vector< double >& resamplingParameters,
                  float radius,
                  float thresholdRatio,
                  const std::string& volumeFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
