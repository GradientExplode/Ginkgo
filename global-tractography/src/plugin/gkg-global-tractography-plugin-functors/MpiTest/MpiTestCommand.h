#ifndef _gkg_hpc_plugin_functors_mpitestcommand_h_
#define _gkg_hpc_plugin_functors_mpitestcommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MpiTestCommand : public Command,
                   public Creator2Arg< MpiTestCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< MpiTestCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    MpiTestCommand( int32_t argc,
                char* argv[], 
                bool loadPlugin = false,
                bool removeFirst = true );
    MpiTestCommand();
    MpiTestCommand( const Dictionary& parameters );
    virtual ~MpiTestCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MpiTestCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MpiTestCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute();

};


}


#endif