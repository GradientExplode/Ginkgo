#ifndef _gkg_processing_plugin_functors_GehcPhaseMapConverter_GehcPhaseMapConverterCommand_h_
#define _gkg_processing_plugin_functors_GehcPhaseMapConverter_GehcPhaseMapConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcPhaseMapConverterCommand :
                               public Command,
                               public Creator2Arg< GehcPhaseMapConverterCommand,
                                         	   Command,
                                         	   int32_t,
                                         	   char** >,
                               public Creator1Arg< GehcPhaseMapConverterCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    GehcPhaseMapConverterCommand( int32_t argc,
                  		  char* argv[], 
                  		  bool loadPlugin = false,
                  		  bool removeFirst = true );
    GehcPhaseMapConverterCommand( const std::string& fileNameIn,
                                  const std::string& fileNameOut,
                                  const std::string& outputFormat,
                                  bool ascii,
                                  bool verbose );
    GehcPhaseMapConverterCommand( const Dictionary& parameters );
    virtual ~GehcPhaseMapConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcPhaseMapConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcPhaseMapConverterCommand, Command,
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
