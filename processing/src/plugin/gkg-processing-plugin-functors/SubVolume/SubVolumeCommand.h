#ifndef _gkg_processing_plugin_functors_SubVolume_SubVolumeCommand_h_
#define _gkg_processing_plugin_functors_SubVolume_SubVolumeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <list>


namespace gkg
{


class SubVolumeCommand : public Command,
                         public Creator2Arg< SubVolumeCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< SubVolumeCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    SubVolumeCommand( int32_t argc,
                      char* argv[], 
                      bool loadPlugin = false,
                      bool removeFirst = true );
    SubVolumeCommand( const std::string& fileNameIn,
                      const std::string& fileNameOut, 
                      int32_t sx, int32_t sy, int32_t sz, int32_t st,
                      int32_t ex, int32_t ey, int32_t ez, int32_t et,
                      const std::list< int32_t >& xIndices,
                      const std::list< int32_t >& yIndices,
                      const std::list< int32_t >& zIndices,
                      const std::list< int32_t >& tIndices,
                      bool ascii,
                      const std::string& format,
                      bool verbose );
    SubVolumeCommand( const Dictionary& parameters );
    virtual ~SubVolumeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SubVolumeCommand, Command, int32_t, char** >;
    friend class Creator1Arg< SubVolumeCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut, 
                  int32_t sx, int32_t sy, int32_t sz, int32_t st,
                  int32_t ex, int32_t ey, int32_t ez, int32_t et,
                  const std::list< int32_t >& xIndices,
                  const std::list< int32_t >& yIndices,
                  const std::list< int32_t >& zIndices,
                  const std::list< int32_t >& tIndices,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
