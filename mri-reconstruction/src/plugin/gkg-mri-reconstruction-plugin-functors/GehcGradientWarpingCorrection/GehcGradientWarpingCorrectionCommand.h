#ifndef _gkg_mri_reconstruction_plugin_functors_GehcGradientWarpingCorrection_GehcGradientWarpingCorrectionCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcGradientWarpingCorrection_GehcGradientWarpingCorrectionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcGradientWarpingCorrectionCommand :
                       public Command,
                       public Creator2Arg< GehcGradientWarpingCorrectionCommand,
                                           Command,
                                           int32_t,
                                           char** >,
                       public Creator1Arg< GehcGradientWarpingCorrectionCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    GehcGradientWarpingCorrectionCommand( int32_t argc,
                                          char* argv[],
                                          bool loadPlugin = false,
                                          bool removeFirst = true );
    GehcGradientWarpingCorrectionCommand( const std::string& fileNameIn,
                                          const std::string& fileNameOut,
                                          const std::string& outputFormat,
                                          bool keepNegativeValues,
                                          bool displayPlanes,
                                          bool displayCoilInfo,
                                          const std::string& gradientCoilType,
                                          bool verbose );
    GehcGradientWarpingCorrectionCommand( const Dictionary& parameters );
    virtual ~GehcGradientWarpingCorrectionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcGradientWarpingCorrectionCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcGradientWarpingCorrectionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool keepNegativeValues,
                  bool displayPlanes,
                  bool displayCoilInfo,
                  const std::string& gradientCoilType,
                  bool verbose );

};


}


#endif
