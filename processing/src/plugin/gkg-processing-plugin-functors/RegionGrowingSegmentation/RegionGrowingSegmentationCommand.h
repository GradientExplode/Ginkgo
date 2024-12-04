#ifndef _gkg_processing_plugin_functors_RegionGrowingSegmentation_RegionGrowingSegmentation_h_
#define _gkg_processing_plugin_functors_RegionGrowingSegmentation_RegionGrowingSegmentation_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>

#include <string>
#include <list>


namespace gkg
{


class RegionGrowingSegmentationCommand :
                           public Command,
                           public Creator2Arg< RegionGrowingSegmentationCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< RegionGrowingSegmentationCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    RegionGrowingSegmentationCommand(
                               int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    RegionGrowingSegmentationCommand(
                               const std::string& fileNameVolumeIn,
                               const std::string& fileNameVolumeSeeds,
                               const std::string& fileNameVolumeOut, 
                               int32_t neighborhoodType,
                               float standardDeviation,
                               float isingFactor,
                               int32_t minimumModificationCount,
                               int32_t maximumFrontProgapagationIterationCount,
                               bool isFrontBasedMeanIntensity,
                               bool ascii,
                               const std::string& format,
                               bool verbose );
    RegionGrowingSegmentationCommand( const Dictionary& parameters );
    virtual ~RegionGrowingSegmentationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RegionGrowingSegmentationCommand,
                              Command, int32_t, char** >;
    friend class Creator1Arg< RegionGrowingSegmentationCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameVolumeIn,
                  const std::string& fileNameVolumeSeeds,
                  const std::string& fileNameVolumeOut, 
                  int32_t neighborhoodType,
                  float standardDeviation,
                  float isingFactor,
                  int32_t minimumModificationCount,
                  int32_t maximumFrontProgapagationIterationCount,
                  bool isFrontBasedMeanIntensity,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

    void doRegionGrowingSegmentation(
                        const Volume< float >& volumeIn,
                        const std::list< Vector3d< int32_t > >& seeds,
                        const int16_t& label,
                        const std::list< Vector3d< int32_t > >& neighborOffsets,
                        const BoundingBox< int32_t >& boundingBox,
                        float standardDeviation,
                        float isingFactor,
                        float isingInFactor,
                        float isingOutFactor,
                        int32_t minimumModificationCount,
                        int32_t maximumFrontProgapagationIterationCount,
                        bool isFrontBasedMeanIntensity,
                        Volume< int16_t >& volumeOut,
                        bool verbose ) const;


};


}


#endif
