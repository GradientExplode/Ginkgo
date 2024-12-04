#ifndef _gkg_processing_plugin_functors_Flipper_FlipperCommand_h_
#define _gkg_processing_plugin_functors_Flipper_FlipperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class FlipperCommand : public Command,
                       public Creator2Arg< FlipperCommand,
                        		   Command,
                        		   int32_t,
                        		   char** >,
                       public Creator1Arg< FlipperCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    FlipperCommand( int32_t argc,
                    char* argv[], 
                    bool loadPlugin = false,
                    bool removeFirst = true );
    FlipperCommand( const std::string& fileNameIn,
                    const std::string& fileNameOut,
                    const std::vector< std::string >& stringTypes,
                    bool ascii,
                    const std::string& format,
                    bool verbose );
    FlipperCommand( const Dictionary& parameters );
    virtual ~FlipperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< FlipperCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< FlipperCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::vector< std::string >& stringTypes,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
