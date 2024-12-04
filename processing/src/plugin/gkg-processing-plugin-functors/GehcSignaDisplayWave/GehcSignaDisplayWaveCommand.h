#ifndef _gkg_processing_plugin_functors_GehcSignaDisplayWave_GehcSignaDisplayWaveCommand_h_
#define _gkg_processing_plugin_functors_GehcSignaDisplayWave_GehcSignaDisplayWaveCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcSignaDisplayWaveCommand :
                                public Command,
                                public Creator2Arg< GehcSignaDisplayWaveCommand,
                                         	    Command,
                                         	    int32_t,
                                         	    char** >,
                                public Creator1Arg< GehcSignaDisplayWaveCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    GehcSignaDisplayWaveCommand( int32_t argc,
                  		 char* argv[], 
                  		 bool loadPlugin = false,
                  		 bool removeFirst = true );
    GehcSignaDisplayWaveCommand( const std::string& fileNameWaveForm,
                                 const std::string& fileNameOutTxt,
                                 bool verbose );
    GehcSignaDisplayWaveCommand( const Dictionary& parameters );
    virtual ~GehcSignaDisplayWaveCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcSignaDisplayWaveCommand, Command, 
                              int32_t, char** >;
   friend class Creator1Arg< GehcSignaDisplayWaveCommand, Command,
                             const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameWaveForm,
                  const std::string& fileNameOutTxt,
                  bool verbose );

};


}


#endif
