#ifndef _gkg_processing_plugin_functors_DericheGaussianFilter3d_DericheGaussianFilter3dCommand_h_
#define _gkg_processing_plugin_functors_DericheGaussianFilter3d_DericheGaussianFilter3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DericheGaussianFilter3dCommand :
                             public Command,
                             public Creator2Arg< DericheGaussianFilter3dCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< DericheGaussianFilter3dCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DericheGaussianFilter3dCommand( int32_t argc,
                                    char* argv[], 
                                    bool loadPlugin = false,
                                    bool removeFirst = true );
    DericheGaussianFilter3dCommand( const std::string& fileNameIn,
                                    const std::string& fileNameOut,
                                    float filteringFWHM,
                                    const std::string& filteringAxis,
                                    const std::string& filteringType,
                                    bool ascii,
                                    const std::string& format,
                                    bool verbose );
    DericheGaussianFilter3dCommand( const Dictionary& parameters );
    virtual ~DericheGaussianFilter3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DericheGaussianFilter3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DericheGaussianFilter3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  float filteringFWHM,
                  const std::string& filteringAxis,
                  const std::string& filteringType,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
