#ifndef _gkg_dmri_plugin_functors_DwiTwoPoolModelField_DwiTwoPoolModelFieldCommand_h_
#define _gkg_dmri_plugin_functors_DwiTwoPoolModelField_DwiTwoPoolModelFieldCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiTwoPoolModelFieldCommand :
                                public Command,
                                public Creator2Arg< DwiTwoPoolModelFieldCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< DwiTwoPoolModelFieldCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    DwiTwoPoolModelFieldCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    DwiTwoPoolModelFieldCommand(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::list< std::string >& twoPoolModelFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerFAThreshold,
                     float upperFAThreshold,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose );
    DwiTwoPoolModelFieldCommand( const Dictionary& parameters );
    virtual ~DwiTwoPoolModelFieldCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiTwoPoolModelFieldCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiTwoPoolModelFieldCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::list< std::string >& twoPoolModelFunctorNames,
                  const std::list< std::string >& outputFileNames,
                  const std::vector< int32_t >& iBox,
                  const std::vector< float >& fBox,
                  const std::vector< float >& superResolution,
                  int32_t outputOrientationCount,
                  float radius,
                  float thresholdRatio,
                  const std::string& volumeFormat,
                  const std::string& meshMapFormat,
                  const std::string& textureMapFormat,
                  float rgbScale,
                  float meshScale,
                  float lowerFAThreshold,
                  float upperFAThreshold,
                  const std::vector< double >& specificScalarParameters,
                  const std::vector< std::string >& specificStringParameters,
                  bool ascii,
                  bool verbose);

};


}


#endif
