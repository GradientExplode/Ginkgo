#ifndef _gkg_mri_reconstruction_plugin_functors_SenseReconstructor_SenseReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_SenseReconstructor_SenseReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class SenseReconstructorCommand : public Command,
                                  public Creator2Arg< SenseReconstructorCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< SenseReconstructorCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    SenseReconstructorCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    SenseReconstructorCommand( const std::string& fileNameCoilInput,
                               const std::string& fileNameFoldedInput,
                               const std::string& fileNameUnfoldedOutput,
                               float dilationRadiusFactor,
                               float closingRadiusFactor,
                               double noisePosition,
                               double noiseRatio,
                               int32_t thrown,
                               int32_t kept,
                               float scale,
                               const std::string& inverseMethod,
                               const std::string& normWeightingMethod,
                               double tikhonovParameter,
                               bool extra,
                               const std::string& outputFormat,
                               bool verbose );
    SenseReconstructorCommand( const Dictionary& parameters );
    virtual ~SenseReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< SenseReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< SenseReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameCoilInput,
                  const std::string& fileNameFoldedInput,
                  const std::string& fileNameUnfoldedOutput,
                  float dilationRadiusFactor,
                  float closingRadiusFactor,
                  double noisePosition,
                  double noiseRatio,
                  int32_t thrown,
                  int32_t kept,
                  float scale,
                  const std::string& inverseMethod,
                  const std::string& normWeightingMethod,
                  double tikhonovParameter,
                  bool extra,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
