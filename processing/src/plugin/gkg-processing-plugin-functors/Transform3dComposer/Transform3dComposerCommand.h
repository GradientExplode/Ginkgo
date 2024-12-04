#ifndef _gkg_processing_plugin_functors_Transform3dComposer_Transform3dComposerCommand_h_
#define _gkg_processing_plugin_functors_Transform3dComposer_Transform3dComposerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class Transform3dComposerCommand :
                                 public Command,
                                 public Creator2Arg< Transform3dComposerCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< Transform3dComposerCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    Transform3dComposerCommand( int32_t argc,
                                char* argv[],
                                bool loadPlugin = false,
                                bool removeFirst = true );
    Transform3dComposerCommand( const std::vector< std::string >& fileNameIns,
                                const std::string& fileNameOut,
                                bool verbose );
    Transform3dComposerCommand( const Dictionary& parameters );
    virtual ~Transform3dComposerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Transform3dComposerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Transform3dComposerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameIns,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
