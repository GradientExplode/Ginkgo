#include <gkg-processing-transform/Quaternion.h>

#include <cstring>


template < class T >
gkg::Quaternion< T >::Quaternion() 
                     : gkg::HomogeneousTransform3d< T >()
{

  _q[ 0 ] = T( 0 );
  _q[ 1 ] = T( 0 );
  _q[ 2 ] = T( 0 );
  _q[ 3 ] = T( 1 );
  computeTransformation();

}


template < class T >
gkg::Quaternion< T >::Quaternion( const gkg::Quaternion< T >& q )
                     : gkg::HomogeneousTransform3d< T >()
{

  _q[ 0 ] = q._q[ 0 ];
  _q[ 1 ] = q._q[ 1 ];
  _q[ 2 ] = q._q[ 2 ];
  _q[ 3 ] = q._q[ 3 ];
  computeTransformation();

}


template < class T >
gkg::Quaternion< T >::Quaternion( T x, T y, T z, T w )
                     : gkg::HomogeneousTransform3d< T >()
{

  _q[ 0 ] = x;
  _q[ 1 ] = y;
  _q[ 2 ] = z;
  _q[ 3 ] = w;
  computeTransformation();

}


template < class T >
gkg::Quaternion< T >::~Quaternion()
{
}


template < class T >
gkg::Quaternion< T >& gkg::Quaternion< T >::operator = (
                                             const gkg::Quaternion< T >& other )
{

  _q[ 0 ] = other._q[ 0 ];
  _q[ 1 ] = other._q[ 1 ];
  _q[ 2 ] = other._q[ 2 ];
  _q[ 3 ] = other._q[ 3 ];
  std::memcpy( _r, other._r, 16 * sizeof( T ) );
  std::memcpy( _ri, other._ri, 16 * sizeof( T ) );
  this->copy( other );
  
  return *this;

}


template < class T >
T gkg::Quaternion< T >::operator [] ( int32_t i )
{

  return _q[ i ];

}


template < class T >
gkg::Quaternion< T >& 
gkg::Quaternion< T >::operator *= ( gkg::Quaternion< T >& q )
{

  T t[ 4 ];

  t[ 0 ] = _q[ 3 ] * q[ 0 ] + _q[ 0 ] * q[ 3 ] + _q[ 1 ] * q[ 2 ] -
           _q[ 2 ] * q[ 1 ];
  t[ 1 ] = _q[ 3 ] * q[ 1 ] + _q[ 1 ] * q[ 3 ] + _q[ 2 ] * q[ 0 ] -
           _q[ 0 ] * q[ 2 ];
  t[ 2 ] = _q[ 3 ] * q[ 2 ] + _q[ 2 ] * q[ 3 ] + _q[ 0 ] * q[ 1 ] -
           _q[ 1 ] * q[ 0 ];
  t[ 3 ] = _q[ 3 ] * q[ 3 ] - _q[ 0 ] * q[ 0 ] - _q[ 1 ] * q[ 1 ] -
           _q[ 2 ] * q[ 2 ];
  _q[ 0 ] = t[ 0 ];
  _q[ 1 ] = t[ 1 ];
  _q[ 2 ] = t[ 2 ];
  _q[ 3 ] = t[ 3 ];

  computeTransformation();

  return *this;

}


template < class T >
void gkg::Quaternion< T >::set( T x, T y, T z, T w )
{

  _q[ 0 ] = x;
  _q[ 1 ] = y;
  _q[ 2 ] = z;
  _q[ 3 ] = w;
  computeTransformation();

}


template < class T >
void gkg::Quaternion< T >::normalize()
{

  T n = _q[ 0 ] * _q[ 0 ] + _q[ 1 ] * _q[ 1 ] +
        _q[ 2 ] * _q[ 2 ] + _q[ 3 ] * _q[ 3 ];

  _q[ 0 ] /= n;
  _q[ 1 ] /= n;
  _q[ 2 ] /= n;
  _q[ 3 ] /= n;

}


