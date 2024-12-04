#ifndef _gkg_dmri_plugin_functors_DwiMultipleShellQSpaceSamplingSplitter_DwiMultipleShellQSpaceSamplingSplitterCommand_h_
#define _gkg_dmri_plugin_functors_DwiMultipleShellQSpaceSamplingSplitter_DwiMultipleShellQSpaceSamplingSplitterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiMultipleShellQSpaceSamplingSplitterCommand : public Command,
              public Creator2Arg< DwiMultipleShellQSpaceSamplingSplitterCommand,
                                  Command,
                                  int32_t,
                                  char** >,
              public Creator1Arg< DwiMultipleShellQSpaceSamplingSplitterCommand,
                                  Command,
                                  const Dictionary& >
{

  public:

    DwiMultipleShellQSpaceSamplingSplitterCommand(
                              int32_t argc,
                              char* argv[],
                              bool loadPlugin = false,
                              bool removeFirst = true );
    DwiMultipleShellQSpaceSamplingSplitterCommand(
                             const std::string& fileNameInputDW,
                             const std::string& fileNameOutputDW,
                             bool verbose );
    DwiMultipleShellQSpaceSamplingSplitterCommand(
                                                 const Dictionary& parameters );
    virtual ~DwiMultipleShellQSpaceSamplingSplitterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiMultipleShellQSpaceSamplingSplitterCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiMultipleShellQSpaceSamplingSplitterCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputDW,
                  const std::string& fileNameOutputDW,
                  bool verbose );

};


}


#endif
