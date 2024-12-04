#ifndef _gkg_dmri_plugin_functors_DwiMultipleSubjectTrackDensityImaging_DwiMultipleSubjectTrackDensityImagingCommand_h_
#define _gkg_dmri_plugin_functors_DwiMultipleSubjectTrackDensityImaging_DwiMultipleSubjectTrackDensityImagingCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiMultipleSubjectTrackDensityImagingCommand :
               public Command,
               public Creator2Arg< DwiMultipleSubjectTrackDensityImagingCommand,
                                   Command,
                                   int32_t,
                                   char** >,
               public Creator1Arg< DwiMultipleSubjectTrackDensityImagingCommand,
                                   Command,
                                   const Dictionary& >
{

  public:

    DwiMultipleSubjectTrackDensityImagingCommand( int32_t argc,
                                                  char* argv[],
                                                  bool loadPlugin = false,
                                                  bool removeFirst = true );
    DwiMultipleSubjectTrackDensityImagingCommand(
             const std::vector< std::string >& fileNameBundleMapDirectories,
             const std::vector< double >& outputResolutionVector,
             const std::vector< std::string >& fileNameInputVolumes,
             const std::string& fileNameTarget,
             const std::vector< std::string >& fileNameReferences,
             const std::vector< std::string >& fileNameDeformations,
             const std::vector< std::string >& fileNameTransform3dForBundleMaps,
             const std::vector< std::string >& fileNameTransform3dForVolume,
             const std::string& fileNameOutputVolume,
             float step,
             int32_t threadCount,
             const std::string& outputFormat,
             bool ascii,
             bool verbose );
    DwiMultipleSubjectTrackDensityImagingCommand(
                                                 const Dictionary& parameters );
    virtual ~DwiMultipleSubjectTrackDensityImagingCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiMultipleSubjectTrackDensityImagingCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiMultipleSubjectTrackDensityImagingCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute(
             const std::vector< std::string >& fileNameBundleMapDirectories,
             const std::vector< double >& outputResolutionVector,
             const std::vector< std::string >& fileNameInputVolumes,
             const std::string& fileNameTarget,
             const std::vector< std::string >& fileNameReferences,
             const std::vector< std::string >& fileNameDeformations,
             const std::vector< std::string >& fileNameTransform3dForBundleMaps,
             const std::vector< std::string >& fileNameTransform3dForVolume,
             const std::string& fileNameOutputVolume,
             float step,
             int32_t threadCount,
             const std::string& outputFormat,
             bool ascii,
             bool verbose );

};


}


#endif
