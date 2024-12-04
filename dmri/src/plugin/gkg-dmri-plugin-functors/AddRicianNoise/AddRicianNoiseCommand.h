#ifndef _gkg_dmri_plugin_functors_AddRicianNoise_AddRicianNoiseCommand_h_
#define _gkg_dmri_plugin_functors_AddRicianNoise_AddRicianNoiseCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class AddRicianNoiseCommand : public Command,
                              public Creator2Arg< AddRicianNoiseCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< AddRicianNoiseCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    AddRicianNoiseCommand( int32_t argc,
                           char* argv[], 
                           bool loadPlugin = false,
                           bool removeFirst = true );
    AddRicianNoiseCommand( const std::string& fileNameIn,
                           const std::string& fileNameOut,
                           double noiseStdDev,
                           bool verbose );
    AddRicianNoiseCommand( const Dictionary& parameters );
    virtual ~AddRicianNoiseCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< AddRicianNoiseCommand, Command, int32_t, char** >;
    friend class Creator1Arg< AddRicianNoiseCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  double noiseStdDev,
                  bool verbose );

};


}


#endif
