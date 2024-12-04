#ifndef _gkg_mri_reconstruction_plugin_functors_GehcEpiDwReconstructor_GehcEpiDwReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcEpiDwReconstructor_GehcEpiDwReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcEpiDwReconstructorCommand :
                              public Command,
                              public Creator2Arg< GehcEpiDwReconstructorCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< GehcEpiDwReconstructorCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    GehcEpiDwReconstructorCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    GehcEpiDwReconstructorCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& xApodizationName,
                                   const std::string& yApodizationName,
                                   int32_t reconstructionSizeX,
                                   int32_t reconstructionSizeY,
                                   float scale,
                                   bool noTrajectoryCorrection,
                                   bool keepFirstMTF,
                                   const std::string& outputFormat,
                                   bool verbose );
    GehcEpiDwReconstructorCommand( const Dictionary& parameters );
    virtual ~GehcEpiDwReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcEpiDwReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcEpiDwReconstructorCommand, Command,
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
                  bool keepFirstMTF,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
