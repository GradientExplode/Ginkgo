#ifndef _gkg_mri_reconstruction_plugin_functors_GehcSignaGenesisDump_GehcSignaGenesisDumpCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcSignaGenesisDump_GehcSignaGenesisDumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcSignaGenesisDumpCommand :
                                public Command,
                                public Creator2Arg< GehcSignaGenesisDumpCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< GehcSignaGenesisDumpCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    GehcSignaGenesisDumpCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    GehcSignaGenesisDumpCommand( const std::string& fileNameIn,
                                 const std::string& fileNameOut );
    GehcSignaGenesisDumpCommand( const Dictionary& parameters );
    virtual ~GehcSignaGenesisDumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcSignaGenesisDumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcSignaGenesisDumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut );

};


}


#endif
