#ifndef _gkg_dmri_plugin_functors_DwiDiskFormatConverter_DwiDiskFormatConverterCommand_h_
#define _gkg_dmri_plugin_functors_DwiDiskFormatConverter_DwiDiskFormatConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiDiskFormatConverterCommand :
                              public Command,
                              public Creator2Arg< DwiDiskFormatConverterCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< DwiDiskFormatConverterCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DwiDiskFormatConverterCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    DwiDiskFormatConverterCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& inputFormat,
                                   const std::string& outputFormat,
                                   bool ascii,
                                   bool verbose );
    DwiDiskFormatConverterCommand( const Dictionary& parameters );
    virtual ~DwiDiskFormatConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiDiskFormatConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiDiskFormatConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& inputFormat,
                  const std::string& outputFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif