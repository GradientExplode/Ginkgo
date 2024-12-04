#ifndef _gkg_mri_reconstruction_plugin_functors_SmsRawDump_SmsRawDumpCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_SmsRawDump_SmsRawDumpCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SmsRawDumpCommand : public Command,
                          public Creator2Arg< SmsRawDumpCommand,
                                              Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< SmsRawDumpCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    SmsRawDumpCommand( int32_t argc,
                       char* argv[],
                       bool loadPlugin = false,
                       bool removeFirst = true );
    SmsRawDumpCommand( const std::string& fileNameIn,
                       const std::string& fileNameOut,
                       const std::string& tagName );
    SmsRawDumpCommand( const Dictionary& parameters );
    virtual ~SmsRawDumpCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SmsRawDumpCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SmsRawDumpCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& tagName );

};


}


#endif
