#ifndef _gkg_processing_plugin_functors_DoubleVolumeResolution_DoubleVolumeResolutionCommand_h_
#define _gkg_processing_plugin_functors_DoubleVolumeResolution_DoubleVolumeResolutionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DoubleVolumeResolutionCommand :
                              public Command,
                              public Creator2Arg< DoubleVolumeResolutionCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< DoubleVolumeResolutionCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DoubleVolumeResolutionCommand( int32_t argc,
                  	   char* argv[], 
                  	   bool loadPlugin = false,
                  	   bool removeFirst = true );
    DoubleVolumeResolutionCommand( const std::string& fileNameIn,
                           const std::string& fileNameOut,
                           bool ascii,
                           const std::string& format,
                           bool verbose );
    DoubleVolumeResolutionCommand( const Dictionary& parameters );
    virtual ~DoubleVolumeResolutionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DoubleVolumeResolutionCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DoubleVolumeResolutionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
