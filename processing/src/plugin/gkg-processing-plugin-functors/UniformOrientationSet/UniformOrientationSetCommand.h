#ifndef _gkg_processing_plugin_functors_UniformOrientationSet_UniformOrientationSetCommand_h_
#define _gkg_processing_plugin_functors_UniformOrientationSet_UniformOrientationSetCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class UniformOrientationSetCommand :
                               public Command,
                               public Creator2Arg< UniformOrientationSetCommand,
                                         	   Command,
                                         	   int32_t,
                                         	   char** >,
                               public Creator1Arg< UniformOrientationSetCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    UniformOrientationSetCommand( int32_t argc,
                  		  char* argv[], 
                  		  bool loadPlugin = false,
                  		  bool removeFirst = true );
    UniformOrientationSetCommand( const std::string& pattern,
                                  float multiplicator,
                                  int32_t orientationCount,
                                  int32_t windowSize,
                                  float threshold,
                                  float power,
                                  int32_t localMinimization,
                                  int32_t globalStepCount,
                                  int32_t internalStepCount,
                                  int32_t probabilityFactor,
                                  std::string& fileNameOut,
                                  const std::string& algorithmType,
                                  float initialStep,
                                  float minimumStep,
                                  bool avoidSymmetricOrientations,
                                  bool verbose );
    UniformOrientationSetCommand( const Dictionary& parameters );
    virtual ~UniformOrientationSetCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< UniformOrientationSetCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< UniformOrientationSetCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& pattern,
                  float multiplicator,
                  int32_t orientationCount,
                  int32_t windowSize,
                  float threshold,
                  float power,
                  int32_t localMinimization,
                  int32_t globalStepCount,
                  int32_t internalStepCount,
                  int32_t probabilityFactor,
                  std::string& fileNameOut,
                  const std::string& algorithmType,
                  float initialStep,
                  float minimumStep,
                  bool avoidSymmetricOrientations,
                  bool verbose );

};


}


#endif
