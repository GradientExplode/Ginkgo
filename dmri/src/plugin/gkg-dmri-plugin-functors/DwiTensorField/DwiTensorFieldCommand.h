#ifndef _gkg_dmri_plugin_functors_DwiTensorField_DwiTensorFieldCommand_h_
#define _gkg_dmri_plugin_functors_DwiTensorField_DwiTensorFieldCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiTensorFieldCommand : public Command,
                              public Creator2Arg< DwiTensorFieldCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< DwiTensorFieldCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DwiTensorFieldCommand( int32_t argc,
                           char* argv[],
                           bool loadPlugin = false,
                           bool removeFirst = true );
    DwiTensorFieldCommand( 
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::list< std::string >& tensorFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
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
    DwiTensorFieldCommand( const Dictionary& parameters );
    virtual ~DwiTensorFieldCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiTensorFieldCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiTensorFieldCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::list< std::string >& tensorFunctorNames,
                  const std::list< std::string >& outputFileNames,
                  const std::vector< int32_t >& iBox,
                  const std::vector< float >& fBox,
                  const std::vector< float >& superResolution,
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

};


}


#endif
