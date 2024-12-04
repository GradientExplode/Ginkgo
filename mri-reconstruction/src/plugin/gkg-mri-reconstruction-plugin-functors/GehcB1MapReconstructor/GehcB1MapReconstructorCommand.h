#ifndef _gkg_mri_reconstruction_plugin_functors_GehcB1MapReconstructor_GehcB1MapReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcB1MapReconstructor_GehcB1MapReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcB1MapReconstructorCommand :
                              public Command,
                              public Creator2Arg< GehcB1MapReconstructorCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< GehcB1MapReconstructorCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    GehcB1MapReconstructorCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    GehcB1MapReconstructorCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   const std::string& fileNameMag,
                                   const std::string& xApodizationName,
                                   const std::string& yApodizationName,
                                   int32_t reconstructionSizeX,
                                   int32_t reconstructionSizeY,
                                   const std::string& outputFormat,
                                   bool verbose );
    GehcB1MapReconstructorCommand( const Dictionary& parameters );
    virtual ~GehcB1MapReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcB1MapReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcB1MapReconstructorCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& fileNameMag,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
