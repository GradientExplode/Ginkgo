#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerRawDump_BrukerRawDumpCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerRawDump_BrukerRawDumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerRawDumpCommand : public Command,
                             public Creator2Arg< BrukerRawDumpCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< BrukerRawDumpCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    BrukerRawDumpCommand( int32_t argc,
                          char* argv[],
                          bool loadPlugin = false,
                          bool removeFirst = true );
    BrukerRawDumpCommand( const std::string& fileNameIn,
                          const std::string& fileNameOut );
    BrukerRawDumpCommand( const Dictionary& parameters );
    virtual ~BrukerRawDumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerRawDumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerRawDumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut );

};


}


#endif
