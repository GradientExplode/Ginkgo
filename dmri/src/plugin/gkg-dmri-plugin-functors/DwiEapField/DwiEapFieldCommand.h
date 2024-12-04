#ifndef _gkg_dmri_plugin_functors_DwiEapField_DwiEapFieldCommand_h_
#define _gkg_dmri_plugin_functors_DwiEapField_DwiEapFieldCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class DwiEapFieldCommand : public Command,
                           public Creator2Arg< DwiEapFieldCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< DwiEapFieldCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    DwiEapFieldCommand( int32_t argc,
                        char* argv[],
                        bool loadPlugin = false,
                        bool removeFirst = true );
    DwiEapFieldCommand( 
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& methodType,
                     const std::list< std::string >& eapFunctorNames,
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
                     bool meshOpacity,
                     const std::vector< float >& eapMeshRadii,
                     const std::vector< float >& odfLowerUpperLimits,
                     bool marginalOdf,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose );
    DwiEapFieldCommand( const Dictionary& parameters );
    virtual ~DwiEapFieldCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiEapFieldCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiEapFieldCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMask,
                  const std::string& methodType,
                  const std::list< std::string >& eapFunctorNames,
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
                  bool meshOpacity,
                  const std::vector< float >& eapMeshRadii,
                  const std::vector< float >& odfLowerUpperLimits,
                  bool marginalOdf,
                  const std::vector< double >& specificScalarParameters,
                  const std::vector< std::string >& specificStringParameters,
                  bool ascii,
                  bool verbose );


};


}


#endif
