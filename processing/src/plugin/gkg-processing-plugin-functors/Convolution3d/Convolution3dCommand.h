#ifndef _gkg_processing_plugin_functors_Convolution3d_Convolution3dCommand_h_
#define _gkg_processing_plugin_functors_Convolution3d_Convolution3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class Convolution3dCommand : public Command,
                             public Creator2Arg< Convolution3dCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< Convolution3dCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    Convolution3dCommand( int32_t argc,
                          char* argv[], 
                          bool loadPlugin = false,
                          bool removeFirst = true );
    Convolution3dCommand( const std::string& fileNameIn,
                          const std::string& fileNameOut,
                          const std::string& outType,
                          std::vector< float >& kernelX,
                          std::vector< float >& kernelY,
                          std::vector< float >& kernelZ,
                          const std::vector< float >& kernel3d,
                          const std::vector< int32_t >& kernel3dSize,
                          bool ascii,
                          const std::string& format,
                          bool verbose );
    Convolution3dCommand( const Dictionary& parameters );
    virtual ~Convolution3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Convolution3dCommand, Command, int32_t, char** >;
    friend class Creator1Arg< Convolution3dCommand, Command, 
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outType,
                  std::vector< float >& kernelX,
                  std::vector< float >& kernelY,
                  std::vector< float >& kernelZ,
                  const std::vector< float >& kernel3d,
                  const std::vector< int32_t >& kernel3dSize,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
