#ifndef _gkg_processing_plugin_functors_SlrRf_SlrRfCommand_h_
#define _gkg_processing_plugin_functors_SlrRf_SlrRfCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SlrRfCommand : public Command,
                     public Creator2Arg< SlrRfCommand,
                                         Command,
                                         int32_t,
                                         char** >,
                     public Creator1Arg< SlrRfCommand,
                                         Command,
                                         const Dictionary& >
{

  public:

    SlrRfCommand( int32_t argc,
                  char* argv[], 
                  bool loadPlugin = false,
                  bool removeFirst = true );
    SlrRfCommand( int32_t sampleCount,
                  double timeBandWidth,
                  double passBandRipple,
                  double stopbandRipple,
                  const std::string& pulseType,
                  const std::string& filterType,
                  const std::string& fileNameOut,
                  bool gnuplot,
                  bool postscript,
                  bool brukerFormat,
                  bool verbose );
    SlrRfCommand( const Dictionary& parameters );
    virtual ~SlrRfCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SlrRfCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SlrRfCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( int32_t sampleCount,
                  double timeBandWidth,
                  double passBandRipple,
                  double stopbandRipple,
                  const std::string& pulseType,
                  const std::string& filterType,
                  const std::string& fileNameOut,
                  bool gnuplot,
                  bool postscript,
                  bool brukerFormat,
                  bool verbose );

};


}


#endif
