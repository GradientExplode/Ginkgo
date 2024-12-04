#ifndef _gkg_processing_plugin_functors_SupportVectorMachinePredicterForVolume_SupportVectorMachinePredicterForVolumeCommand_h_
#define _gkg_processing_plugin_functors_SupportVectorMachinePredicterForVolume_SupportVectorMachinePredicterForVolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SupportVectorMachinePredicterForVolumeCommand : 
              public Command,
              public Creator2Arg< SupportVectorMachinePredicterForVolumeCommand,
                                  Command,
                                  int32_t,
                                  char** >,
              public Creator1Arg< SupportVectorMachinePredicterForVolumeCommand,
                                  Command,
                                  const Dictionary& >
{

  public:

    SupportVectorMachinePredicterForVolumeCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin = false,
                                                   bool removeFirst = true );
    SupportVectorMachinePredicterForVolumeCommand(
                                       const std::string& fileNameData,
                                       const std::string& fileNameMask,
                                       const std::string& fileNameModel,
                                       const std::string& fileNameProbabilities,
                                       const std::string& fileNameOut,
                                       const std::string& svmTypeString,
                                       const std::string& kernelTypeString,
                                       int32_t degree,
                                       double gamma,
                                       double coef0,
                                       double cost,
                                       double nu,
                                       double epsilonSVR,
                                       double epsilonTermination,
                                       int32_t cacheSizeMB,
                                       bool shrinkingBoolean,
                                       bool verbose );
    SupportVectorMachinePredicterForVolumeCommand(
                                                 const Dictionary& parameters );
    virtual ~SupportVectorMachinePredicterForVolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SupportVectorMachinePredicterForVolumeCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SupportVectorMachinePredicterForVolumeCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameData,
                  const std::string& fileNameMask,
                  const std::string& fileNameModel,
                  const std::string& fileNameProbabilities,
                  const std::string& fileNameOut,
                  const std::string& svmTypeString,
                  const std::string& kernelTypeString,
                  int32_t degree,
                  double gamma,
                  double coef0,
                  double cost,
                  double nu,
                  double epsilonSVR,
                  double epsilonTermination,
                  int32_t cacheSizeMB,
                  bool shrinkingBoolean,
                  bool verbose );

};


}


#endif
