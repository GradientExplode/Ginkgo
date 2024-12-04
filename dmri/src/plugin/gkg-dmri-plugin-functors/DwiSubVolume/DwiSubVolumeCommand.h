#ifndef _gkg_dmri_plugin_functors_DwiSubVolume_DwiSubVolumeCommand_h_
#define _gkg_dmri_plugin_functors_DwiSubVolume_DwiSubVolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiSubVolumeCommand : public Command,
                     	   public Creator2Arg< DwiSubVolumeCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< DwiSubVolumeCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    DwiSubVolumeCommand( int32_t argc,
                         char* argv[], 
                         bool loadPlugin = false,
                         bool removeFirst = true );
    DwiSubVolumeCommand( const std::string& fileNameIn,
                         const std::string& fileNameOut,
                         std::vector< int32_t >& ignoredIndices,
                         int32_t subSetOrientationCount,
                         int32_t maximumNeighborCount,
                         bool ascii,
                         const std::string& outputFormat,
                         bool verbose );
    DwiSubVolumeCommand( const Dictionary& parameters );
    virtual ~DwiSubVolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiSubVolumeCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiSubVolumeCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  std::vector< int32_t >& ignoredIndices,
                  int32_t subSetOrientationCount,
                  int32_t maximumNeighborCount,
                  bool ascii,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
