#ifndef _gkg_mri_reconstruction_plugin_functors_GehcSignaGenesis2Dump_GehcSignaGenesis2DumpCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcSignaGenesis2Dump_GehcSignaGenesis2DumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcSignaGenesis2DumpCommand :
                               public Command,
                               public Creator2Arg< GehcSignaGenesis2DumpCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< GehcSignaGenesis2DumpCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    GehcSignaGenesis2DumpCommand( int32_t argc,
                                  char* argv[],
                                  bool loadPlugin = false,
                                  bool removeFirst = true );
    GehcSignaGenesis2DumpCommand( const std::string& fileNameIn,
                                  const std::string& fileNameOut );
    GehcSignaGenesis2DumpCommand( const Dictionary& parameters );
    virtual ~GehcSignaGenesis2DumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcSignaGenesis2DumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcSignaGenesis2DumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut );

};


}


#endif
