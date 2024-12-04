#include <gkg-processing-morphology/GeodesicDistanceMapper.h>
#include <gkg-processing-graph/DijkstraAlgorithm.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


template < class IN >
gkg::GeodesicDistanceMapper< IN >::GeodesicDistanceMapper(
                                                const gkg::Volume< IN >& mask,
                                                bool verbose )
                                  : _mask( mask )
{

  try
  {

    if ( verbose )
    {

      std::cout << "computing adjacency graph" << std::flush;

    }

    mask.getSize( _maskSize );
    mask.getResolution( _maskResolution );

    _labelledVertexVolume.reallocate( _maskSize.x + 1,
                                      _maskSize.y + 1,
                                      _maskSize.z + 1 );

    std::list< std::pair< int32_t, float > > neighborhoodNullList;
    
    int32_t label = 0;
    gkg::Volume< int32_t >::iterator
      l = _labelledVertexVolume.begin(),
      le = _labelledVertexVolume.end();
    while ( l != le )
    {

      *l = label;
      ++ label;
      ++ l;

    }

    _adjacencyGraph.resize( _labelledVertexVolume.getSizeXYZ(),
                            neighborhoodNullList );
    

    float dxy = std::sqrt( ( _maskResolution.x * _maskResolution.x ) + 
                            ( _maskResolution.y * _maskResolution.y ) );
    float dxz = std::sqrt( ( _maskResolution.x * _maskResolution.x ) + 
                            ( _maskResolution.z * _maskResolution.z ) );
    float dyz = std::sqrt( ( _maskResolution.y * _maskResolution.y ) + 
                            ( _maskResolution.z * _maskResolution.z ) );
    float dxyz = std::sqrt( ( _maskResolution.x * _maskResolution.x ) + 
                             ( _maskResolution.y * _maskResolution.y ) + 
                             ( _maskResolution.z * _maskResolution.z ) );

    gkg::Vector3d< int32_t > site;
    int32_t corner1 = 0;
    int32_t corner2 = 0;
    int32_t corner3 = 0;
    int32_t corner4 = 0;
    int32_t corner5 = 0;
    int32_t corner6 = 0;
    int32_t corner7 = 0;
    int32_t corner8 = 0;
    for ( site.z = 0; site.z < _maskSize.z; site.z++ )
    {

      for ( site.y = 0; site.y < _maskSize.y; site.y++ )
      {

        for ( site.x = 0; site.x < _maskSize.x; site.x++ )
        {

          if ( _mask( site ) )
          {

            corner1 = _labelledVertexVolume( site.x, site.y, site.z );
            corner2 = _labelledVertexVolume( site.x, site.y, site.z + 1 );
            corner3 = _labelledVertexVolume( site.x, site.y + 1, site.z );
            corner4 = _labelledVertexVolume( site.x, site.y + 1, site.z + 1 );
            corner5 = _labelledVertexVolume( site.x + 1, site.y, site.z );
            corner6 = _labelledVertexVolume( site.x + 1, site.y, site.z + 1 );
            corner7 = _labelledVertexVolume( site.x + 1, site.y + 1, site.z );
            corner8 = _labelledVertexVolume( site.x + 1, site.y + 1,
                                             site.z + 1 );

            setEdge( corner1, corner2, _maskResolution.z );
            setEdge( corner3, corner4, _maskResolution.z );
            setEdge( corner5, corner6, _maskResolution.z );
            setEdge( corner7, corner8, _maskResolution.z );

            setEdge( corner1, corner3, _maskResolution.y );
            setEdge( corner2, corner4, _maskResolution.y );
            setEdge( corner5, corner7, _maskResolution.y );
            setEdge( corner6, corner8, _maskResolution.y );

            setEdge( corner1, corner5, _maskResolution.x );
            setEdge( corner2, corner6, _maskResolution.x );
            setEdge( corner3, corner7, _maskResolution.x );
            setEdge( corner4, corner8, _maskResolution.x );

            setEdge( corner1, corner6, dxz );
            setEdge( corner2, corner5, dxz );
            setEdge( corner3, corner8, dxz );
            setEdge( corner4, corner7, dxz );

            setEdge( corner1, corner7, dxy );
            setEdge( corner2, corner8, dxy );
            setEdge( corner3, corner5, dxy );
            setEdge( corner4, corner6, dxy );

            setEdge( corner1, corner4, dyz );
            setEdge( corner2, corner3, dyz );
            setEdge( corner5, corner8, dyz );
            setEdge( corner6, corner7, dyz );

            setEdge( corner1, corner8, dxyz );
            setEdge( corner2, corner7, dxyz );
            setEdge( corner3, corner6, dxyz );
            setEdge( corner4, corner5, dxyz );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 26 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > "
             "gkg::GeodesicDistanceMapper< IN >::GeodesicDistanceMapper( "
             "const gkg::Volume< IN >& mask, bool verbose )" );

}


template < class IN >
gkg::GeodesicDistanceMapper< IN >::~GeodesicDistanceMapper()
{
}


template < class IN > template < class OUT >
void gkg::GeodesicDistanceMapper< IN >::getDistance(
                                          gkg::Volume< OUT >& distanceMap,
                                          gkg::Vector3d< int32_t > sourceVertex,
                                          bool verbose ) const
{

  try
  {

    if ( verbose )
    {

      std::cout << "computing distance map" << std::flush;

    }

    std::list< int32_t > sourceVertices = getVertexLabels( sourceVertex );

    gkg::DijkstraAlgorithm dijkstraAlgorithm( _adjacencyGraph, sourceVertices );
    const std::vector< float >&
      distances = dijkstraAlgorithm.getDistances();

    distanceMap.reallocate( _maskSize );
    distanceMap.setResolution( _maskResolution );

    gkg::Vector3d< int32_t > site;
    for ( site.z = 0; site.z < _maskSize.z; site.z++ )
    {

      for ( site.y = 0; site.y < _maskSize.y; site.y++ )
      {

        for ( site.x = 0; site.x < _maskSize.x; site.x++ )
        {

          OUT distance = std::numeric_limits< OUT >::max();
          if ( _mask( site ) )
          {

            std::list< int32_t > labels = getVertexLabels( site );
            std::list< int32_t >::const_iterator
              l = labels.begin(),
              le = labels.end();
            while ( l != le )
            {

              if ( distance > ( OUT )distances[ *l ] )
              {

                distance = ( OUT )distances[ *l ];

              }
              ++ l;

            }

          }
          distanceMap( site ) = distance;

        }

      }

    }
          

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN > template < class OUT > "
             "void gkg::GeodesicDistanceMapper< IN >::getDistance( "
             "gkg::Volume< OUT >& distanceMap, "
             "gkg::Vector3d< int32_t > sourceVertex, bool verbose ) const" );

}


template < class IN >
std::list< int32_t > gkg::GeodesicDistanceMapper< IN >::getVertexLabels(
                                 const gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    std::list< int32_t > vertexLabels;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( x = 0; x <= 1; x++ )
    {

      for ( y = 0; y <= 1; y++ )
      {

        for ( z = 0; z <= 1; z++ )
        {

          vertexLabels.push_back( _labelledVertexVolume( site.x + x,
                                                         site.y + y,
                                                         site.z + z ) );

        }

      }

    }

    return vertexLabels;

  }
  GKG_CATCH( "std::list< int32_t > "
             "gkg::GeodesicDistanceMapper::getVertexLabels( "
             "const gkg::Vector3d< int32_t >& site ) const" );
  
}



template < class IN >
void gkg::GeodesicDistanceMapper< IN >::setEdge( int32_t vertex1,
                                                  int32_t vertex2,
                                                  float distance )
{

  try
  {
  
    _adjacencyGraph[ vertex1 ].push_back(
                           std::pair< int32_t, float >( vertex2, distance ) );

    _adjacencyGraph[ vertex2 ].push_back(
                           std::pair< int32_t, float >( vertex1, distance ) );

  }
  GKG_CATCH( "template < class IN > "
             "void gkg::GeodesicDistanceMapper< IN >::setEdge( "
             "int32_t vertex1, int32_t vertex2, float distance )" );
}


template class gkg::GeodesicDistanceMapper< int8_t >;
template class gkg::GeodesicDistanceMapper< uint8_t >;
template class gkg::GeodesicDistanceMapper< int16_t >;
template class gkg::GeodesicDistanceMapper< uint16_t >;
template class gkg::GeodesicDistanceMapper< int32_t >;
template class gkg::GeodesicDistanceMapper< uint32_t >;
template class gkg::GeodesicDistanceMapper< int64_t >;
template class gkg::GeodesicDistanceMapper< uint64_t >;


#define InstanciateGeodesicDistanceMapper( IN, OUT )           \
template void gkg::GeodesicDistanceMapper< IN >::getDistance(  \
                   gkg::Volume< OUT >& distance,               \
                   gkg::Vector3d< int32_t > sourceVertex,      \
                   bool verbose ) const


InstanciateGeodesicDistanceMapper( int8_t, uint8_t );
InstanciateGeodesicDistanceMapper( uint8_t, uint8_t );
InstanciateGeodesicDistanceMapper( int16_t, uint8_t );
InstanciateGeodesicDistanceMapper( uint16_t, uint8_t );
InstanciateGeodesicDistanceMapper( int32_t, uint8_t );
InstanciateGeodesicDistanceMapper( uint32_t, uint8_t );
InstanciateGeodesicDistanceMapper( int64_t, uint8_t );
InstanciateGeodesicDistanceMapper( uint64_t, uint8_t );

InstanciateGeodesicDistanceMapper( int8_t, int8_t );
InstanciateGeodesicDistanceMapper( uint8_t, int8_t );
InstanciateGeodesicDistanceMapper( int16_t, int8_t );
InstanciateGeodesicDistanceMapper( uint16_t, int8_t );
InstanciateGeodesicDistanceMapper( int32_t, int8_t );
InstanciateGeodesicDistanceMapper( uint32_t, int8_t );
InstanciateGeodesicDistanceMapper( int64_t, int8_t );
InstanciateGeodesicDistanceMapper( uint64_t, int8_t );

InstanciateGeodesicDistanceMapper( int8_t, uint16_t );
InstanciateGeodesicDistanceMapper( uint8_t, uint16_t );
InstanciateGeodesicDistanceMapper( int16_t, uint16_t );
InstanciateGeodesicDistanceMapper( uint16_t, uint16_t );
InstanciateGeodesicDistanceMapper( int32_t, uint16_t );
InstanciateGeodesicDistanceMapper( uint32_t, uint16_t );
InstanciateGeodesicDistanceMapper( int64_t, uint16_t );
InstanciateGeodesicDistanceMapper( uint64_t, uint16_t );

InstanciateGeodesicDistanceMapper( int8_t, int16_t );
InstanciateGeodesicDistanceMapper( uint8_t, int16_t );
InstanciateGeodesicDistanceMapper( int16_t, int16_t );
InstanciateGeodesicDistanceMapper( uint16_t, int16_t );
InstanciateGeodesicDistanceMapper( int32_t, int16_t );
InstanciateGeodesicDistanceMapper( uint32_t, int16_t );
InstanciateGeodesicDistanceMapper( int64_t, int16_t );
InstanciateGeodesicDistanceMapper( uint64_t, int16_t );

InstanciateGeodesicDistanceMapper( int8_t, uint32_t );
InstanciateGeodesicDistanceMapper( uint8_t, uint32_t );
InstanciateGeodesicDistanceMapper( int16_t, uint32_t );
InstanciateGeodesicDistanceMapper( uint16_t, uint32_t );
InstanciateGeodesicDistanceMapper( int32_t, uint32_t );
InstanciateGeodesicDistanceMapper( uint32_t, uint32_t );
InstanciateGeodesicDistanceMapper( int64_t, uint32_t );
InstanciateGeodesicDistanceMapper( uint64_t, uint32_t );

InstanciateGeodesicDistanceMapper( int8_t, int32_t );
InstanciateGeodesicDistanceMapper( uint8_t, int32_t );
InstanciateGeodesicDistanceMapper( int16_t, int32_t );
InstanciateGeodesicDistanceMapper( uint16_t, int32_t );
InstanciateGeodesicDistanceMapper( int32_t, int32_t );
InstanciateGeodesicDistanceMapper( uint32_t, int32_t );
InstanciateGeodesicDistanceMapper( int64_t, int32_t );
InstanciateGeodesicDistanceMapper( uint64_t, int32_t );

InstanciateGeodesicDistanceMapper( int8_t, uint64_t );
InstanciateGeodesicDistanceMapper( uint8_t, uint64_t );
InstanciateGeodesicDistanceMapper( int16_t, uint64_t );
InstanciateGeodesicDistanceMapper( uint16_t, uint64_t );
InstanciateGeodesicDistanceMapper( int32_t, uint64_t );
InstanciateGeodesicDistanceMapper( uint32_t, uint64_t );
InstanciateGeodesicDistanceMapper( int64_t, uint64_t );
InstanciateGeodesicDistanceMapper( uint64_t, uint64_t );

InstanciateGeodesicDistanceMapper( int8_t, int64_t );
InstanciateGeodesicDistanceMapper( uint8_t, int64_t );
InstanciateGeodesicDistanceMapper( int16_t, int64_t );
InstanciateGeodesicDistanceMapper( uint16_t, int64_t );
InstanciateGeodesicDistanceMapper( int32_t, int64_t );
InstanciateGeodesicDistanceMapper( uint32_t, int64_t );
InstanciateGeodesicDistanceMapper( int64_t, int64_t );
InstanciateGeodesicDistanceMapper( uint64_t, int64_t );

InstanciateGeodesicDistanceMapper( int8_t, float );
InstanciateGeodesicDistanceMapper( uint8_t, float );
InstanciateGeodesicDistanceMapper( int16_t, float );
InstanciateGeodesicDistanceMapper( uint16_t, float );
InstanciateGeodesicDistanceMapper( int32_t, float );
InstanciateGeodesicDistanceMapper( uint32_t, float );
InstanciateGeodesicDistanceMapper( int64_t, float );
InstanciateGeodesicDistanceMapper( uint64_t, float );

InstanciateGeodesicDistanceMapper( int8_t, double );
InstanciateGeodesicDistanceMapper( uint8_t, double );
InstanciateGeodesicDistanceMapper( int16_t, double );
InstanciateGeodesicDistanceMapper( uint16_t, double );
InstanciateGeodesicDistanceMapper( int32_t, double );
InstanciateGeodesicDistanceMapper( uint32_t, double );
InstanciateGeodesicDistanceMapper( int64_t, double );
InstanciateGeodesicDistanceMapper( uint64_t, double );


#undef InstanciateGeodesicDistanceMapper

