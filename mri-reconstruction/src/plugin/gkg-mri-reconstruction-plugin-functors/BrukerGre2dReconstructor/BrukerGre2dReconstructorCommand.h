#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerGre2dReconstructor_BrukerGre2dReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerGre2dReconstructor_BrukerGre2dReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerGre2dReconstructorCommand :
                            public Command,
                            public Creator2Arg< BrukerGre2dReconstructorCommand,
                                                Command,
                                                int32_t,
                                                char** >,
                            public Creator1Arg< BrukerGre2dReconstructorCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    BrukerGre2dReconstructorCommand( int32_t argc,
                                     char* argv[],
                                     bool loadPlugin = false,
                                     bool removeFirst = true );
    BrukerGre2dReconstructorCommand( const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                                     const std::string& xApodizationName,
                                     const std::string& yApodizationName,
                                     int32_t reconstructionSizeX,
                                     int32_t reconstructionSizeY,
                                     float scale,
                                     const std::string& outputFormat,
                                     bool verbose );
    BrukerGre2dReconstructorCommand( const Dictionary& parameters );
    virtual ~BrukerGre2dReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerGre2dReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerGre2dReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  float scale,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
