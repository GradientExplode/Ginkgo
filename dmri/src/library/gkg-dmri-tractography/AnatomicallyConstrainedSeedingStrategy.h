#ifndef _gkg_dmri_tractography_AnatomicallyConstrainedSeedingStrategy_h_
#define _gkg_dmri_tractography_AnatomicallyConstrainedSeedingStrategy_h_


#include <gkg-dmri-tractography/SeedingStrategy.h>
#include <list>


namespace gkg
{


#define CORTEX_ID 1
#define WHITE_MATTER_ID 2
#define DEEP_GRAY_MATTER_ID 3
#define CSF_ID 4

#define PIAL_CORTEX_INTERFACE_ID   5
#define CORTEX_WHITE_MATTER_INTERFACE_ID 6
#define DEEP_GRAY_MATTER_WHITE_MATTER_INTERFACE_ID 7
#define CSF_WHITE_MATTER_INTERFACE_ID 8



class AnatomicallyConstrainedSeedingStrategy : public SeedingStrategy
{

  public:

    AnatomicallyConstrainedSeedingStrategy(
            const Vector3d< double >& roisResolution,
            const Volume< int16_t >& computingMask,
            const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
            const std::vector< SeedingStrategy::RegionId >& regionIds, 
            const std::vector< SeedingStrategy::SamplingType >& samplingTypes,
            const std::vector< int32_t >& seedPerVoxelCounts );
    virtual ~AnatomicallyConstrainedSeedingStrategy();

    SeedingStrategy::RegionId getRegionId( 
                       const Vector3d< int32_t >& voxelComputingMask ) const;

  protected:

    std::list< Vector3d< int32_t > > _neighborOffsets;

};


}


#endif
