#ifndef _gkg_dmri_plugin_functors_DwiBundleMapDensityMask_DwiBundleMapDensityMaskCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapDensityMask_DwiBundleMapDensityMaskCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapDensityMaskCommand :
                             public Command,
                             public Creator2Arg< DwiBundleMapDensityMaskCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< DwiBundleMapDensityMaskCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DwiBundleMapDensityMaskCommand( int32_t argc,
                  char* argv[], 
                  bool loadPlugin = false,
                  bool removeFirst = true );
    DwiBundleMapDensityMaskCommand( const std::string& fileNameT2,
                                    const std::string& fileNameBundleMap,
                                    const std::string& fileNameOut,
                                    float step,
                                    const std::string& outputFormat,
                                    bool normalized,
                                    bool singleFile,
                                    const bool& useThickFascicles,
                                    const float& fascicleRadius,
                                    const int32_t& sphereOrientationCount,
                                    bool ascii,
                                    bool verbose );
    DwiBundleMapDensityMaskCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapDensityMaskCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapDensityMaskCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapDensityMaskCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameBundleMap,
                  const std::string& fileNameOut,
                  float step,
                  const std::string& outputFormat,
                  bool normalized,
                  bool singleFile,
                  const bool& useThickFascicles,
                  const float& fascicleRadius,
                  const int32_t& sphereOrientationCount,
                  bool ascii,
                  bool verbose );

};


}


#endif
