#ifndef _gkg_dmri_plugin_functors_DwiChangeRGBBackgroundColor_DwiChangeRGBBackgroundColorCommand_h_
#define _gkg_dmri_plugin_functors_DwiChangeRGBBackgroundColor_DwiChangeRGBBackgroundColorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiChangeRGBBackgroundColorCommand : 
                         public Command,
                         public Creator2Arg< DwiChangeRGBBackgroundColorCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< DwiChangeRGBBackgroundColorCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    DwiChangeRGBBackgroundColorCommand( int32_t argc,
                                        char* argv[],
                                        bool loadPlugin = false,
                                        bool removeFirst = true );
    DwiChangeRGBBackgroundColorCommand( const std::string& fileNameRGB,
                                        const std::string& fileNameMaskOfSignal,
                                        const std::string& fileNameOutput,
                                        const std::vector< int32_t >& rgb,
                                        bool verbose );
    DwiChangeRGBBackgroundColorCommand( const Dictionary& parameters );
    virtual ~DwiChangeRGBBackgroundColorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiChangeRGBBackgroundColorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiChangeRGBBackgroundColorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameRGB,
                  const std::string& fileNameMaskOfSignal,
                  const std::string& fileNameOutput,
                  const std::vector< int32_t >& rgb,
                  bool verbose );

};


}


#endif
