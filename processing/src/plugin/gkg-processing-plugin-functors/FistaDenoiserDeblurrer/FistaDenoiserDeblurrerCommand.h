#ifndef _gkg_processing_plugin_functors_FistaDenoiserDeblurrer_FistaDenoiserDeblurrerCommand_h_
#define _gkg_processing_plugin_functors_FistaDenoiserDeblurrer_FistaDenoiserDeblurrerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class FistaDenoiserDeblurrerCommand : public Command,
                             public Creator2Arg< FistaDenoiserDeblurrerCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< FistaDenoiserDeblurrerCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    FistaDenoiserDeblurrerCommand( int32_t argc,
                          char* argv[], 
                          bool loadPlugin = false,
                          bool removeFirst = true );
    FistaDenoiserDeblurrerCommand(
                          const std::string& fileNameIn,
                          const std::string& fileNameMask,
                          const std::string& fileNameOut,
                          const std::vector< float >& kernel3d,
                          const std::vector< int32_t >& kernel3dSize,
                          const std::string& mode,
                          double threshold1,
                          double threshold2,
                          double background,
                          int32_t maximumIterationCount,
                          double tolerance,
                          double shrinkageStep,
                          int32_t skrinkageMaximumIterationCount,
                          double shrinkageTolerance,
                          bool ascii,
                          const std::string& format,
                          bool verbose );
    FistaDenoiserDeblurrerCommand( const Dictionary& parameters );
    virtual ~FistaDenoiserDeblurrerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< FistaDenoiserDeblurrerCommand, Command, int32_t,
                              char** >;
    friend class Creator1Arg< FistaDenoiserDeblurrerCommand, Command, 
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameMask,
                  const std::string& fileNameOut,
                  const std::vector< float >& kernel3d,
                  const std::vector< int32_t >& kernel3dSize,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  double background,
                  int32_t maximumIterationCount,
                  double tolerance,
                  double shrinkageStep,
                  int32_t skrinkageMaximumIterationCount,
                  double shrinkageTolerance,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
