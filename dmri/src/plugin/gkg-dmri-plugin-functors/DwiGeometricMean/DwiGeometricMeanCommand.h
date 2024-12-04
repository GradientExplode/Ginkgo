#ifndef _gkg_dmri_plugin_functors_DwiGeometricMean_DwiGeometricMeanCommand_h_
#define _gkg_dmri_plugin_functors_DwiGeometricMean_DwiGeometricMeanCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiGeometricMeanCommand : public Command,
                                public Creator2Arg< DwiGeometricMeanCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiGeometricMeanCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiGeometricMeanCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin = false,
                             bool removeFirst = true );
    DwiGeometricMeanCommand( const std::string& fileNameInput,
                             const std::string& fileNameOutput,
                             bool ascii,
                             const std::string& format,
                             bool verbose );
    DwiGeometricMeanCommand( const Dictionary& parameters );
    virtual ~DwiGeometricMeanCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiGeometricMeanCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiGeometricMeanCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInput,
                  const std::string& fileNameOutput,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
