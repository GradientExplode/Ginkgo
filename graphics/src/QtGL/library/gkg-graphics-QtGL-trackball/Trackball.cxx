#include <gkg-graphics-QtGL-trackball/Trackball.h>

#include <cmath>


template < class T >
inline
gkg::Trackball< T >::Trackball() 
                   : _width( T( 1 ) ),
                     _height( T( 1 ) ),
                     _rho( T( 1 ) )
{
}


template < class T >
inline
gkg::Trackball< T >::~Trackball()
{
}


template < class T >
inline
void gkg::Trackball< T >::setQuaternion( const gkg::Quaternion< T >& q )
{

  _q = q;

}


template < class T >
inline
void gkg::Trackball< T >::setViewport( int32_t width, int32_t height, T radius )
{

  _width = T( width );
  _height = T( height );
  _rho = radius;

}

template < class T >
inline
const typename gkg::Quaternion< T >::Rotation& 
gkg::Trackball< T >::getRotation() const
{

  return _q.getRotation();

}


template < class T >
inline
const typename gkg::Quaternion< T >::Rotation& 
gkg::Trackball< T >::getInverseRotation() const
{

  return _q.getInverseRotation();

}


template< class T >
inline
gkg::Quaternion< T >& gkg::Trackball< T >::getQuaternion()
{

  return _q;

}


template < class T >
inline
void gkg::Trackball< T >::mousePress( int32_t x, int32_t y )
{

  mouseToSphere( T( x ), T( y ), _v0 );

}


template < class T >
inline
void gkg::Trackball< T >::mouseMove( int32_t x, int32_t y )
{

  T v1[ 3 ];
  gkg::Quaternion< T > q0;

  mouseToSphere( T( x ), T( y ), v1 );

  if ( ( _v0[ 0 ] != v1[ 0 ] ) || ( _v0[ 1 ] != v1[ 1 ] ) )
  {

    T c0 = v1[ 1 ] * _v0[ 2 ] - v1[ 2 ] * _v0[ 1 ];
    T c1 = v1[ 2 ] * _v0[ 0 ] - v1[ 0 ] * _v0[ 2 ];
    T c2 = v1[ 0 ] * _v0[ 1 ] - v1[ 1 ] * _v0[ 0 ];
    T n = std::sqrt( c0 * c0 + c1 * c1 + c2 * c2 );
    T d0 = _v0[ 0 ] - v1[ 0 ];
    T d1 = _v0[ 1 ] - v1[ 1 ];
    T d2 = _v0[ 2 ] - v1[ 2 ];
    T t = std::sqrt( d0 * d0 + d1 * d1 + d2 * d2 ) / ( 2.0 * _rho );

    if ( t > T( 1 ) )
    {

      t = T( 1 );

    }

    if ( t < T( -1 ) )
    {

      t = T( -1 );

    }

    T s = t / n;

    q0.set( c0 * s, c1 * s, c2 * s, std::cos( std::asin( t ) ) );

  }

  _q *= q0;
  _v0[ 0 ] = v1[ 0 ];
  _v0[ 1 ] = v1[ 1 ];
  _v0[ 2 ] = v1[ 2 ];

}


template < class T >
inline
void gkg::Trackball< T >::mouseToSphere( T x, T y, T* p )
{

  T px = ( 2.0 * x - _width ) / _width;
  T py = ( _height - 2.0 * y ) / _height;
  T d2 = px * px + py * py;
  T r2 = _rho * _rho;

  p[ 0 ] = px;
  p[ 1 ] = py;

  if ( d2 < ( 0.5 * r2 ) )
  {

    p[ 2 ] = std::sqrt( r2 - d2 );

  }
  else
  {

    p[ 2 ] = 0.5 * r2 / std::sqrt( d2 );

  }

}


template class gkg::Trackball< float >;
template class gkg::Trackball< double >;
