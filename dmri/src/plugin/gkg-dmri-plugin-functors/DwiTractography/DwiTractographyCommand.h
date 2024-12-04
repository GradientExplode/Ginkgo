#ifndef _gkg_dmri_plugin_functors_DwiTractography_DwiTractographyCommand_h_
#define _gkg_dmri_plugin_functors_DwiTractography_DwiTractographyCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DwiTractographyCommand : public Command,
                               public Creator2Arg< DwiTractographyCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< DwiTractographyCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    DwiTractographyCommand( int32_t argc,
                            char* argv[],
                            bool loadPlugin = false,
                            bool removeFirst = true );
    DwiTractographyCommand(
               const std::string& fileNameSiteMap,
               const std::string& fileNameOdfs,
               const std::string& fileNameRois,
               const std::string& fileNameMask,
               const std::string& fileNameRoisToOdfsTransform3d,
               const std::string& fileNameMaskToOdfsTransform3d,
               const std::string& fileNameBundleMap,
               const std::string& algorithmType,
               const std::vector< double >& algorithmScalarParameters,
               const std::vector< std::string >& algorithmStringParameters,
               const std::string& seedingStrategyType,
               const std::vector< std::string >& seedingRegionIds,
               const std::vector< std::string >& seedingSamplingTypes,
               const std::vector< int32_t >& seedingSeedPerVoxelCounts,
               const std::string& stoppingStrategyType,
               const std::vector< double >& stoppingStrategyScalarParameters,
               const std::string& startingOrientationStrategyType,
               const std::vector< double >& startingOrientationScalarParameters,
               int32_t outputOrientationCount,
               const std::string& volumeFormat,
               const std::string& bundleMapFormat,
               int32_t stepCount,
               bool ascii,
               bool verbose );
    DwiTractographyCommand( const Dictionary& parameters );
    virtual ~DwiTractographyCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiTractographyCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiTractographyCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute(
               const std::string& fileNameSiteMap,
               const std::string& fileNameOdfs,
               const std::string& fileNameRois,
               const std::string& fileNameMask,
               const std::string& fileNameRoisToOdfsTransform3d,
               const std::string& fileNameMaskToOdfsTransform3d,
               const std::string& fileNameBundleMap,
               const std::string& algorithmType,
               const std::vector< double >& algorithmScalarParameters,
               const std::vector< std::string >& algorithmStringParameters,
               const std::string& seedingStrategyType,
               const std::vector< std::string >& seedingRegionIds,
               const std::vector< std::string >& seedingSamplingTypes,
               const std::vector< int32_t >& seedingSeedPerVoxelCounts,
               const std::string& stoppingStrategyType,
               const std::vector< double >& stoppingStrategyScalarParameters,
               const std::string& startingOrientationStrategyType,
               const std::vector< double >& startingOrientationScalarParameters,
               int32_t outputOrientationCount,
               const std::string& volumeFormat,
               const std::string& bundleMapFormat,
               int32_t stepCount,
               bool ascii,
               bool verbose );

};


}


#endif
