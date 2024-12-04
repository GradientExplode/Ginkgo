#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerGre3dReconstructor_BrukerGre3dReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerGre3dReconstructor_BrukerGre3dReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerGre3dReconstructorCommand :
                            public Command,
                            public Creator2Arg< BrukerGre3dReconstructorCommand,
                                                Command,
                                                int32_t,
                                                char** >,
                            public Creator1Arg< BrukerGre3dReconstructorCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    BrukerGre3dReconstructorCommand( int32_t argc,
                                     char* argv[],
                                     bool loadPlugin = false,
                                     bool removeFirst = true );
    BrukerGre3dReconstructorCommand( const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                                     const std::string& xApodizationName,
                                     const std::string& yApodizationName,
                                     const std::string& zApodizationName,
                                     int32_t reconstructionSizeX,
                                     int32_t reconstructionSizeY,
                                     int32_t reconstructionSizeZ,
                                     float scale,
                                     const std::string& outputFormat,
                                     bool verbose );
    BrukerGre3dReconstructorCommand( const Dictionary& parameters );
    virtual ~BrukerGre3dReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerGre3dReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerGre3dReconstructorCommand, Command,
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
                  float scale,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
