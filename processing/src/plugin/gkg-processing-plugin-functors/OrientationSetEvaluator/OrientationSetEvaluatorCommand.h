#ifndef _gkg_processing_plugin_functors_OrientationSetEvaluator_OrientationSetEvaluatorCommand_h_
#define _gkg_processing_plugin_functors_OrientationSetEvaluator_OrientationSetEvaluatorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class OrientationSetEvaluatorCommand :
                             public Command,
                             public Creator2Arg< OrientationSetEvaluatorCommand,
                                         	 Command,
                                         	 int32_t,
                                         	 char** >,
                             public Creator1Arg< OrientationSetEvaluatorCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    OrientationSetEvaluatorCommand( int32_t argc,
                  		    char* argv[], 
                  		    bool loadPlugin = false,
                  		    bool removeFirst = true );
    OrientationSetEvaluatorCommand( const std::string& fileNameIn,
                                    int32_t firstOrientation,
                                    int32_t orientationCount,
                                    int32_t windowSize,
                                    const std::string& backupName,
                                    bool verbose );
    OrientationSetEvaluatorCommand( const Dictionary& parameters );
    virtual ~OrientationSetEvaluatorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< OrientationSetEvaluatorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< OrientationSetEvaluatorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  int32_t firstOrientation,
                  int32_t orientationCount,
                  int32_t windowSize,
                  const std::string& backupName,
                  bool verbose );

};


}


#endif
