#include <gkg-dmri-tractography/AnatomicallyConstrainedSeedingStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-core-exception/Exception.h>


gkg::AnatomicallyConstrainedSeedingStrategy::
                                         AnatomicallyConstrainedSeedingStrategy(
        const gkg::Vector3d< double >& roisResolution,
        const gkg::Volume< int16_t >& computingMask,
        const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
        const std::vector< gkg::SeedingStrategy::RegionId >& regionIds, 
        const std::vector< gkg::SeedingStrategy::SamplingType >& samplingTypes,
        const std::vector< int32_t >& seedPerVoxelCounts )
                                           : gkg::SeedingStrategy(
                                               roisResolution,
                                               computingMask,
                                               computingMaskBoundingBox,
                                               regionIds,
                                               samplingTypes,
                                               seedPerVoxelCounts )
{

  try
  {

    _neighborOffsets = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_26 );

  }
  GKG_CATCH( "gkg::AnatomicallyConstrainedSeedingStrategy::AnatomicallyConstrainedSeedingStrategy( "
             "const gkg::Vector3d< double >& roisResolution, "
             "const gkg::Volume< int16_t >& computingMask, "
             "const gkg::BoundingBox< int32_t >& computingMaskBoundingBox, "
             "const std::vector< gkg::gkg::SeedingStrategy::RegionId >& "
             "regionIds, "
             "const std::vector< gkg::SeedingStrategy::SamplingType >& "
             "samplingTypes, "
             "const std::vector< int32_t >& seedPerVoxelCounts )" );

}


gkg::AnatomicallyConstrainedSeedingStrategy::
                                       ~AnatomicallyConstrainedSeedingStrategy()
{
}


gkg::SeedingStrategy::RegionId 
gkg::AnatomicallyConstrainedSeedingStrategy::getRegionId( 
                   const gkg::Vector3d< int32_t >& voxelComputingMask ) const
{

  try
  {

    gkg::SeedingStrategy::RegionId
      regionId = _computingMask( voxelComputingMask );

    if ( regionId == WHITE_MATTER_ID )
    {

      gkg::Vector3d< int32_t > neighborVoxelComputingMask;
      std::list< gkg::Vector3d< int32_t > >::const_iterator
        o = _neighborOffsets.begin(),
        oe = _neighborOffsets.end();
      while ( o != oe )
      {

        neighborVoxelComputingMask = voxelComputingMask + *o;
        if ( _computingMaskBoundingBox.contains( neighborVoxelComputingMask ) )
        {

          if ( _computingMask( neighborVoxelComputingMask ) == 
                CORTEX_ID )
          {

            regionId =  CORTEX_WHITE_MATTER_INTERFACE_ID;
            break;

          }
          else if ( _computingMask( neighborVoxelComputingMask ) == 
                      DEEP_GRAY_MATTER_ID )
          {

            regionId = DEEP_GRAY_MATTER_WHITE_MATTER_INTERFACE_ID;
            break;

          }
          else if ( _computingMask( neighborVoxelComputingMask ) == 
                      CSF_ID )
          {

            regionId = CSF_WHITE_MATTER_INTERFACE_ID;
            break;

          }

        }
        ++ o;

      }

    }
    else if ( regionId == CORTEX_ID )
    {

      gkg::Vector3d< int32_t > neighborVoxelComputingMask;
      std::list< gkg::Vector3d< int32_t > >::const_iterator
        o = _neighborOffsets.begin(),
        oe = _neighborOffsets.end();
      while ( o != oe )
      {

        neighborVoxelComputingMask = voxelComputingMask + *o;
        if ( _computingMaskBoundingBox.contains( neighborVoxelComputingMask ) )
        {

          if ( _computingMask( neighborVoxelComputingMask ) == 
                OUTSIDE_BRAIN_ID )
          {

            regionId =  PIAL_CORTEX_INTERFACE_ID;
            break;

          }
          else if ( _computingMask( neighborVoxelComputingMask ) == 
                      WHITE_MATTER_ID )
          {

            regionId = CORTEX_WHITE_MATTER_INTERFACE_ID;
            break;

          }

        }
        ++ o;

      }

    }

    return regionId;

  }
  GKG_CATCH( "gkg::SeedingStrategy::RegionId "
             "gkg::AnatomicallyConstrainedSeedingStrategy::getRegionType( "
             "const gkg::Vector3d< int32_t >& voxelComputingMask ) const" );

}
