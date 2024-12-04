#ifndef _gkg_processing_plugin_functors_SplitDwData_SplitDwDataCommand_h_
#define _gkg_processing_plugin_functors_SplitDwData_SplitDwDataCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class SplitDwDataCommand : public Command,
                           public Creator2Arg< SplitDwDataCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< SplitDwDataCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    SplitDwDataCommand( int32_t argc,
                        char* argv[],		   
                        bool loadPlugin = false,   
                        bool removeFirst = true ); 
    SplitDwDataCommand( const std::vector< std::string >& fileNameIn,
                        const std::vector< std::string >& fileNameWeight,
                        const std::string& fileNamePC,
                        const std::string& fileNameT2,
                        const std::string& fileNameMultiT2,
                        const std::string& fileNameDW,
                        bool ascii,
                        const std::string& outputFormat,
                        bool verbose );
    SplitDwDataCommand( const Dictionary& parameters );
    virtual ~SplitDwDataCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SplitDwDataCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SplitDwDataCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameIn,
                  const std::vector< std::string >& fileNameWeight,
                  const std::string& fileNamePC,
                  const std::string& fileNameT2,
                  const std::string& fileNameMultiT2,
                  const std::string& fileNameDW,
                  bool ascii,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
