#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerRaw2GisConverter_BrukerRaw2GisConverterCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerRaw2GisConverter_BrukerRaw2GisConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerRaw2GisConverterCommand :
                              public Command,
                              public Creator2Arg< BrukerRaw2GisConverterCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< BrukerRaw2GisConverterCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    BrukerRaw2GisConverterCommand( int32_t argc,
                                  char* argv[],
                                  bool loadPlugin = false,
                                  bool removeFirst = true );
    BrukerRaw2GisConverterCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& outputFormat,
                                   const std::string& fileNameMag,
                                   const std::string& fileNamePha,
                                   bool verbose );
    BrukerRaw2GisConverterCommand( const Dictionary& parameters );
    virtual ~BrukerRaw2GisConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerRaw2GisConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerRaw2GisConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  const std::string& fileNameMag,
                  const std::string& fileNamePha,
                  bool verbose );

};


}


#endif
