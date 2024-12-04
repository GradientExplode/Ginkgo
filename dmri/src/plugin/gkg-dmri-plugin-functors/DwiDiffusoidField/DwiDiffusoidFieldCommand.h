#ifndef _gkg_dmri_plugin_functors_DwiDiffusoidField_DwiDiffusoidFieldCommand_h_
#define _gkg_dmri_plugin_functors_DwiDiffusoidField_DwiDiffusoidFieldCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiDiffusoidFieldCommand : public Command,
                                 public Creator2Arg< DwiDiffusoidFieldCommand,
                                                     Command,
                                                     int32_t,
                                                     char** >,
                                 public Creator1Arg< DwiDiffusoidFieldCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    DwiDiffusoidFieldCommand( int32_t argc,
                              char* argv[],
                              bool loadPlugin = false,
                              bool removeFirst = true );
    DwiDiffusoidFieldCommand( 
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& diffusoidFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     int32_t maximumNeighborCount,
                     float sigma,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose );
    DwiDiffusoidFieldCommand( const Dictionary& parameters );
    virtual ~DwiDiffusoidFieldCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiDiffusoidFieldCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiDiffusoidFieldCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::string& modelType,
                  const std::list< std::string >& diffusoidFunctorNames,
                  const std::list< std::string >& outputFileNames,
                  const std::vector< int32_t >& iBox,
                  const std::vector< float >& fBox,
                  const std::vector< float >& superResolution,
                  int32_t outputOrientationCount,
                  int32_t maximumNeighborCount,
                  float sigma,
                  float radius,
                  float thresholdRatio,
                  const std::string& volumeFormat,
                  const std::string& meshMapFormat,
                  const std::string& textureMapFormat,
                  float rgbScale,
                  float meshScale,
                  const std::vector< double >& specificScalarParameters,
                  const std::vector< std::string >& specificStringParameters,
                  bool ascii,
                  bool verbose );

};


}


#endif
