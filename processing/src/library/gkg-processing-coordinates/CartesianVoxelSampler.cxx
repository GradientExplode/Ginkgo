#include <gkg-processing-coordinates/CartesianVoxelSampler.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class T >
gkg::CartesianVoxelSampler< T >::CartesianVoxelSampler(
                                      const gkg::Vector3d< double >& resolution,
                                      int32_t pointCount )
                                : gkg::VoxelSampler< T >( resolution,
                                                          pointCount ),
                                  _offsets( pointCount )
{

  try
  {

    int32_t integerCbrt = ( int32_t )std::floor( std::pow( ( double )pointCount,
                                                           1.0 / 3.0 ) );
    int32_t pointCountX = integerCbrt;
    int32_t pointCountY = integerCbrt;
    int32_t pointCountZ = integerCbrt;

    int32_t deltaPointCount = std::abs( pointCountX *
                                        pointCountY *
                                        pointCountZ - pointCount );

    int32_t newdeltaPointCount = std::abs( pointCountX *
                                           pointCountY *
                                           ( pointCountZ + 1 ) - pointCount );
    if ( newdeltaPointCount < deltaPointCount )
    {

      ++ pointCountZ;
      deltaPointCount = newdeltaPointCount;

    }

    newdeltaPointCount = std::abs( pointCountX *
                                   ( pointCountY + 1 ) *
                                   ( pointCountZ + 1 ) - pointCount );
    if ( newdeltaPointCount < deltaPointCount )
    {

      ++ pointCountY;
      deltaPointCount = newdeltaPointCount;

    }

    newdeltaPointCount = std::abs( ( pointCountX + 1 ) *
                                   ( pointCountY + 1 ) *
                                   ( pointCountZ + 1 ) - pointCount );
    if ( newdeltaPointCount < deltaPointCount )
    {

      ++ pointCountX;
      deltaPointCount = newdeltaPointCount;

    }

    int32_t newPointCount = pointCountX * pointCountY * pointCountZ;
    if ( newPointCount != pointCount )
    {

      this->_offsets.resize( newPointCount );

    }

    int32_t x, y, z;
    int32_t count = 0;
    for ( z = 0; z < pointCountZ; z++ )
    {

      for ( y = 0; y < pointCountY; y++ )
      {

        for ( x = 0; x < pointCountX; x++ )
        {

          gkg::Vector3d< T >& offset = this->_offsets[ count ];

          offset.x = ( -0.5 + 0.5 / ( double )pointCountX +
                       ( double )x / ( double )pointCountX ) *
                     this->_resolution.x;
          offset.y = ( -0.5 + 0.5 / ( double )pointCountY +
                       ( double )y / ( double )pointCountY ) *
                     this->_resolution.y;
          offset.z = ( -0.5 + 0.5 / ( double )pointCountZ +
                       ( double )z / ( double )pointCountZ ) *
                     this->_resolution.z;
          ++ count;

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::CartesianVoxelSampler< T >::CartesianVoxelSampler( "
             "const gkg::Vector3d< double >& resolution, "
             "int32_t pointCount )" );


}


template < class T >
gkg::CartesianVoxelSampler< T >::~CartesianVoxelSampler()
{
}


template < class T >
void gkg::CartesianVoxelSampler< T >::getPoint(
                                         const gkg::Vector3d< T >& origin,
                                         int32_t pointIndex,
                                         gkg::Vector3d< T >& destination ) const
{

  try
  {

    if ( ( pointIndex >= this->_pointCount ) || ( pointIndex < 0 ) )
    {

      throw std::runtime_error( "not a valid point index" );

    }
    const gkg::Vector3d< T >& offset = this->_offsets[ pointIndex ];
    
    destination.x = origin.x + offset.x;
    destination.y = origin.y + offset.y;
    destination.z = origin.z + offset.z;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::CartesianVoxelSampler< T >::getPoint( "
             "const gkg::Vector3d< T >& origin, "
             "int32_t pointIndex, "
             "gkg::Vector3d< T >& destination ) const" );

}


template class gkg::CartesianVoxelSampler< float >;
template class gkg::CartesianVoxelSampler< double >;

