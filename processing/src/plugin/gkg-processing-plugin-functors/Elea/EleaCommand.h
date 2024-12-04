#ifndef _gkg_processing_plugin_functors_Elea_EleaCommand_h_
#define _gkg_processing_plugin_functors_Elea_EleaCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class EleaCommand : public Command,
                    public Creator2Arg< EleaCommand,
                                        Command,
                                        int32_t,
                                        char** >,
                    public Creator1Arg< EleaCommand,
                                        Command,
                                        const Dictionary& >
{

  public:

    EleaCommand( int32_t argc,
                 char* argv[], 
                 bool loadPlugin = false,
                 bool removeFirst = true );
    EleaCommand( const std::string& fileNameIn,
                 const std::string& fileNameMask,
                 const std::string& fileNameOut,
                 bool ascii,
                 const std::string& format,
                 bool verbose );
    EleaCommand( const Dictionary& parameters );
    virtual ~EleaCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< EleaCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< EleaCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameMask,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
