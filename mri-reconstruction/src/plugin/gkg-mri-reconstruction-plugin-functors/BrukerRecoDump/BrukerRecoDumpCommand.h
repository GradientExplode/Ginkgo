#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerRecoDump_BrukerRecoDumpCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerRecoDump_BrukerRecoDumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerRecoDumpCommand : public Command,
                              public Creator2Arg< BrukerRecoDumpCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< BrukerRecoDumpCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    BrukerRecoDumpCommand( int32_t argc,
                           char* argv[],
                           bool loadPlugin = false,
                           bool removeFirst = true );
    BrukerRecoDumpCommand( const std::string& fileNameIn,
                           const std::string& fileNameOut );
    BrukerRecoDumpCommand( const Dictionary& parameters );
    virtual ~BrukerRecoDumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerRecoDumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerRecoDumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut );

};


}


#endif
