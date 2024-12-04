#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerPhaseMap3dReconstructor_BrukerPhaseMap3dReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerPhaseMap3dReconstructor_BrukerPhaseMap3dReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerPhaseMap3dReconstructorCommand :
                       public Command,
                       public Creator2Arg< BrukerPhaseMap3dReconstructorCommand,
                                           Command,
                                           int32_t,
                                           char** >,
                       public Creator1Arg< BrukerPhaseMap3dReconstructorCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    BrukerPhaseMap3dReconstructorCommand( int32_t argc,
                                          char* argv[],
                                          bool loadPlugin = false,
                                          bool removeFirst = true );
    BrukerPhaseMap3dReconstructorCommand( const std::string& fileNameIn,
                                          const std::string& fileNameOut,
                                          const std::string& xApodizationName,
                                          const std::string& yApodizationName,
                                          const std::string& zApodizationName,
                                          int32_t reconstructionSizeX,
                                          int32_t reconstructionSizeY,
                                          int32_t reconstructionSizeZ,
                                          const std::string& outputFormat,
                                          bool verbose );
    BrukerPhaseMap3dReconstructorCommand( const Dictionary& parameters );
    virtual ~BrukerPhaseMap3dReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerPhaseMap3dReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerPhaseMap3dReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  const std::string& zApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  int32_t reconstructionSizeZ,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
