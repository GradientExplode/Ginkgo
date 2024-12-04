#include <gkg-dmri-tractography/SeedingStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::SeedingStrategy::SeedingStrategy(
        const gkg::Vector3d< double >& roisResolution,
        const gkg::Volume< int16_t >& computingMask,
        const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
        const std::vector< gkg::SeedingStrategy::RegionId >& regionIds, 
        const std::vector< gkg::SeedingStrategy::SamplingType >& samplingTypes,
        const std::vector< int32_t >& seedPerVoxelCounts )
                     : _roisResolution( roisResolution ),
                       _computingMask( computingMask ),
                       _computingMaskBoundingBox( computingMaskBoundingBox )
{

  try
  {

    // allocating a look-up table to store region id to index
    std::vector< gkg::SeedingStrategy::RegionId >::const_iterator
      r = regionIds.begin(),
      re = regionIds.end();
    int32_t maximumRegionId = 0;
    while ( r != re )
    {


      if ( *r > maximumRegionId )
      {

        maximumRegionId = *r;

      }
      ++ r;

    }
    _regionIdToIndex.resize( maximumRegionId + 1, -1 );
    

    // addding +1 to include the outside area of the mask corresponding
    // to OUTSIDE_BRAIN_ID
    _voxelSamplers.resize( regionIds.size() );

    r = regionIds.begin();
    std::vector< gkg::SeedingStrategy::SamplingType >::const_iterator
      s = samplingTypes.begin();
    std::vector< int32_t >::const_iterator
      c = seedPerVoxelCounts.begin();
    int32_t index = 0;
    while ( r != re )
    {

      _regionIdToIndex[ *r ] = index;

      if ( *s == gkg::SeedingStrategy::Cartesian )
      {

        _voxelSamplers[ index ].reset(
                    new gkg::CartesianVoxelSampler< float >( _roisResolution,
                                                               *c ) );


      }
      else if ( *s == gkg::SeedingStrategy::Random )
      {

        _voxelSamplers[ index ].reset( 
                    new gkg::RandomVoxelSampler< float >( _roisResolution,
                                                            *c ) );


      }
      ++ s;
      ++ c;
      ++ index;
      ++ r;

    }

  }
  GKG_CATCH( "gkg::SeedingStrategy::SeedingStrategy( "
             "const gkg::Vector3d< double >& roisResolution, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const std::vector< gkg::SeedingStrategy::RegionId >& "
             "regionIds, "
             "const std::vector< gkg::SeedingStrategy::SamplingType >& "
             "samplingTypes, "
             "const std::vector< int32_t >& seedPerVoxelCounts )" );

}


gkg::SeedingStrategy::~SeedingStrategy()
{
}


int32_t gkg::SeedingStrategy::getSeedCount(
                      const gkg::SeedingStrategy::RegionId& regionId ) const
{

  try
  {

    int32_t index = _regionIdToIndex[ regionId ];
    if ( index >= 0 )
    {

      return _voxelSamplers[ index ]->getPointCount();

    }
    return 0; 

  }
  GKG_CATCH( "int32_t gkg::SeedingStrategy::getSeedCount( "
             "const gkg::SeedingStrategy::RegionId& regionId ) const" );

}


void gkg::SeedingStrategy::getSeed( 
                           const gkg::SeedingStrategy::RegionId& regionId,
                           const gkg::Vector3d< float>& pointOdfs,
                           int32_t seedIndex, 
                           gkg::Vector3d< float>& seed ) const
{

  try
  {

    int32_t index = _regionIdToIndex[ regionId ];
    if ( index >= 0 )
    {

      _voxelSamplers[ index ]->getPoint( pointOdfs, seedIndex, seed );

    }

  }
  GKG_CATCH( "void gkg::SeedingStrategy::getSeed( "
             "const gkg::SeedingStrategy::RegionId& regionId, "
             "const gkg::Vector3d< float>& pointOdfs, "
             "int32_t seedIndex, "
             "gkg::Vector3d< float>& seed ) const" );

}

