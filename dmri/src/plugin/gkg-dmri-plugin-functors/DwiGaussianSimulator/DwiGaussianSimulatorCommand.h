#ifndef _gkg_dmri_plugin_functors_DwiGaussianSimulator_DwiGaussianSimulatorCommand_h_
#define _gkg_dmri_plugin_functors_DwiGaussianSimulator_DwiGaussianSimulatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiGaussianSimulatorCommand :
                                public Command,
                                public Creator2Arg< DwiGaussianSimulatorCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiGaussianSimulatorCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiGaussianSimulatorCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    DwiGaussianSimulatorCommand( const std::string& fileNameIn,
                                 const std::string& fileNameSignal,
                                 const std::string& fileNameOdf,
                                 const std::string& fileNameNormalizedOdf,
                                 const std::string& fileNameTexture,
                                 const std::string& fileNameOdfValue,
                                 int32_t modelIndex,
                                 double bValue,
                                 int32_t meshPointCount,
                                 double bigDelta,
                                 double littleDelta,
                                 double radius,
                                 double snr,
                                 const std::string& meshMapFormat,
                                 const std::string& textureMapFormat,
                                 bool ascii,
                                 bool verbose );
    DwiGaussianSimulatorCommand( const Dictionary& parameters );
    virtual ~DwiGaussianSimulatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiGaussianSimulatorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiGaussianSimulatorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameSignal,
                  const std::string& fileNameOdf,
                  const std::string& fileNameNormalizedOdf,
                  const std::string& fileNameTexture,
                  const std::string& fileNameOdfValue,
                  int32_t modelIndex,
                  double bValue,
                  int32_t meshPointCount,
                  double bigDelta,
                  double littleDelta,
                  double radius,
                  double snr,
                  const std::string& meshMapFormat,
                  const std::string& textureMapFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
