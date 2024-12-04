#ifndef _gkg_dmri_plugin_functors_DwiPdfField_DwiPdfFieldCommand_h_
#define _gkg_dmri_plugin_functors_DwiPdfField_DwiPdfFieldCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <list>
#include <vector>


namespace gkg
{


class DwiPdfFieldCommand : public Command,
                           public Creator2Arg< DwiPdfFieldCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< DwiPdfFieldCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    DwiPdfFieldCommand( int32_t argc,
                        char* argv[],
                        bool loadPlugin = false,
                        bool removeFirst = true );
    DwiPdfFieldCommand( 
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& pdfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     const std::vector< float >& outputSampling,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose );
    DwiPdfFieldCommand( const Dictionary& parameters );
    virtual ~DwiPdfFieldCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiPdfFieldCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiPdfFieldCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::string& modelType,
                  const std::list< std::string >& odfFunctorNames,
                  const std::list< std::string >& outputFileNames,
                  const std::vector< int32_t >& iBox,
                  const std::vector< float >& fBox,
                  const std::vector< float >& superResolution,
                  const std::vector< float >& outputSampling,
                  float radius,
                  float thresholdRatio,
                  const std::string& volumeFormat,
                  const std::string& meshMapFormat,
                  const std::string& textureMapFormat,
                  float rgbScale,
                  float meshScale,
                  float lowerGFAThreshold,
                  float upperGFAThreshold,
                  const std::vector< double >& specificScalarParameters,
                  const std::vector< std::string >& specificStringParameters,
                  const std::vector< double >& furtherScalarParameters,
                  const std::vector< std::string >& furtherStringParameters,
                  bool ascii,
                  bool verbose );

};


}


#endif
