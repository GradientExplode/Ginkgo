#include <gkg-graphics-QtGL-slicing/Slicing.h>



template < class T >
T gkg::Slicing< T >::_vertex[ 8 ][ 3 ] = 
  {

    { 0.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 1.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 1.0f },
    { 0.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f }

  };


template < class T >
int32_t gkg::Slicing< T >::_edge[ 12 ][ 2 ] =
  {

    { 0, 1 },
    { 4, 5 },
    { 2, 3 },
    { 6, 7 },
    { 0, 2 },
    { 4, 6 },
    { 1, 3 },
    { 5, 7 },
    { 0, 4 },
    { 2, 6 },
    { 1, 5 },
    { 3, 7 }

  };


template < class T >
int32_t gkg::Slicing< T >::_intersection[ 256 ][ 8 ] =
  {

    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  4,  8, -1, -1, -1, -1, -1 },
    {  0, 10,  6, -1, -1, -1, -1, -1 },
    {  4,  8, 10,  6, -1, -1, -1, -1 },
    {  4,  2,  9, -1, -1, -1, -1, -1 },
    {  0,  2,  9,  8, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  8,  9,  2,  6, 10, -1, -1, -1 },
    {  6, 11,  2, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0, 10, 11,  2, -1, -1, -1, -1 },
    { 10,  8,  4,  2, 11, -1, -1, -1 },
    {  4,  6, 11,  9, -1, -1, -1, -1 },
    {  9, 11,  6,  0,  8, -1, -1, -1 },
    { 11, 10,  0,  4,  9, -1, -1, -1 },
    {  8, 10, 11,  9, -1, -1, -1, -1 },
    {  8,  5,  1, -1, -1, -1, -1, -1 },
    {  0,  4,  5,  1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  4,  6, 10,  1,  5, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  1,  5,  9,  2, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1, 10,  6,  2,  9,  5, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { 11,  9,  5,  1, 10, -1, -1, -1 },
    { 10,  1,  7, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  1,  7,  6, -1, -1, -1, -1 },
    {  6,  7,  1,  8,  4, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  2, 11,  7,  1, -1, -1, -1 },
    {  1,  7, 11,  2,  4,  8, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  9,  8,  1,  7, 11, -1, -1, -1 },
    {  5,  7, 10,  8, -1, -1, -1, -1 },
    {  5,  4,  0, 10,  7, -1, -1, -1 },
    {  7,  5,  8,  0,  6, -1, -1, -1 },
    {  4,  5,  7,  6, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  7,  6,  2,  9,  5, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  5,  7, 11,  2,  4, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  5,  7, 11,  9, -1, -1, -1, -1 },
    {  9,  3,  5, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  2,  3,  5,  4, -1, -1, -1, -1 },
    {  2,  0,  8,  5,  3, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  4,  5,  3, 11,  6, -1, -1, -1 },
    {  0,  6, 11,  3,  5,  8, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { 10, 11,  3,  5,  8, -1, -1, -1 },
    {  1,  8,  9,  3, -1, -1, -1, -1 },
    {  1,  3,  9,  4,  0, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  3,  2,  4,  8,  1, -1, -1, -1 },
    {  0,  2,  3,  1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  3,  1, 10,  6,  2, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1,  0,  6, 11,  3, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1, 10, 11,  3, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  8, 10,  7,  3,  9, -1, -1, -1 },
    {  0,  4,  9,  3,  7, 10, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  6,  4,  9,  3,  7, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  2,  3,  7, 10,  0, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  2,  3,  7,  6, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { 11,  3,  7, -1, -1, -1, -1, -1 },
    { 11,  7,  3, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  2,  6,  7,  3, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  2,  0, 10,  7,  3, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  6,  7,  3,  9,  4, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0, 10,  7,  3,  9,  4, -1, -1 },
    {  8,  9,  3,  7, 10, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1,  3, 11, 10, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1,  3, 11,  6,  0, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  3,  2,  6, 10,  1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  1,  3,  2, -1, -1, -1, -1 },
    {  3,  1,  8,  4,  2, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1,  0,  4,  9,  3, -1, -1, -1 },
    {  1,  3,  9,  8, -1, -1, -1, -1 },
    { 10,  8,  5,  3, 11, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  8,  5,  3, 11,  6, -1, -1 },
    {  4,  6, 11,  3,  5, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  2,  3,  5,  8,  0, -1, -1, -1 },
    {  2,  4,  5,  3, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  9,  5,  3, -1, -1, -1, -1, -1 },
    {  5,  9, 11,  7, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  5,  4,  2, 11,  7, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  7,  5,  9,  2,  6, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  4,  6,  7,  5, -1, -1, -1, -1 },
    {  7,  6,  0,  8,  5, -1, -1, -1 },
    {  5,  7, 10,  0,  4, -1, -1, -1 },
    {  5,  8, 10,  7, -1, -1, -1, -1 },
    {  9, 11,  7,  1,  8, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1,  8,  4,  2, 11,  7, -1, -1 },
    {  0,  1,  7, 11,  2, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  6,  4,  8,  1,  7, -1, -1, -1 },
    {  0,  6,  7,  1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { 10,  7,  1, -1, -1, -1, -1, -1 },
    { 11, 10,  1,  5,  9, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  1,  5,  9,  2,  6, 10, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  2,  9,  5,  1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  4,  5,  1, 10,  6, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  1,  5,  4, -1, -1, -1, -1 },
    {  8,  1,  5, -1, -1, -1, -1, -1 },
    {  8,  9, 11, 10, -1, -1, -1, -1 },
    { 11,  9,  4,  0, 10, -1, -1, -1 },
    {  9,  8,  0,  6, 11, -1, -1, -1 },
    {  4,  9, 11,  6, -1, -1, -1, -1 },
    { 10, 11,  2,  4,  8, -1, -1, -1 },
    {  0,  2, 11, 10, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  6,  2, 11, -1, -1, -1, -1, -1 },
    {  8, 10,  6,  2,  9, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  8,  9,  2, -1, -1, -1, -1 },
    {  4,  9,  2, -1, -1, -1, -1, -1 },
    {  4,  6, 10,  8, -1, -1, -1, -1 },
    {  0,  6, 10, -1, -1, -1, -1, -1 },
    {  0,  8,  4, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1 }

  };


template < class T >
gkg::Slicing< T >::Slicing()
                  : _maxNbSlices( 512 )
{
}


template < class T >
void gkg::Slicing< T >::setMaxSlices( int32_t nbSlices )
{

  _maxNbSlices = nbSlices;

}


template < class T >
std::list< gkg::Vector3d< T > >
gkg::Slicing< T >::getSlice( const T* m, T z )
{

  _slices.clear();
  _cubeZBuffer.setTransform( m );

  if ( z >= _cubeZBuffer.getMinZ() && z <= _cubeZBuffer.getMaxZ() )
  {

    getOneSlice( z );
    return _slices.find( z )->second;

  }

  return std::list< gkg::Vector3d< T > >();

}


template < class T >
std::map< T, std::list< gkg::Vector3d< T > > >&
gkg::Slicing< T >::getSlices( const T* m )
{

  _slices.clear();
  _cubeZBuffer.setTransform( m );

  T step = _cubeZBuffer.getStep( _maxNbSlices );
  T z = _cubeZBuffer.getMinZ();

  while ( z <= _cubeZBuffer.getMaxZ() )
  {

    getOneSlice( z );
    z += step;

  }

  return _slices;

}


template < class T >
std::map< T, std::list< gkg::Vector3d< T > > >&
gkg::Slicing< T >::getSlab( const T* m, T z, int32_t nbSlices )
{

  _slices.clear();

  if ( nbSlices <= _maxNbSlices )
  {

    _cubeZBuffer.setTransform( m );
    T step = _cubeZBuffer.getStep( _maxNbSlices );
    T z0 = z - step * T( nbSlices / 2 );
    T z1 = z + step * T( nbSlices / 2 );

    if ( z0 < _cubeZBuffer.getMinZ() )
    {

      z0 = _cubeZBuffer.getMinZ();

    }

    if ( z1 > _cubeZBuffer.getMaxZ() )
    {

      z1 = _cubeZBuffer.getMaxZ();

    }

    while ( z0 <= z1 )
    {

      getOneSlice( z0 );
      z0 += step;

    }

  }

  return _slices;

}


template < class T >
void gkg::Slicing< T >::getOneSlice( T z )
{

  int32_t e, i, key = _cubeZBuffer.getKey( z );

  if ( _intersection[ key ][ 0 ] >= 0 )
  {

    std::list< gkg::Vector3d< T > > vList;
    i = 0;

    while ( ( e = _intersection[ key ][ i++ ] ) >= 0 )
    {

      T z0 = _cubeZBuffer.getVertexZ( _edge[ e ][ 0 ] );
      T z1 = _cubeZBuffer.getVertexZ( _edge[ e ][ 1 ] );

      switch ( e )
      {
 
        case 0:
        case 1:
        case 2:
        case 3:
          vList.push_back( gkg::Vector3d< T >(
                                            ( z - z0 ) / ( z1 - z0 ), 
                                            _vertex[ _edge[ e ][ 0 ] ][ 1 ], 
                                            _vertex[ _edge[ e ][ 0 ] ][ 2 ] ) );
          break;
        case 4:
        case 5:
        case 6:
        case 7:
          vList.push_back( gkg::Vector3d< T >( 
                                            _vertex[ _edge[ e ][ 0 ] ][ 0 ], 
                                            ( z - z0 ) / ( z1 - z0 ),
                                            _vertex[ _edge[ e ][ 0 ] ][ 2 ] ) );
          break;
        case 8:
        case 9:
        case 10:
        case 11:
          vList.push_back( gkg::Vector3d< T >( _vertex[ _edge[ e ][ 0 ] ][ 0 ], 
                                               _vertex[ _edge[ e ][ 0 ] ][ 1 ], 
                                               ( z - z0 ) / ( z1 - z0 ) ) );
          break;
        default:
          break;

      }

    }

    _slices.insert( std::make_pair( z, vList ) );

  }

}


template class gkg::Slicing< float >;
template class gkg::Slicing< double >;