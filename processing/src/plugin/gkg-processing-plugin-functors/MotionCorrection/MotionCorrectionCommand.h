#ifndef _gkg_processing_plugin_functors_MotionCorrection_MotionCorrectionCommand_h_
#define _gkg_processing_plugin_functors_MotionCorrection_MotionCorrectionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class MotionCorrectionCommand : public Command,
                                public Creator2Arg< MotionCorrectionCommand,
                                         	    Command,
                                         	    int32_t,
                                         	    char** >,
                                public Creator1Arg< MotionCorrectionCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    MotionCorrectionCommand( int32_t argc,
                  	     char* argv[], 
                  	     bool loadPlugin = false,
                  	     bool removeFirst = true );
    MotionCorrectionCommand(
                       const std::string& inputName,
                       int32_t referenceRank,
                       const std::string& referenceName,
                       std::string& outputName,
                       const std::string& transform3dName,
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       double lowerThreshold,
                       int32_t registrationResamplingOrder,
                       std::list< int32_t >& subSamplingMaximumSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       std::vector< double >& optimizerParameters,
                       const std::string& transformType,
                       const std::vector< float >& initialParameters,
                       int32_t outputResamplingOrder,
                       double background,
                       const std::string& furtherMotionName,
                       const gkg::Vector3d< int32_t >& furtherSliceCount,
                       bool verboseRegistration,
                       bool verbose );
    MotionCorrectionCommand( const Dictionary& parameters );
    virtual ~MotionCorrectionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MotionCorrectionCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MotionCorrectionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& inputName,
                  int32_t referenceRank,
                  const std::string& referenceName,
                  std::string& outputName,
                  const std::string& transform3dName,
                  const std::string& similarityMeasureName,
                  const std::string& optimizerName,
                  double lowerThreshold,
                  int32_t registrationResamplingOrder,
                  std::list< int32_t >& subSamplingMaximumSizes,
                  const std::vector< double >& similarityMeasureParameters,
                  const std::vector< double >& optimizerParameters,
                  const std::string& transformType,
                  const std::vector< float >& initialParameters,
                  int32_t outputResamplingOrder,
                  double background,
                  const std::string& furtherMotionName,
                  const gkg::Vector3d< int32_t >& furtherSliceCount,
                  bool verboseRegistration,
                  bool verbose );

};


}


#endif
