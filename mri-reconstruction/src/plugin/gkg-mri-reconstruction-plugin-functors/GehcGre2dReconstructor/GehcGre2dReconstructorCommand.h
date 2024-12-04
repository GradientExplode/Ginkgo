#ifndef _gkg_mri_reconstruction_plugin_functors_GehcGre2dReconstructor_GehcGre2dReconstructorCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcGre2dReconstructor_GehcGre2dReconstructorCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcGre2dReconstructorCommand :
                              public Command,
                              public Creator2Arg< GehcGre2dReconstructorCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< GehcGre2dReconstructorCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    GehcGre2dReconstructorCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    GehcGre2dReconstructorCommand( const std::string& fileNameIn,
                                   const std::string& fileNameOut,
                                   bool saveComplex,
                                   const std::string& xApodizationName,
                                   const std::string& yApodizationName,
                                   int32_t reconstructionSizeX,
                                   int32_t reconstructionSizeY,
                                   float scale,
                                   const std::string& outputFormat,
                                   bool multiChannel,
                                   bool verbose );
    GehcGre2dReconstructorCommand( const Dictionary& parameters );
    virtual ~GehcGre2dReconstructorCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcGre2dReconstructorCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcGre2dReconstructorCommand, Command,
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
                  const std::string& outputFormat,
                  bool multiChannel,
                  bool verbose );

};


}


#endif
