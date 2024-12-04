#ifndef _gkg_processing_plugin_functors_Minf2SomaConverter_Minf2SomaConverterCommand_h_
#define _gkg_processing_plugin_functors_Minf2SomaConverter_Minf2SomaConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Minf2SomaConverterCommand : public Command,
                                  public Creator2Arg< Minf2SomaConverterCommand,
                                         	      Command,
                                         	      int32_t,
                                         	      char** >,
                                  public Creator1Arg< Minf2SomaConverterCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    Minf2SomaConverterCommand( int32_t argc,
                 	       char* argv[], 
                 	       bool loadPlugin = false,
                 	       bool removeFirst = true );
    Minf2SomaConverterCommand( const std::string& fileNameIn, bool verbose );
    Minf2SomaConverterCommand( const Dictionary& parameters );
    virtual ~Minf2SomaConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Minf2SomaConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Minf2SomaConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn, bool verbose );

};


}


#endif
