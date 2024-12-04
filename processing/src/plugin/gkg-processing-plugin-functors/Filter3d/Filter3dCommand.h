#ifndef _gkg_processing_plugin_functors_Filter3d_Filter3dCommand_h_
#define _gkg_processing_plugin_functors_Filter3d_Filter3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Filter3dCommand : public Command,
                        public Creator2Arg< Filter3dCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< Filter3dCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    Filter3dCommand( int32_t argc,
                     char* argv[], 
                     bool loadPlugin = false,
                     bool removeFirst = true );
    Filter3dCommand( const std::string& fileNameIn,
                     const std::string& fileNameOut,
                     const std::string& outType,
                     const std::string& filterType,
                     const std::string& fileNameMask,
                     const std::string& neighborhood,
                     bool ascii,
                     const std::string& format,
                     bool verbose );
    Filter3dCommand( const Dictionary& parameters );
    virtual ~Filter3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Filter3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Filter3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outType,
                  const std::string& filterType,
                  const std::string& fileNameMask,
                  const std::string& neighborhood,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
