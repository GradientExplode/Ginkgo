#ifndef _gkg_processing_plugin_functors_Cat_CatCommand_h_
#define _gkg_processing_plugin_functors_Cat_CatCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <vector>
#include <string>


namespace gkg
{


class CatCommand : public Command,
                   public Creator2Arg< CatCommand, Command, int32_t, char** >,
                   public Creator1Arg< CatCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    CatCommand( int32_t argc,
                char* argv[], 
                bool loadPlugin = false,
                bool removeFirst = true );
    CatCommand( const std::vector< std::string >& inputNames,
                const std::string& outputName,
                const std::string& type,
                bool ascii,
                const std::string& format,
                bool verbose );
    CatCommand( const Dictionary& parameters );
    virtual ~CatCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CatCommand, Command, int32_t, char** >;
    friend class Creator1Arg< CatCommand, Command, const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& inputNames,
                  const std::string& outputName,
                  const std::string& type,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
