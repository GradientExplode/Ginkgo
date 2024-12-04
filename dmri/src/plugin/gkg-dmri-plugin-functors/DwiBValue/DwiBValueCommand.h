#ifndef _gkg_dmri_plugin_functors_DwiBValue_DwiBValueCommand_h_
#define _gkg_dmri_plugin_functors_DwiBValue_DwiBValueCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBValueCommand : public Command,
                         public Creator2Arg< DwiBValueCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< DwiBValueCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    DwiBValueCommand( int32_t argc,
                      char* argv[],
                      bool loadPlugin = false,
                      bool removeFirst = true );
    DwiBValueCommand( double maximuGradientAmplitude,
                      double minimumSlewRate,
                      double gradientResolution,
                      double timeSeparation,
                      double timeBeforeEcho,
                      double bValue,
                      double B0,
                      double ADC,
                      double averageT2 );
    DwiBValueCommand( const Dictionary& parameters );
    virtual ~DwiBValueCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBValueCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBValueCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( double maximuGradientAmplitude,
                  double minimumSlewRate,
                  double gradientResolution,
                  double timeSeparation,
                  double timeBeforeEcho,
                  double bValue,
                  double B0,
                  double ADC,
                  double averageT2 );

};


}


#endif
