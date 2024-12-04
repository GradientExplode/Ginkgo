#include <gkg-processing-mask/MaskPartitioner.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <list>
#include <metis.h>
#include <limits>


template < class IN, class OUT >
gkg::MaskPartitioner< IN, OUT >::MaskPartitioner(
             const std::list< gkg::Vector3d< int32_t > >& neighborhoodOffsets,
             const gkg::Volume< IN >& mask )
                                 : _mask( mask )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting mask size and resolution
    ////////////////////////////////////////////////////////////////////////////

    _mask.getSize( _maskSize );
    _mask.getResolution( _maskResolution );

    gkg::BoundingBox< int32_t > boundingBox( _mask );

    ////////////////////////////////////////////////////////////////////////////
    // computing graph for metis
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< int32_t > labelledVolume( _maskSize );
    labelledVolume.setResolution( _maskResolution );

    int32_t label = 0;
    int64_t voxelInTheMaskCount = 0;

    gkg::Volume< int32_t >::iterator
      l = labelledVolume.begin(),
      le = labelledVolume.end();
    typename gkg::Volume< IN >::const_iterator
      m = _mask.begin();
    while ( l != le )
    {

      if ( *m )
      {

        *l = label;
        ++ label;
        ++ voxelInTheMaskCount;

      }
      else
      {

        *l = -1;

      }
      ++ m;
      ++l;

    }

    _vertexCount = label;

    if ( voxelInTheMaskCount >
         ( int64_t )std::numeric_limits< int32_t >::max() )
    {

      throw std::runtime_error( "voxel in the mask number is higher "
                                 "than METIS capacity" );

    }

    std::list< int32_t > adjList;
    std::list< int32_t > adjncyList;
    int32_t vertexCounter = 0;
    int64_t vertexCounter64 = 0;


    gkg::Vector3d< int32_t > neighbor;
    gkg::Vector3d< int32_t > site;
    for ( site.z = 0; site.z < _maskSize.z; site.z++ )
    {

      for ( site.y = 0; site.y < _maskSize.y; site.y++ )
      {

        for ( site.x = 0; site.x < _maskSize.x; site.x++ )
        {

          if ( _mask( site ) )
          {

            adjList.push_back( vertexCounter );
            std::list< gkg::Vector3d< int32_t > >::const_iterator
              o = neighborhoodOffsets.begin(),
              oe = neighborhoodOffsets.end();
            while ( o != oe )
            {

              neighbor = site + *o;

              if ( boundingBox.contains( neighbor ) && _mask( neighbor ) )
              {

                ++ vertexCounter;
                ++ vertexCounter64;
                adjncyList.push_back( labelledVolume( neighbor ) );

              }
              ++ o;

            }

          }

        }

      }

    }
    adjList.push_back( vertexCounter );

    if ( vertexCounter64 > ( int64_t )std::numeric_limits< int32_t >::max() )
    {

      throw std::runtime_error( "vertex count in the mask is higher "
                                 "than METIS capacity" );

    }

    _adjTab = std::vector< int32_t >( adjList.begin(), adjList.end() );
    _adjncyTab = std::vector< int32_t >( adjncyList.begin(), adjncyList.end() );

    if ( ( int32_t )adjncyList.size() !=  _adjTab.back() )
    {

      throw std::runtime_error( "corrupted graph structures" );

    }
  
  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::MaskPartitioner< IN, OUT >::MaskPartitioner( "
             "const std::list< gkg::Vector3d< int32_t > >& "
             "neighborhoodOffsets, "
             "const gkg::Volume< IN >& mask )" );

}


template < class IN, class OUT >
gkg::MaskPartitioner< IN, OUT >::~MaskPartitioner()
{
}


template < class IN, class OUT >
std::string gkg::MaskPartitioner< IN, OUT >::getName() const
{

  return getStaticName();

}


