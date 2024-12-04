#ifndef _gkg_processing_plugin_functors_BrukerPhaseMapConverter_BrukerPhaseMapConverterCommand_h_
#define _gkg_processing_plugin_functors_BrukerPhaseMapConverter_BrukerPhaseMapConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerPhaseMapConverterCommand :
                             public Command,
                             public Creator2Arg< BrukerPhaseMapConverterCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< BrukerPhaseMapConverterCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    BrukerPhaseMapConverterCommand( int32_t argc,
                              	    char* argv[], 
                              	    bool loadPlugin = false,
                              	    bool removeFirst = true );
    BrukerPhaseMapConverterCommand( const std::string& fileNameEcho1Real,
                                    const std::string& fileNameEcho1Imag,
                                    const std::string& fileNameEcho2Real,
                                    const std::string& fileNameEcho2Imag,
                                    const std::string& fileNameB0Phase,
                                    const std::string& fileNameB0Magnitude,
                                    bool verbose );
    BrukerPhaseMapConverterCommand( const Dictionary& parameters );
    virtual ~BrukerPhaseMapConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerPhaseMapConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerPhaseMapConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameEcho1Real,
                  const std::string& fileNameEcho1Imag,
                  const std::string& fileNameEcho2Real,
                  const std::string& fileNameEcho2Imag,
                  const std::string& fileNameB0Phase,
                  const std::string& fileNameB0Magnitude,
                  bool verbose );

};


}


#endif
