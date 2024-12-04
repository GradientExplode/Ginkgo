#ifndef _gkg_dmri_plugin_functors_Anonymizer_AnonymizerCommand_h_
#define _gkg_dmri_plugin_functors_Anonymizer_AnonymizerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class AnonymizerCommand : public Command,
                          public Creator2Arg< AnonymizerCommand,
                                              Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< AnonymizerCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    AnonymizerCommand( int32_t argc,
                       char* argv[],
                       bool loadPlugin = false,
                       bool removeFirst = true );
    AnonymizerCommand( const std::string& fileName,
                       const std::string& anonymousName,
                       bool verbose );
    AnonymizerCommand( const Dictionary& parameters );
    virtual ~AnonymizerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AnonymizerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< AnonymizerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileName,
                  const std::string& anonymousName,
                  bool verbose );

};


}


#endif
