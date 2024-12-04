#include <gkg-graphics-QtGL-slicing/CubeZBuffer.h>
#include <gkg-core-cppext/Limits.h>


template < class T >
gkg::CubeZBuffer< T >::CubeZBuffer()
                      : _zMin( std::numeric_limits< T >::lowest() ),
                        _zMax( std::numeric_limits< T >::max() )
{
}


template < class T >
gkg::CubeZBuffer< T >::~CubeZBuffer()
{
}


template < class T >
void gkg::CubeZBuffer< T >::setTransform( const T* m )
{

  int32_t i;

  _zMin = std::numeric_limits< T >::max();
  _zMax = std::numeric_limits< T >::lowest();

  _vz[ 0 ] = m[ 8 ];
  _vz[ 1 ] = m[ 9 ];
  _vz[ 2 ] = m[ 10 ];
  _vz[ 3 ] = m[ 14 ];

  for ( i = 0; i < 8; i++ )
  {

    _zi[ i ] = m[ 14 ] + ( ( ( i & 0x1 ) ? m[ 8 ] : 0.0f ) +
			   ( ( i & 0x2 ) ? m[ 9 ] : 0.0f ) +
			   ( ( i & 0x4 ) ? m[ 10 ] : 0.0f ) );

    if ( _zi[ i ] > _zMax )
    {

      _zMax = _zi[ i ];

    }

    if ( _zi[ i ] < _zMin )
    {

      _zMin = _zi[ i ];

    }

  }

}


template < class T >
const T& gkg::CubeZBuffer< T >::getMinZ() const
{

  return _zMin;

}


template < class T >
const T& gkg::CubeZBuffer< T >::getMaxZ() const
{

  return _zMax;

}


template < class T >
const T& gkg::CubeZBuffer< T >::getVertexZ( int32_t i ) const
{

  return _zi[ i ];

}


template < class T >
int32_t gkg::CubeZBuffer< T >::getKey( T z ) const
{

  int32_t i, key = 0;

  for ( i = 0; i < 8; i++ )
  {

    key += ( _zi[ i ] > z ) ? 1 << i : 0;

  }

  return key;

}


template < class T >
T gkg::CubeZBuffer< T >::getStep( int32_t nbSlices ) const
{

  return ( _zMax - _zMin ) / T( nbSlices );

}


template < class T >
T gkg::CubeZBuffer< T >::getZ( T x, T y, T z ) const
{

  return _vz[ 0 ] * x + _vz[ 1 ] * y + _vz[ 2 ] * z + _vz[ 3 ];

}


template class gkg::CubeZBuffer< float >;
template class gkg::CubeZBuffer< double >;
