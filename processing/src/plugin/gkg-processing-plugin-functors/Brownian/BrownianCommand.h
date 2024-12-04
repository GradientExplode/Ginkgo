#ifndef _gkg_processing_plugin_functors_Brownian_BrownianCommand_h_
#define _gkg_processing_plugin_functors_Brownian_BrownianCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class BrownianCommand : public Command,
                        public Creator2Arg< BrownianCommand,
                        		    Command,
                        		    int32_t,
                        		    char** >,
                        public Creator1Arg< BrownianCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    BrownianCommand( int32_t argc,
                     char* argv[], 
                     bool loadPlugin = false,
                     bool removeFirst = true );
    BrownianCommand( bool verbose );
    BrownianCommand( const Dictionary& parameters );
    virtual ~BrownianCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrownianCommand, Command, int32_t, char** >;
    friend class Creator1Arg< BrownianCommand, Command, const Dictionary& >;

    void parse();
    void execute( bool verbose );

};


}


#endif
