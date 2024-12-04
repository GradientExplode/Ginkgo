#ifndef _gkg_processing_plugin_functors_VolumeAverager_VolumeAveragerCommand_h_
#define _gkg_processing_plugin_functors_VolumeAverager_VolumeAveragerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class VolumeAveragerCommand : public Command,
                              public Creator2Arg< VolumeAveragerCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< VolumeAveragerCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    VolumeAveragerCommand( int32_t argc,
                  	   char* argv[], 
                  	   bool loadPlugin = false,
                  	   bool removeFirst = true );
    VolumeAveragerCommand( const std::string& fileNameIn,
                           const std::string& fileNameOut,
                           const std::string& axis,
                           const std::vector< int32_t >& range,
                           bool ascii,
                           const std::string& format,
                           bool verbose );
    VolumeAveragerCommand( const Dictionary& parameters );
    virtual ~VolumeAveragerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeAveragerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VolumeAveragerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& axis,
                  const std::vector< int32_t >& range,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
