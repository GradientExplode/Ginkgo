#ifndef _gkg_processing_plugin_functors_PhilipsPhaseMapConverter_PhilipsPhaseMapConverterCommand_h_
#define _gkg_processing_plugin_functors_PhilipsPhaseMapConverter_PhilipsPhaseMapConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class PhilipsPhaseMapConverterCommand :
                            public Command,
                            public Creator2Arg< PhilipsPhaseMapConverterCommand,
                                         	Command,
                                         	int32_t,
                                         	char** >,
                            public Creator1Arg< PhilipsPhaseMapConverterCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    PhilipsPhaseMapConverterCommand( int32_t argc,
                  		     char* argv[], 
                  		     bool loadPlugin = false,
                  		     bool removeFirst = true );
    PhilipsPhaseMapConverterCommand( const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                                     const std::string& outputFormat,
                                     bool ascii,
                                     bool verbose );
    PhilipsPhaseMapConverterCommand( const Dictionary& parameters );
    virtual ~PhilipsPhaseMapConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< PhilipsPhaseMapConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< PhilipsPhaseMapConverterCommand, Command,
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
