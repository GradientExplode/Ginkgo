#ifndef _gkg_mri_reconstruction_plugin_functors_GehcEpiReconstructor_GehcEpiReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcEpiReconstructor_GehcEpiReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcEpiReconstructorCommand :
                                public Command,
                                public Creator2Arg< GehcEpiReconstructorCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< GehcEpiReconstructorCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    GehcEpiReconstructorCommand( int32_t argc,
                  char* argv[], 
                  bool loadPlugin = false,
                  bool removeFirst = true );
    GehcEpiReconstructorCommand( const std::string& fileNameIn,
                                 const std::string& fileNameOut,
                                 bool saveComplex,
                                 const std::string& xApodizationName,
                                 const std::string& yApodizationName,
                                 int32_t reconstructionSizeX,
                                 int32_t reconstructionSizeY,
                                 float scale,
                                 bool noTrajectoryCorrection,
                                 const std::string& phaseCorrectionMethod,
                                 bool keepFirstMTF,
                                 int32_t fitPointCount,
                                 const std::string& outputFormat,
                                 const std::string& fileNamePhaseCorrection,
                                 bool allSliceRefScan,
                                 int32_t pcCoilNumber,
                                 bool multiChannel,
                                 const std::string& fileNamePhaseMap,
                                 const std::string& phaseMapMethod,
                                 bool gradwarp,
                                 const std::string& gradientCoilType,
                                 bool verbose );
    GehcEpiReconstructorCommand( const Dictionary& parameters );
    virtual ~GehcEpiReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcEpiReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcEpiReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool saveComplex,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  float scale,
                  bool noTrajectoryCorrection,
                  const std::string& phaseCorrectionMethod,
                  bool keepFirstMTF,
                  int32_t fitPointCount,
                  const std::string& outputFormat,
                  const std::string& fileNamePhaseCorrection,
                  bool allSliceRefScan,
                  int32_t pcCoilNumber,
                  bool multiChannel,
                  const std::string& fileNamePhaseMap,
                  const std::string& phaseMapMethod,
                  bool gradwarp,
                  const std::string& gradientCoilType,
                  bool verbose );

};


}


#endif
