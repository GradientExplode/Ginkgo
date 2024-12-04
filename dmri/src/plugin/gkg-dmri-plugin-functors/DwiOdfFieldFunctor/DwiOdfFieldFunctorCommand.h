#ifndef _gkg_dmri_plugin_functors_DwiOdfFieldFunctor_DwiOdfFieldFunctorCommand_h_
#define _gkg_dmri_plugin_functors_DwiOdfFieldFunctor_DwiOdfFieldFunctorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <list>
#include <vector>


namespace gkg
{


class DwiOdfFieldFunctorCommand : public Command,
                                  public Creator2Arg< DwiOdfFieldFunctorCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< DwiOdfFieldFunctorCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    DwiOdfFieldFunctorCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    DwiOdfFieldFunctorCommand( const std::string& fileNameSiteMap,
                               const std::string& fileNameOdfs,
                               const std::list< std::string >& odfFunctorNames,
                               const std::list< std::string >& outputFileNames,
                               int32_t outputOrientationCount,
                               const std::string& volumeFormat,
                               const std::string& meshMapFormat,
                               const std::string& textureMapFormat,
                               float rgbScale,
                               float meshScale,
                               float lowerGFAThreshold,
                               float upperGFAThreshold,
                               const std::vector< double >&
                                                        furtherScalarParameters,
                               const std::vector< std::string >&
                                                        furtherStringParameters,
                               bool ascii,
                               bool verbose );
    DwiOdfFieldFunctorCommand( const Dictionary& parameters );
    virtual ~DwiOdfFieldFunctorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiOdfFieldFunctorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiOdfFieldFunctorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameSiteMap,
                  const std::string& fileNameOdfs,
                  const std::list< std::string >& odfFunctorNames,
                  const std::list< std::string >& outputFileNames,
                  int32_t outputOrientationCount,
                  const std::string& volumeFormat,
                  const std::string& meshMapFormat,
                  const std::string& textureMapFormat,
                  float rgbScale,
                  float meshScale,
                  float lowerGFAThreshold,
                  float upperGFAThreshold,
                  const std::vector< double >& furtherScalarParameters,
                  const std::vector< std::string >& furtherStringParameters,
                  bool ascii,
                  bool verbose );

};


}


#endif
