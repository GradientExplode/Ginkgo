#ifndef _gkg_mri_reconstruction_plugin_functors_SmsRaw2GisConverter_SmsRaw2GisConverterCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_SmsRaw2GisConverter_SmsRaw2GisConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SmsRaw2GisConverterCommand :
                                 public Command,
                                 public Creator2Arg< SmsRaw2GisConverterCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< SmsRaw2GisConverterCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    SmsRaw2GisConverterCommand( int32_t argc,
                                char* argv[],
                                bool loadPlugin = false,
                                bool removeFirst = true );
    SmsRaw2GisConverterCommand( const std::string& fileNameIn,
                                const std::string& fileNameOut,
                                const std::string& outputFormat,
                                const std::string& fileNameMag,
                                const std::string& fileNamePha,
                                bool verbose );
    SmsRaw2GisConverterCommand( const Dictionary& parameters );
    virtual ~SmsRaw2GisConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SmsRaw2GisConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SmsRaw2GisConverterCommand, Command,
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
