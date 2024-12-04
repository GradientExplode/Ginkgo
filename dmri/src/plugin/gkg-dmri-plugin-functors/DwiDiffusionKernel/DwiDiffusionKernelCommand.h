#ifndef _gkg_dmri_plugin_functors_DwiDiffusionKernel_DwiDiffusionKernelCommand_h_
#define _gkg_dmri_plugin_functors_DwiDiffusionKernel_DwiDiffusionKernelCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiDiffusionKernelCommand : public Command,
                                  public Creator2Arg< DwiDiffusionKernelCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< DwiDiffusionKernelCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    DwiDiffusionKernelCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    DwiDiffusionKernelCommand( const std::string& fileNameT2,
                               const std::string& fileNameDW,
                               const std::string& fileNameMask,
                               const std::string& fileNameOutput,
                               const std::string& volumeFormat,
                               int32_t voxelCount,
                               float fractionalAnisotropyLowerThreshold,
                               float fractionalAnisotropyUpperThreshold,
                               float radius,
                               float thresholdRatio,
                               bool verbose,
                               bool ascii );
    DwiDiffusionKernelCommand( const Dictionary& parameters );
    virtual ~DwiDiffusionKernelCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiDiffusionKernelCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiDiffusionKernelCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::string& fileNameOutput,
                  const std::string& volumeFormat,
                  int32_t voxelCount,
                  float fractionalAnisotropyLowerThreshold,
                  float fractionalAnisotropyUpperThreshold,
                  float radius,
                  float thresholdRatio,
                  bool verbose,
                  bool ascii );

};


}


#endif
