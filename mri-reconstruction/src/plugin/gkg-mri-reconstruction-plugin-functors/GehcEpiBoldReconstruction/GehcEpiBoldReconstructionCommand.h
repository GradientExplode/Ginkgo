#ifndef _gkg_mri_reconstruction_plugin_functors_GehcEpiBoldReconstruction_GehcEpiBoldReconstructionCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_GehcEpiBoldReconstruction_GehcEpiBoldReconstructionCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class GehcEpiBoldReconstructionCommand :
                           public Command,
                           public Creator2Arg< GehcEpiBoldReconstructionCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< GehcEpiBoldReconstructionCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    GehcEpiBoldReconstructionCommand( int32_t argc,
                                      char* argv[],
                                      bool loadPlugin = false,
                                      bool removeFirst = true );
    GehcEpiBoldReconstructionCommand( const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      const std::string& xApodizationName,
                                      const std::string& yApodizationName,
                                      int32_t reconstructionSizeX,
                                      int32_t reconstructionSizeY,
                                      float scale,
                                      bool keepFirstMTF,
                                      int32_t mtfSliceReference,
                                      const std::string& outputFormat,
                                      bool verbose );
    GehcEpiBoldReconstructionCommand( const Dictionary& parameters );
    virtual ~GehcEpiBoldReconstructionCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< GehcEpiBoldReconstructionCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< GehcEpiBoldReconstructionCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& xApodizationName,
                  const std::string& yApodizationName,
                  int32_t reconstructionSizeX,
                  int32_t reconstructionSizeY,
                  float scale,
                  bool keepFirstMTF,
                  int32_t mtfSliceReference,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
