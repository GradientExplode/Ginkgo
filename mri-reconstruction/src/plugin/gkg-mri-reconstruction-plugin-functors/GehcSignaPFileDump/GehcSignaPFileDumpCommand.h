#ifndef _gkg_mri_reconstruction_plugin_functors_GehcSignaPFileDump_GehcSignaPFileDumpCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcSignaPFileDump_GehcSignaPFileDumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcSignaPFileDumpCommand : public Command,
                                  public Creator2Arg< GehcSignaPFileDumpCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< GehcSignaPFileDumpCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    GehcSignaPFileDumpCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    GehcSignaPFileDumpCommand( const std::string& fileNameIn,
                               const std::string& fileNameOut,
                               bool printOption,
                               const std::string& tagName );
    GehcSignaPFileDumpCommand( const Dictionary& parameters );
    virtual ~GehcSignaPFileDumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcSignaPFileDumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcSignaPFileDumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool printOption,
                  const std::string& tagName );

};


}


#endif
