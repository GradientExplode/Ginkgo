#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerReco2GisConverter_BrukerReco2GisConverterCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerReco2GisConverter_BrukerReco2GisConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerReco2GisConverterCommand :
                             public Command,
                             public Creator2Arg< BrukerReco2GisConverterCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< BrukerReco2GisConverterCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    BrukerReco2GisConverterCommand( int32_t argc,
                                    char* argv[],
                                    bool loadPlugin = false,
                                    bool removeFirst = true );
    BrukerReco2GisConverterCommand( const std::string& fileNameIn,
                                    const std::string& fileNameOut,
                                    const std::string& outputFormat,
                                    bool verbose );
    BrukerReco2GisConverterCommand( const Dictionary& parameters );
    virtual ~BrukerReco2GisConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerReco2GisConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerReco2GisConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
