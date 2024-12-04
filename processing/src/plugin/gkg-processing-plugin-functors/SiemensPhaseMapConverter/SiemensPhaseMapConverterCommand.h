#ifndef _gkg_processing_plugin_functors_SiemensPhaseMapConverter_SiemensPhaseMapConverterCommand_h_
#define _gkg_processing_plugin_functors_SiemensPhaseMapConverter_SiemensPhaseMapConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SiemensPhaseMapConverterCommand :
                            public Command,
                            public Creator2Arg< SiemensPhaseMapConverterCommand,
                                         	Command,
                                         	int32_t,
                                         	char** >,
                            public Creator1Arg< SiemensPhaseMapConverterCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    SiemensPhaseMapConverterCommand( int32_t argc,
                  		     char* argv[], 
                  		     bool loadPlugin = false,
                  		     bool removeFirst = true );
    SiemensPhaseMapConverterCommand( const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                                     const std::string& outputFormat,
                                     bool ascii,
                                     bool verbose );
    SiemensPhaseMapConverterCommand( const Dictionary& parameters );
    virtual ~SiemensPhaseMapConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SiemensPhaseMapConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SiemensPhaseMapConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
