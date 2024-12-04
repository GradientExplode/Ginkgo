#ifndef _gkg_processing_plugin_functors_SupportVectorMachineTrainer_SupportVectorMachineTrainerCommand_h_
#define _gkg_processing_plugin_functors_SupportVectorMachineTrainer_SupportVectorMachineTrainerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SupportVectorMachineTrainerCommand : 
                         public Command,
                         public Creator2Arg< SupportVectorMachineTrainerCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< SupportVectorMachineTrainerCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    SupportVectorMachineTrainerCommand( int32_t argc,
                                        char* argv[],
                                        bool loadPlugin = false,
                                        bool removeFirst = true );
    SupportVectorMachineTrainerCommand( const std::string& fileNameData,
                                        const std::string& fileNameLabels,
                                        const std::string& fileNameWeights,
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
    SupportVectorMachineTrainerCommand( const Dictionary& parameters );
    virtual ~SupportVectorMachineTrainerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SupportVectorMachineTrainerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SupportVectorMachineTrainerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameData,
                  const std::string& fileNameLabels,
                  const std::string& fileNameWeights,
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
