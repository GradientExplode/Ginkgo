#ifndef _gkg_hpc_plugin_functors_mpilaunchcommand_h_
#define _gkg_hpc_plugin_functors_mpilaunchcommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MpiLaunchCommand : public Command,
                   public Creator2Arg< MpiLaunchCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< MpiLaunchCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    MpiLaunchCommand( int32_t argc,
                char* argv[], 
                bool loadPlugin = false,
                bool removeFirst = true );
    MpiLaunchCommand();
    MpiLaunchCommand( const Dictionary& parameters );
    virtual ~MpiLaunchCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MpiLaunchCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MpiLaunchCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute();

};


}


#endif