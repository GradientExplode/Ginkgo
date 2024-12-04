#ifndef _gkg_dmri_plugin_functors_DwiBundleMapSectionMask_DwiBundleMapSectionMaskCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapSectionMask_DwiBundleMapSectionMaskCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiBundleMapSectionMaskCommand :
                             public Command,
                             public Creator2Arg< DwiBundleMapSectionMaskCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< DwiBundleMapSectionMaskCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DwiBundleMapSectionMaskCommand( int32_t argc,
                                    char* argv[],
                                    bool loadPlugin = false,
                                    bool removeFirst = true );
    DwiBundleMapSectionMaskCommand(
                          const std::string& fileNameInputBundleMap,
                          const std::string& fileNameCentroids,
                          const std::string& fileNameSectionMask,
                          const std::string& fileNameDensityMask,
                          float sectionWidth,
                          float resamplingStep,
                          const std::vector< double >& vectorOfOutputResolution,
                          const std::string& outputFormat,
                          bool normalizedDensity,
                          bool ascii,
                          bool verbose );
    DwiBundleMapSectionMaskCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapSectionMaskCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapSectionMaskCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapSectionMaskCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputBundleMap,
                  const std::string& fileNameCentroids,
                  const std::string& fileNameSectionMask,
                  const std::string& fileNameDensityMask,
                  float sectionWidth,
                  float resamplingStep,
                  const std::vector< double >& vectorOfOutputResolution,
                  const std::string& outputFormat,
                  bool normalizedDensity,
                  bool ascii,
                  bool verbose );

};


}


#endif
