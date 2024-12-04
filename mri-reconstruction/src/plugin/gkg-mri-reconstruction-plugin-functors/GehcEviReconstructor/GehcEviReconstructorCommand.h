#ifndef _gkg_mri_reconstruction_plugin_functors_GehcEviReconstructor_GehcEviReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcEviReconstructor_GehcEviReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcEviReconstructorCommand :
                                public Command,
                                public Creator2Arg< GehcEviReconstructorCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< GehcEviReconstructorCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    GehcEviReconstructorCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    GehcEviReconstructorCommand( const std::string& fileNameIn,
                                 const std::string& fileNameOut,
                                 bool saveComplex,
                                 const std::string& xApodizationName,
                                 const std::string& yApodizationName,
                                 const std::string& zApodizationName,
                                 int32_t reconstructionSizeX,
                                 int32_t reconstructionSizeY,
                                 int32_t reconstructionSizeZ,
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
                                 bool gradwarp,
                                 const std::string& gradientCoilType,
                                 bool verbose );
    GehcEviReconstructorCommand( const Dictionary& parameters );
    virtual ~GehcEviReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcEviReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcEviReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool saveComplex,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  const std::string& zApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  int32_t reconstructionSizeZ,
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
                  bool gradwarp,
                  const std::string& gradientCoilType,
                  bool verbose );

};


}


#endif
