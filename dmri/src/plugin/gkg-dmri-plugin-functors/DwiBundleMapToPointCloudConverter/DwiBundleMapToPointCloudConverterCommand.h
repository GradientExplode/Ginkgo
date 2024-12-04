#ifndef _gkg_dmri_plugin_functors_DwiBundleMapToPointCloudConverter_DwiBundleMapToPointCloudConverterCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapToPointCloudConverter_DwiBundleMapToPointCloudConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapToPointCloudConverterCommand :
                   public Command,
                   public Creator2Arg< DwiBundleMapToPointCloudConverterCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< DwiBundleMapToPointCloudConverterCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    DwiBundleMapToPointCloudConverterCommand( int32_t argc,
                                              char* argv[],
                                              bool loadPlugin = false,
                                              bool removeFirst = true );
    DwiBundleMapToPointCloudConverterCommand(
                                 const std::string& fileNameBundleMap,
                                 const std::string& fileNameOut,
                                 float step,
                                 bool singleFile,
                                 const std::string& coloringMode,
                                 const std::string& fileNameColorInformation,
                                 bool verbose );
    DwiBundleMapToPointCloudConverterCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapToPointCloudConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapToPointCloudConverterCommand, Command,
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapToPointCloudConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                  const std::string& fileNameOut,
                  float step,
                  bool singleFile,
                  const std::string& coloringMode,
                  const std::string& fileNameColorInformation,
                  bool verbose );

};


}


#endif
