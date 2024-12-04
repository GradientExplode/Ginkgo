#ifndef _gkg_dmri_plugin_functors_DwiSiteAndTextureToVolumeConverter_DwiSiteAndTextureToVolumeConverterCommand_h_
#define _gkg_dmri_plugin_functors_DwiSiteAndTextureToVolumeConverter_DwiSiteAndTextureToVolumeConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiSiteAndTextureToVolumeConverterCommand :
                  public Command,
                  public Creator2Arg< DwiSiteAndTextureToVolumeConverterCommand,
                                      Command,
                                      int32_t,
                                      char** >,
                  public Creator1Arg< DwiSiteAndTextureToVolumeConverterCommand,
                                      Command,
                                      const Dictionary& >
{

  public:

    DwiSiteAndTextureToVolumeConverterCommand( int32_t argc,
                                               char* argv[],
                                               bool loadPlugin = false,
                                               bool removeFirst = true );
    DwiSiteAndTextureToVolumeConverterCommand( 
                                             const std::string& fileNameSiteMap,
                                             const std::string& fileNameOdfs,
                                             const std::string& outputFileName,
                                             const std::string& volumeFormat,
                                             bool ascii,
                                             bool verbose );
    DwiSiteAndTextureToVolumeConverterCommand( const Dictionary& parameters );
    virtual ~DwiSiteAndTextureToVolumeConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiSiteAndTextureToVolumeConverterCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiSiteAndTextureToVolumeConverterCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameSiteMap,
                  const std::string& fileNameOdfs,
                  const std::string& outputFileName,
                  const std::string& volumeFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