template < class IN, class OUT >
typename gkg::MaskPartitioner< IN, OUT >::PartitionStatistics
gkg::MaskPartitioner< IN, OUT >::computePartition(
                                     gkg::Volume< OUT >& outputVolume,
                                     float& loadImbalanceTolerance,
                                     int32_t& partCount,
                                     int32_t& runCount,
                                     bool& totalCommunicationVolumeMinimization,
                                     int32_t& startingRandomSeed ) const
{

  try
  {

    int32_t vertexCount = _vertexCount;
    int32_t constraintCount = 1;

    int32_t options[ METIS_NOPTIONS ];
    METIS_SetDefaultOptions(options);

    options[ METIS_OPTION_DBGLVL ] = 0; // 1 at least for debugging
    options[ METIS_OPTION_NCUTS ] = runCount;
    options[ METIS_OPTION_SEED ] = startingRandomSeed;

    if ( totalCommunicationVolumeMinimization ) // CAUTION needs more time
    {

      options[ METIS_OPTION_OBJTYPE ] = METIS_OBJTYPE_VOL;

    }
    else  // edge cut minimization
    {

      options[ METIS_OPTION_OBJTYPE ] = METIS_OBJTYPE_CUT;

    }

    int32_t edgeCut = 0;

    std::vector< int32_t > parts( vertexCount );

    int32_t outputState = METIS_PartGraphKway(
                  &vertexCount,       // nb of vertices in the graph
                  &constraintCount,   // nb of balancing constraints
                  &_adjTab[ 0 ],      // adjency structure of the graph
                  &_adjncyTab[ 0 ],   // adjency structure of the graph
                  0,                  // vertices weight (NULL for the moment)
                  0,                  // not concerned
                  0,                  // weight of the egde (NULL here)
                  &partCount,         // nb of partitions
                  0,                  // partition balancing weight
                                      // (0 here to equally divided partitions)
                  &loadImbalanceTolerance,   // load imbalance
                  options,
                  &edgeCut,           // output
                  &parts[ 0 ] );      //output
    
    if ( outputState == METIS_ERROR_INPUT )
    {

      throw std::runtime_error( "Metis exited with status "
                                 "'METIS_ERROR_INPUT'" );

    }
    else if ( outputState == METIS_ERROR_MEMORY )
    {

      throw std::runtime_error( "Metis exited with status "
                                 "'METIS_ERROR_MEMORY'" );

    }
    else if ( outputState == METIS_ERROR )
    {

      // other type of error :(
      throw std::runtime_error( "Metis exited with status "
                                 "'METIS_ERROR'" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output volume
    ////////////////////////////////////////////////////////////////////////////

    outputVolume.reallocate( _maskSize );
    outputVolume.setResolution( _maskResolution );

    std::vector< int32_t > partSizes( partCount, 0 );

    int32_t voxelInMaskIndex = 0;

    typename gkg::Volume< OUT >::iterator
      o = outputVolume.begin(),
      oe = outputVolume.end();
    typename gkg::Volume< IN >::const_iterator
      m = _mask.begin();
    while ( o != oe )
    {

      if ( *m )
      {

        ++ partSizes[ parts[ voxelInMaskIndex ] ];
        *o = ( OUT )( parts[ voxelInMaskIndex ] + 1 );
        ++ voxelInMaskIndex;

      }
      else
      {

        *o = 0;

      }
      ++ m;
      ++o;

    }


    // computing the number of voxels in the biggest regions
    int32_t maximumPartSize = 0;
    int32_t minimumPartSize = std::numeric_limits< int32_t >::max();

    std::vector< int32_t >::const_iterator
      s = partSizes.begin(),
      se = partSizes.end();
    while ( s != se )
    {

     if ( *s > maximumPartSize )
     {

        maximumPartSize = *s ;

      }
      if ( *s < minimumPartSize )
      {

        minimumPartSize = *s;

      }
      ++ s;

    }

    // computing the ratio btw the expected nb of voxel and the biggest regions
    // number of voxel 
    float ratio = ( ( float ) maximumPartSize ) / 
                  ( ( float ) vertexCount / ( float )partCount );

    return gkg::MaskPartitioner< IN, OUT >::PartitionStatistics( ratio,
                                                                 edgeCut );

  }
  GKG_CATCH( "template < class IN, class OUT > "
            "gkg::MaskPartitioner< IN, OUT >::PartitionStatistics "
            "gkg::MaskPartitioner< IN, OUT >::computePartition( "
            "gkg::Volume< OUT >& outputVolume, "
            "float& loadImbalanceTolerance, "
            "int32_t& partCount, "
            "int32_t& runCount, "
            "int32_t& startingRandomSeed ) const" );

}


template < class IN, class OUT >
std::string gkg::MaskPartitioner< IN, OUT >::getStaticName()
{

  try
  {

    return gkg::TypeOf< IN >::getName() + " -> " +
             gkg::TypeOf< OUT >::getName() +
             " mask partioner algorithm";

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "std::string gkg::MaskPartitioner< IN, OUT >::getStaticName()" );

}


// force instanciation for standard types

template class gkg::MaskPartitioner< int8_t, uint8_t >;
template class gkg::MaskPartitioner< int8_t, int16_t >;
template class gkg::MaskPartitioner< int8_t, float >;
template class gkg::MaskPartitioner< int8_t, double >;

template class gkg::MaskPartitioner< uint8_t, uint8_t >;
template class gkg::MaskPartitioner< uint8_t, int16_t >;
template class gkg::MaskPartitioner< uint8_t, float >;
template class gkg::MaskPartitioner< uint8_t, double >;

template class gkg::MaskPartitioner< int16_t, uint8_t >;
template class gkg::MaskPartitioner< int16_t, int16_t >;
template class gkg::MaskPartitioner< int16_t, float >;
template class gkg::MaskPartitioner< int16_t, double >;

template class gkg::MaskPartitioner< uint16_t, uint8_t >;
template class gkg::MaskPartitioner< uint16_t, int16_t >;
template class gkg::MaskPartitioner< uint16_t, float >;
template class gkg::MaskPartitioner< uint16_t, double >;

template class gkg::MaskPartitioner< int32_t, uint8_t >;
template class gkg::MaskPartitioner< int32_t, int16_t >;
template class gkg::MaskPartitioner< int32_t, float >;
template class gkg::MaskPartitioner< int32_t, double >;

template class gkg::MaskPartitioner< uint32_t, uint8_t >;
template class gkg::MaskPartitioner< uint32_t, int16_t >;
template class gkg::MaskPartitioner< uint32_t, float >;
template class gkg::MaskPartitioner< uint32_t, double >;

template class gkg::MaskPartitioner< int64_t, uint8_t >;
template class gkg::MaskPartitioner< int64_t, int16_t >;
template class gkg::MaskPartitioner< int64_t, float >;
template class gkg::MaskPartitioner< int64_t, double >;

template class gkg::MaskPartitioner< uint64_t, uint8_t >;
template class gkg::MaskPartitioner< uint64_t, int16_t >;
template class gkg::MaskPartitioner< uint64_t, float >;
template class gkg::MaskPartitioner< uint64_t, double >;

template class gkg::MaskPartitioner< float, uint8_t >;
template class gkg::MaskPartitioner< float, int16_t >;
template class gkg::MaskPartitioner< float, float >;
template class gkg::MaskPartitioner< float, double >;

template class gkg::MaskPartitioner< double, uint8_t >;
template class gkg::MaskPartitioner< double, int16_t >;
template class gkg::MaskPartitioner< double, float >;
template class gkg::MaskPartitioner< double, double >;

