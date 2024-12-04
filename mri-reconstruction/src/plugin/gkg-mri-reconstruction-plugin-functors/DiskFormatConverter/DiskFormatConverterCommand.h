#ifndef _gkg_mri_reconstruction_plugin_functors_DiskFormatConverter_DiskFormatConverterCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_DiskFormatConverter_DiskFormatConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DiskFormatConverterCommand :
                                 public Command,
                                 public Creator2Arg< DiskFormatConverterCommand,
                                         	     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< DiskFormatConverterCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DiskFormatConverterCommand( int32_t argc,
                  		char* argv[], 
                  		bool loadPlugin = false,
                  		bool removeFirst = true );
    DiskFormatConverterCommand( const std::string& fileNameIn,
                                const std::string& fileNameOut,
                                const std::string& inputFormat,
                                const std::string& outputFormat,
                                bool ascii,
                                bool verbose );
    DiskFormatConverterCommand( const Dictionary& parameters );
    virtual ~DiskFormatConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DiskFormatConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DiskFormatConverterCommand, Command,
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
