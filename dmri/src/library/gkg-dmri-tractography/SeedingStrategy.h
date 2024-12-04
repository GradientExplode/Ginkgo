#ifndef _gkg_dmri_tractography_SeedingStrategy_h_
#define _gkg_dmri_tractography_SeedingStrategy_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/CartesianVoxelSampler.h>
#include <gkg-processing-coordinates/RandomVoxelSampler.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


#define OUTSIDE_BRAIN_ID 0


class SeedingStrategy
{

  public:

    typedef int32_t RegionId;

    enum SamplingType
    {

      Cartesian,
      Random

    };

    virtual ~SeedingStrategy();

    int32_t getSeedCount( const RegionId& regionId ) const;
    void getSeed( const RegionId& regionId,
                   const Vector3d< float>& pointOdfs,
                   int32_t seedIndex, 
                   Vector3d< float>& seed ) const;

    virtual RegionId getRegionId(
                   const Vector3d< int32_t >& voxelComputingMask ) const = 0;

  protected:

    SeedingStrategy(
                  const Vector3d< double >& roisResolution,
                  const Volume< int16_t >& computingMask,
                  const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
                  const std::vector< RegionId >& regionIds, 
                  const std::vector< SamplingType >& samplingTypes,
                  const std::vector< int32_t >& seedPerVoxelCounts );

    Vector3d< double > _roisResolution;
    const Volume< int16_t >& _computingMask;
    const gkg::BoundingBox< int32_t >& _computingMaskBoundingBox;
    std::vector< int32_t > _regionIdToIndex;
    std::vector< RCPointer< VoxelSampler< float > > > _voxelSamplers;

};


}


#endif
