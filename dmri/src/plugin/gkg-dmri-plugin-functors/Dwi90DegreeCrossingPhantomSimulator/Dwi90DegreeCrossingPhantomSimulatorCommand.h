#ifndef _gkg_dmri_plugin_functors_Dwi90DegreeCrossingPhantomSimulator_Dwi90DegreeCrossingPhantomSimulatorCommand_h_
#define _gkg_dmri_plugin_functors_Dwi90DegreeCrossingPhantomSimulator_Dwi90DegreeCrossingPhantomSimulatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Dwi90DegreeCrossingPhantomSimulatorCommand :
                 public Command,
                 public Creator2Arg< Dwi90DegreeCrossingPhantomSimulatorCommand,
                                     Command,
                                     int32_t,
                                     char** >,
                 public Creator1Arg< Dwi90DegreeCrossingPhantomSimulatorCommand,
                                     Command,
                                     const Dictionary& >
{

  public:

    Dwi90DegreeCrossingPhantomSimulatorCommand( int32_t argc,
                                                char* argv[],
                                                bool loadPlugin = false,
                                                bool removeFirst = true );
    Dwi90DegreeCrossingPhantomSimulatorCommand( 
                                  bool verbose,
                                  int32_t orientationCount,
                                  float b,
                                  float lambda1,
                                  float lambda2,
                                  float lambda3,
                                  float horizontalVolumeFraction,
                                  float noiseStdDev,
                                  const std::string& fileNameDWI,
                                  const std::string& fileNameTractographyMask );
    Dwi90DegreeCrossingPhantomSimulatorCommand( const Dictionary& parameters );
    virtual ~Dwi90DegreeCrossingPhantomSimulatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Dwi90DegreeCrossingPhantomSimulatorCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Dwi90DegreeCrossingPhantomSimulatorCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( bool verbose,
                  int32_t orientationCount,
                  float b,
                  float lambda1,
                  float lambda2,
                  float lambda3,
                  float horizontalVolumeFraction,
                  float noiseStdDev,
                  const std::string& fileNameDWI,
                  const std::string& fileNameTractographyMask );

};


}


#endif
