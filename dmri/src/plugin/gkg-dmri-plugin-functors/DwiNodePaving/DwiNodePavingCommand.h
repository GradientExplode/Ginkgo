#ifndef _gkg_dmri_plugin_functors_DwiNodePaving_DwiNodePavingCommand_h_
#define _gkg_dmre_plugin_functors_DwiNodePaving_DwiNodePavingCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-core-object/Dictionary.h>

#include <string>


namespace gkg
{


class DwiNodePavingCommand : public Command,
                             public Creator2Arg< DwiNodePavingCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< DwiNodePavingCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DwiNodePavingCommand( int32_t argc,
                          char* argv[],
                          bool loadPlugin = false,
                          bool removeFirst = true );
    DwiNodePavingCommand( const std::string& fileNameMask,
                          const std::string& outputDirectory,
                          std::string neighborhoodStrategy,
                          float distance,
                          int32_t targetPartCount,
                          float loadImbalanceTolerance,
                          int32_t runCount,
                          bool totalCommunicationMinimization,
                          bool verbose );
    DwiNodePavingCommand( const Dictionary& parameters );
    virtual ~DwiNodePavingCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiNodePavingCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiNodePavingCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMask,
                  const std::string& outputDirectory,
                  std::string neighborhoodStrategy,
                  float distance,
                  int32_t targetPartCount,
                  float loadImbalanceTolerance,
                  int32_t runCount,
                  bool totalCommunicationMinimization,
                  bool verbose );

};


}


#endif
