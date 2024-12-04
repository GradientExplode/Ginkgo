#ifndef _gkg_processing_plugin_functors_CheckFloatVolume_CheckFloatingVolumeCommand_h_
#define _gkg_processing_plugin_functors_CheckFloatVolume_CheckFloatingVolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class CheckFloatingVolumeCommand : public Command,
                                   public Creator2Arg<
                                                     CheckFloatingVolumeCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                   public Creator1Arg<
                                                     CheckFloatingVolumeCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    CheckFloatingVolumeCommand( int32_t argc,
                                char* argv[], 
                                bool loadPlugin = false,
                                bool removeFirst = true );
    CheckFloatingVolumeCommand( const std::string& fileNameIn,
                                bool verbose );
    CheckFloatingVolumeCommand( const Dictionary& parameters );
    virtual ~CheckFloatingVolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CheckFloatingVolumeCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< CheckFloatingVolumeCommand, Command, 
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  bool verbose );

};


}


#endif
