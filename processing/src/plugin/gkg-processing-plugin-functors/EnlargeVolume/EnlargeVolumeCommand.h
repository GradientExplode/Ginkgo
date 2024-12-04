#ifndef _gkg_processing_plugin_functors_EnlargeVolume_EnlargeVolumeCommand_h_
#define _gkg_processing_plugin_functors_EnlargeVolume_EnlargeVolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <list>


namespace gkg
{


class EnlargeVolumeCommand : public Command,
                             public Creator2Arg< EnlargeVolumeCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< EnlargeVolumeCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    EnlargeVolumeCommand( int32_t argc,
                          char* argv[], 
                          bool loadPlugin = false,
                          bool removeFirst = true );
    EnlargeVolumeCommand( const std::string& fileNameIn,
                          const std::string& fileNameOut, 
                          int32_t sx, int32_t sy, int32_t sz, int32_t st,
                          int32_t ex, int32_t ey, int32_t ez, int32_t et,
                          bool ascii,
                          const std::string& format,
                          bool verbose );
    EnlargeVolumeCommand( const Dictionary& parameters );
    virtual ~EnlargeVolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< EnlargeVolumeCommand, Command, int32_t, char** >;
    friend class Creator1Arg< EnlargeVolumeCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut, 
                  int32_t sx, int32_t sy, int32_t sz, int32_t st,
                  int32_t ex, int32_t ey, int32_t ez, int32_t et,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
