#ifndef _gkg_processing_plugin_functors_Transform3dInverter_Transform3dInverterCommand_h_
#define _gkg_processing_plugin_functors_Transform3dInverter_Transform3dInverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Transform3dInverterCommand :
                                 public Command,
                                 public Creator2Arg< Transform3dInverterCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< Transform3dInverterCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    Transform3dInverterCommand( int32_t argc,
                                char* argv[],
                                bool loadPlugin = false,
                                bool removeFirst = true );
    Transform3dInverterCommand( const std::string& fileNameIn,
                                const std::string& fileNameOut,
                                bool verbose );
    Transform3dInverterCommand( const Dictionary& parameters );
    virtual ~Transform3dInverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Transform3dInverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Transform3dInverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
