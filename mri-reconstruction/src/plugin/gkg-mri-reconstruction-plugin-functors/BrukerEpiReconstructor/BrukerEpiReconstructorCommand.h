#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerEpiReconstructor_BrukerEpiReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerEpiReconstructor_BrukerEpiReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerEpiReconstructorCommand :
                              public Command,
                              public Creator2Arg< BrukerEpiReconstructorCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< BrukerEpiReconstructorCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    BrukerEpiReconstructorCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    BrukerEpiReconstructorCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& xApodizationName,
                                   const std::string& yApodizationName,
                                   int32_t reconstructionSizeX,
                                   int32_t reconstructionSizeY,
                                   float scale,
                                   bool noTrajectoryCorrection,
                                   float forcedYScrollingRatio,
                                   bool applyMTF,
                                   bool keepFirstMTF,
                                   int32_t referenceSliceMTF,
                                   bool useCentreSliceMTF,
                                   const std::string& ppg,
                                   const std::string& outputFormat,
                                   bool verbose );
    BrukerEpiReconstructorCommand( const Dictionary& parameters );
    virtual ~BrukerEpiReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerEpiReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerEpiReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  float scale,
                  bool noTrajectoryCorrection,
                  float forcedYScrollingRatio,
                  bool applyMTF,
                  bool keepFirstMTF,
                  int32_t referenceSliceMTF,
                  bool useCentreSliceMTF,
                  const std::string& ppg,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
