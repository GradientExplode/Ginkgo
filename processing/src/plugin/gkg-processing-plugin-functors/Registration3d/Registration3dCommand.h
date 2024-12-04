#ifndef _gkg_processing_plugin_functors_Registration3d_Registration3dCommand_h_
#define _gkg_processing_plugin_functors_Registration3d_Registration3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class Registration3dCommand : public Command,
                              public Creator2Arg< Registration3dCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< Registration3dCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    Registration3dCommand( int32_t argc,
                  char* argv[], 
                  bool loadPlugin = false,
                  bool removeFirst = true );
    Registration3dCommand(
                       const std::string& referenceName,
                       const std::string& floatingName,
                       const std::string& outputName,
                       const std::string& outputInverseName,
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       double referenceLowerThreshold ,
                       double floatingLowerThreshold,
                       int32_t resamplingOrder,
                       const std::list< int32_t >& subSamplingMaximumSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       const std::string& transformType,
                       const std::vector< float >& initialParameters,
                       const std::string& initialTrmName,
                       const std::string& intermediateFileName,
                       bool ignoreHeaderTransformations,
                       bool initializedUsingCentreOfGravity,
                       bool verbose );
    Registration3dCommand( const Dictionary& parameters );
    virtual ~Registration3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Registration3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Registration3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& referenceName,
                  const std::string& floatingName,
                  const std::string& outputName,
                  const std::string& outputInverseName,
                  const std::string& similarityMeasureName,
                  const std::string& optimizerName,
                  double referenceLowerThreshold ,
                  double floatingLowerThreshold,
                  int32_t resamplingOrder,
                  const std::list< int32_t >& subSamplingMaximumSizes,
                  const std::vector< double >& similarityMeasureParameters,
                  const std::vector< double >& optimizerParameters,
                  const std::string& transformType,
                  const std::vector< float >& initialParameters,
                  const std::string& initialTrmName,
                  const std::string& intermediateFileName,
                  bool ignoreHeaderTransformations,
                  bool initializedUsingCentreOfGravity,
                  bool verbose );

};


}


#endif
