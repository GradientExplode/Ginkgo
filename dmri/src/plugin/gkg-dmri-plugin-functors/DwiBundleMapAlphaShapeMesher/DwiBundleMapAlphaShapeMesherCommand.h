#ifndef _gkg_dmri_plugin_functors_DwiBundleMapAlphaShapeMesher_DwiBundleMapAlphaShapeMesherCommand_h_
#define _gkg_dmri_plugin_functors_DwiBundleMapAlphaShapeMesher_DwiBundleMapAlphaShapeMesherCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiBundleMapAlphaShapeMesherCommand :
                        public Command,
                        public Creator2Arg< DwiBundleMapAlphaShapeMesherCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< DwiBundleMapAlphaShapeMesherCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    DwiBundleMapAlphaShapeMesherCommand( int32_t argc,
                                         char* argv[],
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    DwiBundleMapAlphaShapeMesherCommand(
                                 const std::string& fileNameBundleMap,
                                 const std::string& fileNameOut,
                                 const std::string& fileNamePointCloud,
                                 const std::string& fileNamePointCloudAtSurface,
                                 float step,
                                 const std::string& outputFormat,
                                 bool singleFile,
                                 const bool& useThickFascicles,
                                 const float& fascicleRadius,
                                 const int32_t& sphereOrientationCount,
                                 bool removingOutliers,
                                 int32_t neighborCount,
                                 float outlierDistanceScale,
                                 bool simplifyingPointSet,
                                 bool smoothingPointSet,
                                 bool subdividingMesh,
                                 int32_t subdivisionIterationCount,
                                 const std::string& intermediateFileName,
                                 bool ascii,
                                 bool verbose );
    DwiBundleMapAlphaShapeMesherCommand( const Dictionary& parameters );
    virtual ~DwiBundleMapAlphaShapeMesherCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiBundleMapAlphaShapeMesherCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiBundleMapAlphaShapeMesherCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameBundleMap,
                  const std::string& fileNameOut,
                  const std::string& fileNamePointCloud,
                  const std::string& fileNamePointCloudAtSurface,
                  float step,
                  const std::string& outputFormat,
                  bool singleFile,
                  const bool& useThickFascicles,
                  const float& fascicleRadius,
                  const int32_t& sphereOrientationCount,
                  bool removingOutliers,
                  int32_t neighborCount,
                  float outlierDistanceScale,
                  bool simplifyingPointSet,
                  bool smoothingPointSet,
                  bool subdividingMesh,
                  int32_t subdivisionIterationCount,
                  const std::string& intermediateFileName,
                  bool ascii,
                  bool verbose );

};


}


#endif