template < class T >
void gkg::Quaternion< T >::getMatrix( gkg::Quaternion< T >::Rotation& r, T s )
{

  T t, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

  t = T( 2 ) / ( _q[ 0 ] * _q[ 0 ] + _q[ 1 ] * _q[ 1 ] +
                 _q[ 2 ] * _q[ 2 ] + _q[ 3 ] * _q[ 3 ] );

  xs = _q[ 0 ] * t;
  ys = _q[ 1 ] * t;
  zs = _q[ 2 ] * t;
  wx = s * _q[ 3 ] * xs;
  wy = s * _q[ 3 ] * ys;
  wz = s * _q[ 3 ] * zs;
  xx = _q[ 0 ] * xs;
  xy = _q[ 0 ] * ys;
  xz = _q[ 0 ] * zs;
  yy = _q[ 1 ] * ys;
  yz = _q[ 1 ] * zs;
  zz = _q[ 2 ] * zs;

  // The 4x4 rotation matrix is arranged as follows
  //
  //     ( r0  r4  r8  r12 )
  // R = ( r1  r5  r9  r13 )
  //     ( r2  r6  r10 r14 )
  //     ( r3  r7  r11 r15 )
  //
  // to fit OpenGL's convention.
  //
  r[ 0 ] = T( 1 ) - ( yy + zz );
  r[ 1 ] = xy - wz;
  r[ 2 ] = xz + wy;
  r[ 3 ] = T( 0 );
  r[ 4 ] = xy + wz;
  r[ 5 ] = T( 1 )- ( xx + zz );
  r[ 6 ] = yz - wx;
  r[ 7 ] = T( 0 );
  r[ 8 ] = xz - wy;
  r[ 9 ] = yz + wx;
  r[ 10 ] = T( 1 ) - ( xx + yy );
  r[ 11 ] = T( 0 );
  r[ 12 ] = T( 0 );
  r[ 13 ] = T( 0 );
  r[ 14 ] = T( 0 );
  r[ 15 ] = T( 1 );

}


template < class T >
const typename gkg::Quaternion< T >::Rotation& 
gkg::Quaternion< T >::getRotation() const
{

  return _r;

}


template < class T >
const typename gkg::Quaternion< T >::Rotation&
gkg::Quaternion< T >::getInverseRotation() const
{

  return _ri;

}


template < class T >
void gkg::Quaternion< T >::computeTransformation()
{

  getMatrix( _r, T( 1 ) );
  this->setDirectItems( _r[ 0 ], _r[ 4 ], _r[ 8 ], _r[ 12 ],
                        _r[ 1 ], _r[ 5 ], _r[ 9 ], _r[ 13 ],
                        _r[ 2 ], _r[ 6 ], _r[ 10 ], _r[ 14 ],
                        _r[ 3 ], _r[ 7 ], _r[ 11 ], _r[ 15 ] );
  getMatrix( _ri, T( -1 ) );
  this->setInverseItems( _ri[ 0 ], _ri[ 4 ], _ri[ 8 ], _ri[ 12 ],
                         _ri[ 1 ], _ri[ 5 ], _ri[ 9 ], _ri[ 13 ],
                         _ri[ 2 ], _ri[ 6 ], _ri[ 10 ], _ri[ 14 ],
                         _ri[ 3 ], _ri[ 7 ], _ri[ 11 ], _ri[ 15 ] );

}


// Specialization


template < class T >
gkg::Quaternion< T > operator * ( gkg::Quaternion< T >& q1, 
                                  gkg::Quaternion< T >& q2 )
{

  gkg::Quaternion< T > q( q1 );

  q *= q2;

  return q;

}



template class gkg::Quaternion< float >;
template class gkg::Quaternion< double >;


template gkg::Quaternion< float > 
operator * ( gkg::Quaternion< float >&, gkg::Quaternion< float >& );
template gkg::Quaternion< double > 
operator * ( gkg::Quaternion< double >&,  gkg::Quaternion< double >& );
