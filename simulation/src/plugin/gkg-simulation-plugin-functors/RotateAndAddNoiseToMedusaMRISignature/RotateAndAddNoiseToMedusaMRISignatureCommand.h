#ifndef _gkg_simulation_plugin_functors_RotateAndAddNoiseToMedusaMRISignature_RotateAndAddNoiseToMedusaMRISignatureCommand_h_
#define _gkg_simulation_plugin_functors_RotateAndAddNoiseToMedusaMRISignature_RotateAndAddNoiseToMedusaMRISignatureCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class RotateAndAddNoiseToMedusaMRISignatureCommand :
               public Command,
               public Creator2Arg< RotateAndAddNoiseToMedusaMRISignatureCommand,
                                   Command,
                                   int32_t,
                                   char** >,
               public Creator1Arg< RotateAndAddNoiseToMedusaMRISignatureCommand,
                                   Command,
                                   const Dictionary& >
{

  public:

    RotateAndAddNoiseToMedusaMRISignatureCommand( int32_t argc,
                                                  char* argv[],
                                                  bool loadPlugin = false,
                                                  bool removeFirst = true );
    RotateAndAddNoiseToMedusaMRISignatureCommand(
                            const std::string& fileNameInputMRISignature,
                            const std::string& directoryNameOutputMRISignatures,
                            int32_t rotationCount,
                            float noiseStandardDeviation,
                            bool verbose );
    RotateAndAddNoiseToMedusaMRISignatureCommand(
                                                 const Dictionary& parameters );
    virtual ~RotateAndAddNoiseToMedusaMRISignatureCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RotateAndAddNoiseToMedusaMRISignatureCommand,
                              Command,
                              int32_t, char** >;
    friend class Creator1Arg< RotateAndAddNoiseToMedusaMRISignatureCommand,
                              Command,
                              const Dictionary& >;

    void parse();

    void execute( const std::string& fileNameInputMRISignature,
                  const std::string& directoryNameOutputMRISignatures,
                  int32_t rotationCount,
                  float noiseStandardDeviation,
                  bool verbose );

};


}


#endif
