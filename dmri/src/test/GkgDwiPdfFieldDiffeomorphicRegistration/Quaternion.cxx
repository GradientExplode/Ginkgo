#include "Quaternion.h"


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
gkg::Quaternion< T >::Quaternion( const gkg::Matrix& rotation )
                     : gkg::HomogeneousTransform3d< T >()
{

  // quaternion construction from a 3x3 rotation matrix :
  // algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
  // article "Quaternion Calculus and Fast Animation".
  double trace = rotation.getTrace();
  double root;

  if (trace > 0.0) 
  {
    // |w| > 1/2, may as well choose w > 1/2
    root = std::sqrt( ( double )( trace + 1.0 ) );  // 2w
    _q[ 3 ] = ( T )( 0.5 * root );
    root = 0.5 / root; // 1/(4w)
    _q[ 0 ] = ( T )( rotation( 1, 2 ) - rotation( 2, 1 ) ) * root;
    _q[ 1 ] = ( T )( rotation( 2, 0 ) - rotation( 0, 2 ) ) * root;
    _q[ 2 ] = ( T )( rotation( 0, 1 ) - rotation( 1, 0 ) ) * root;

  } 
  else 
  {
    // |w| <= 1/2
    int32_t i = 0;
    if ( rotation( 1 , 1 ) > rotation( 0, 0 ) ) 
    {
    
      i = 1;
    
    }
    if ( rotation( 2, 2 ) > rotation( i, i ) ) 
    {
    
      i = 2;
    
    }
    int32_t j = ( i + 1 ) % 3;
    int32_t k = ( i + 2 ) % 3;

    root = std::sqrt( rotation( i, i ) - rotation( j, j ) - 
                                                      rotation( k, k ) + 1.0 );
    _q[ i ] = ( T )( 0.5 * root );
    root = 0.5 / root;
    _q[ 3 ] = ( T )( ( rotation( j, k ) - rotation( k, j ) ) * root );
    _q[ j ] = ( T )( ( rotation( j, i ) + rotation( i, j ) ) * root );
    _q[ k ] = ( T )( ( rotation( k, i ) + rotation( i, k ) ) * root );

  }
  computeTransformation();

}


template < class T >
gkg::Quaternion< T >::~Quaternion()
{
}


template < class T >
T gkg::Quaternion< T >::operator [] ( int32_t i )
{

  return _q[ i ];

}


template < class T >
gkg::Quaternion< T >& 
gkg::Quaternion< T >::operator += ( gkg::Quaternion< T >& q )
{

  _q[ 0 ] += q[ 0 ];
  _q[ 1 ] += q[ 1 ];
  _q[ 2 ] += q[ 2 ];
  _q[ 3 ] += q[ 3 ];

  normalize();
  computeTransformation();

  return *this;

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

  normalize();
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
bool gkg::Quaternion< T >::isClose( gkg::Quaternion< T >& other )
{

  bool isClose = true;

  if ( ( _q[ 0 ] * other._q[ 0 ] + 
         _q[ 1 ] * other._q[ 1 ] + 
         _q[ 2 ] * other._q[ 2 ] + 
         _q[ 3 ] * other._q[ 3 ] ) < ( T )( 0.0 ) )
  {

    isClose = false;

  }
  return isClose;

}


template < class T >
void gkg::Quaternion< T >::inverseSign()
{

  _q[ 0 ] *= ( T )( -1.0 );
  _q[ 1 ] *= ( T )( -1.0 );
  _q[ 2 ] *= ( T )( -1.0 );
  _q[ 3 ] *= ( T )( -1.0 );

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
void gkg::Quaternion< T >::
                         getRotationMatrix( gkg::Matrix& rotationMatrix ) const
{

  T t, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

  t = T( 2 ) / ( _q[ 0 ] * _q[ 0 ] + _q[ 1 ] * _q[ 1 ] +
                 _q[ 2 ] * _q[ 2 ] + _q[ 3 ] * _q[ 3 ] );

  xs = _q[ 0 ] * t;
  ys = _q[ 1 ] * t;
  zs = _q[ 2 ] * t;
  wx = _q[ 3 ] * xs;
  wy = _q[ 3 ] * ys;
  wz = _q[ 3 ] * zs;
  xx = _q[ 0 ] * xs;
  xy = _q[ 0 ] * ys;
  xz = _q[ 0 ] * zs;
  yy = _q[ 1 ] * ys;
  yz = _q[ 1 ] * zs;
  zz = _q[ 2 ] * zs;

  rotationMatrix( 0, 0 ) = T( 1 ) - ( yy + zz );
  rotationMatrix( 0, 1 ) = xy + wz;
  rotationMatrix( 0, 2 ) = xz - wy;
  rotationMatrix( 1, 0 ) = xy - wz;
  rotationMatrix( 1, 1 ) = T( 1 )- ( xx + zz );
  rotationMatrix( 1, 2 ) = yz + wx;
  rotationMatrix( 2, 0 ) = xz + wy;
  rotationMatrix( 2, 1 ) = yz - wx;
  rotationMatrix( 2, 2 ) = T( 1 ) - ( xx + yy );

}

template < class T >
gkg::Vector gkg::Quaternion< T >::getComponents() const
{

  gkg::Vector componentsVector( 4 );
  componentsVector( 0 ) = _q[ 0 ]; 
  componentsVector( 1 ) = _q[ 1 ]; 
  componentsVector( 2 ) = _q[ 2 ]; 
  componentsVector( 3 ) = _q[ 3 ]; 

  return componentsVector;

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


namespace gkg
{


template < class U >
gkg::Quaternion< U > operator * ( gkg::Quaternion< U >& q1, 
                                  gkg::Quaternion< U >& q2 )
{

  gkg::Quaternion< U > q( q1 );

  q *= q2;

  return q;

}


template gkg::Quaternion< float > 
operator *< float > ( gkg::Quaternion< float >&, gkg::Quaternion< float >& );
template gkg::Quaternion< double > 
operator *< double > ( gkg::Quaternion< double >&, gkg::Quaternion< double >& );


}


template class gkg::Quaternion< float >;
template class gkg::Quaternion< double >;
