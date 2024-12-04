#ifndef _gkg_processing_plugin_functors_RandomIndexGenerator_RandomIndexGeneratorCommand_h_
#define _gkg_processing_plugin_functors_RandomIndexGenerator_RandomIndexGeneratorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class RandomIndexGeneratorCommand :
                                public Command,
                                public Creator2Arg< RandomIndexGeneratorCommand,
                                         	    Command,
                                         	    int32_t,
                                         	    char** >,
                                public Creator1Arg< RandomIndexGeneratorCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    RandomIndexGeneratorCommand( int32_t argc,
                  		 char* argv[], 
                  		 bool loadPlugin = false,
                  		 bool removeFirst = true );
    RandomIndexGeneratorCommand( int32_t start, int32_t size );
    RandomIndexGeneratorCommand( const Dictionary& parameters );
    virtual ~RandomIndexGeneratorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RandomIndexGeneratorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< RandomIndexGeneratorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( int32_t start, int32_t size );

};


}


#endif
