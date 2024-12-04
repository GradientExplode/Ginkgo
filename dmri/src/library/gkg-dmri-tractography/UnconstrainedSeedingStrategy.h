#ifndef _gkg_dmri_tractography_UnconstrainedSeedingStrategy_h_
#define _gkg_dmri_tractography_UnconstrainedSeedingStrategy_h_


#include <gkg-dmri-tractography/SeedingStrategy.h>


namespace gkg
{


class UnconstrainedSeedingStrategy : public SeedingStrategy
{

  public:

    enum SamplingType
    {

      Cartesian,
      Random

    };


    UnconstrainedSeedingStrategy(
            const Vector3d< double >& roisResolution,
            const Volume< int16_t >& computingMask,
            const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
            const std::vector< SeedingStrategy::RegionId >& regionIds, 
            const std::vector< SeedingStrategy::SamplingType >& samplingTypes,
            const std::vector< int32_t >& seedPerVoxelCounts );
    virtual ~UnconstrainedSeedingStrategy();

    SeedingStrategy::RegionId getRegionId( 
                       const Vector3d< int32_t >& voxelComputingMask ) const;

};

}


#endif
