#ifndef _gkg_processing_plugin_functors_SupportVectorMachinePredicter_SupportVectorMachinePredicterCommand_h_
#define _gkg_processing_plugin_functors_SupportVectorMachinePredicter_SupportVectorMachinePredicterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SupportVectorMachinePredicterCommand : 
                       public Command,
                       public Creator2Arg< SupportVectorMachinePredicterCommand,
                                           Command,
                                           int32_t,
                                           char** >,
                       public Creator1Arg< SupportVectorMachinePredicterCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    SupportVectorMachinePredicterCommand( int32_t argc,
                                        char* argv[],
                                        bool loadPlugin = false,
                                        bool removeFirst = true );
    SupportVectorMachinePredicterCommand(
                                       const std::string& fileNameData,
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
    SupportVectorMachinePredicterCommand( const Dictionary& parameters );
    virtual ~SupportVectorMachinePredicterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SupportVectorMachinePredicterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SupportVectorMachinePredicterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameData,
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
