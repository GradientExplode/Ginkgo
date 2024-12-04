#ifndef _gkg_simulation_plugin_functors_DwiGradientShapeGenerator_DwiGradientShapeGeneratorCommand_h_
#define _gkg_simulation_plugin_functors_DwiGradientShapeGenerator_DwiGradientShapeGeneratorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class DwiGradientShapeGeneratorCommand :
                           public Command,
                           public Creator2Arg< DwiGradientShapeGeneratorCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< DwiGradientShapeGeneratorCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    DwiGradientShapeGeneratorCommand( int32_t argc,
                        char* argv[],
                        bool loadPlugin = false,
                        bool removeFirst = true );
    DwiGradientShapeGeneratorCommand(
                  const std::string& fileNameMRISequenceInputDictionary,
                  const std::string&
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                  float timeStepInUs,
                  bool verbose );
    DwiGradientShapeGeneratorCommand( const Dictionary& parameters );
    virtual ~DwiGradientShapeGeneratorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiGradientShapeGeneratorCommand,
                              Command,
                              int32_t, char** >;
    friend class Creator1Arg< DwiGradientShapeGeneratorCommand,
                              Command,
                              const Dictionary& >;

    void parse();

    void execute( const std::string& fileNameMRISequenceInputDictionary,
                  const std::string&
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                  float timeStepInUs,
                  bool verbose );

};


}


#endif
