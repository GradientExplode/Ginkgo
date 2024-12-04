#ifndef _gkg_dmri_plugin_functors_DwiEddyCurrent_DwiEddyCurrentCommand_h_
#define _gkg_dmri_plugin_functors_DwiEddyCurrent_DwiEddyCurrentCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiEddyCurrentCommand : public Command,
                              public Creator2Arg< DwiEddyCurrentCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< DwiEddyCurrentCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DwiEddyCurrentCommand( int32_t argc,
                           char* argv[],
                           bool loadPlugin = false,
                           bool removeFirst = true );
    DwiEddyCurrentCommand( 
                       const std::string& inputName,
                       int32_t referenceRank,
                       const std::string& referenceName,
                       const std::string& outputName,
                       const std::string& transform3dName,
                       const std::string& similarityMeasureName,
                       const std::string& optimizerName,
                       double lowerThreshold,
                       int32_t registrationResamplingOrder,
                       const std::list< int32_t >& subSamplingMaximumSizes,
                       const std::vector< double >& similarityMeasureParameters,
                       const std::vector< double >& optimizerParameters,
                       const std::vector< float >& initialParameters,
                       int32_t outputResamplingOrder,
                       double background,
                       const std::string& furtherMotionName,
                       const gkg::Vector3d< int32_t >& furtherSliceCount,
                       bool correctQSpaceSampling,
                       const std::string& eddyCurrentProfileName,
                       bool verboseRegistration,
                       bool verbose );
    DwiEddyCurrentCommand( const Dictionary& parameters );
    virtual ~DwiEddyCurrentCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiEddyCurrentCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiEddyCurrentCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& inputName,
                  int32_t referenceRank,
                  const std::string& referenceName,
                  const std::string& outputName,
                  const std::string& transform3dName,
                  const std::string& similarityMeasureName,
                  const std::string& optimizerName,
                  double lowerThreshold,
                  int32_t registrationResamplingOrder,
                  const std::list< int32_t >& subSamplingMaximumSizes,
                  const std::vector< double >& similarityMeasureParameters,
                  const std::vector< double >& optimizerParameters,
                  const std::vector< float >& initialParameters,
                  int32_t outputResamplingOrder,
                  double background,
                  const std::string& furtherMotionName,
                  const gkg::Vector3d< int32_t >& furtherSliceCount,
                  bool correctQSpaceSampling,
                  const std::string& eddyCurrentProfileName,
                  bool verboseRegistration,
                  bool verbose );


};


}


#endif
