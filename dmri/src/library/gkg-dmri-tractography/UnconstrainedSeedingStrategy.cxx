#include <gkg-dmri-tractography/UnconstrainedSeedingStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::UnconstrainedSeedingStrategy::UnconstrainedSeedingStrategy(
        const gkg::Vector3d< double >& roisResolution,
        const gkg::Volume< int16_t >& computingMask,
        const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
        const std::vector< gkg::SeedingStrategy::RegionId >& regionIds, 
        const std::vector< gkg::SeedingStrategy::SamplingType >& samplingTypes,
        const std::vector< int32_t >& seedPerVoxelCounts )
                     : gkg::SeedingStrategy( roisResolution,
                                             computingMask,
                                             computingMaskBoundingBox,
                                             regionIds,
                                             samplingTypes,
                                             seedPerVoxelCounts )
{
}


gkg::UnconstrainedSeedingStrategy::~UnconstrainedSeedingStrategy()
{
}


gkg::SeedingStrategy::RegionId 
gkg::UnconstrainedSeedingStrategy::getRegionId( 
                   const gkg::Vector3d< int32_t >& voxelComputingMask ) const
{

  try
  {

    return ( gkg::SeedingStrategy::RegionId )_computingMask(
                                                           voxelComputingMask );

  }
  GKG_CATCH( "gkg::SeedingStrategy::RegionId "
             "gkg::UnconstrainedSeedingStrategy::getRegionId( "
             "const gkg::Vector3d< int32_t >& voxelComputingMask ) const" );

}
