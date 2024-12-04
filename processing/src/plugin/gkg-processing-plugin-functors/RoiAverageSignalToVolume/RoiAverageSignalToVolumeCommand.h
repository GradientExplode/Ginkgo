#ifndef _gkg_processing_plugin_functors_RoiAverageSignalToVolume_RoiAverageSignalToVolumeCommand_h_
#define _gkg_processing_plugin_functors_RoiAverageSignalToVolume_RoiAverageSignalToVolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class RoiAverageSignalToVolumeCommand :
                            public Command,
                            public Creator2Arg< RoiAverageSignalToVolumeCommand,
                                         	Command,
                                         	int32_t,
                                         	char** >,
                            public Creator1Arg< RoiAverageSignalToVolumeCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    RoiAverageSignalToVolumeCommand( int32_t argc,
                  		     char* argv[], 
                  		     bool loadPlugin = false,
                  		     bool removeFirst = true );
    RoiAverageSignalToVolumeCommand(
                             const std::string& fileNameIn,
                             const std::string& fileNameLabel,
                             const std::string& fileNameLabelToInputTransform3d,
                             const std::string& fileNameOut,
                             bool ascii,
                             const std::string& format,
                             bool verbose );
    RoiAverageSignalToVolumeCommand( const Dictionary& parameters );
    virtual ~RoiAverageSignalToVolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RoiAverageSignalToVolumeCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< RoiAverageSignalToVolumeCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameLabel,
                  const std::string& fileNameLabelToInputTransform3d,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
