#ifndef _gkg_mri_reconstruction_plugin_functors_GehcSignaGenesis2GisConverter_GehcSignaGenesis2GisConverterCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcSignaGenesis2GisConverter_GehcSignaGenesis2GisConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcSignaGenesis2GisConverterCommand :
                       public Command,
                       public Creator2Arg< GehcSignaGenesis2GisConverterCommand,
                                           Command,
                                           int32_t,
                                           char** >,
                       public Creator1Arg< GehcSignaGenesis2GisConverterCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    GehcSignaGenesis2GisConverterCommand( int32_t argc,
                                          char* argv[],
                                          bool loadPlugin = false,
                                          bool removeFirst = true );
    GehcSignaGenesis2GisConverterCommand( const std::string& fileNameIn,
                                          const std::string& fileNameOut,
                                          const std::string& outputFormat,
                                          bool verbose );
    GehcSignaGenesis2GisConverterCommand( const Dictionary& parameters );
    virtual ~GehcSignaGenesis2GisConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcSignaGenesis2GisConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcSignaGenesis2GisConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
