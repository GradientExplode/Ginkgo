#ifndef _gkg_mri_reconstruction_plugin_functors_BrukerPhaseMapReconstructor_BrukerPhaseMapReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_BrukerPhaseMapReconstructor_BrukerPhaseMapReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrukerPhaseMapReconstructorCommand :
                         public Command,
                         public Creator2Arg< BrukerPhaseMapReconstructorCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< BrukerPhaseMapReconstructorCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    BrukerPhaseMapReconstructorCommand( int32_t argc,
                                        char* argv[],
                                        bool loadPlugin = false,
                                        bool removeFirst = true );
    BrukerPhaseMapReconstructorCommand( const std::string& fileNameIn,
                                        const std::string& fileNameOut,
                                        const std::string& xApodizationName,
                                        const std::string& yApodizationName,
                                        int32_t reconstructionSizeX,
                                        int32_t reconstructionSizeY,
                                        const std::string& outputFormat,
                                        bool verbose );
    BrukerPhaseMapReconstructorCommand( const Dictionary& parameters );
    virtual ~BrukerPhaseMapReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrukerPhaseMapReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< BrukerPhaseMapReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
