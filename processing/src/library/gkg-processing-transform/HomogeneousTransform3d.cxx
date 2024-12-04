#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-exception/NonReversibleMatrixException.h>
#include <gkg-core-exception/Exception.h>


namespace gkg
{

template < class T >
static T determinant( T d[ 4 ][ 4 ] );

template < class T >
static void invert( T d[ 4 ][ 4 ], T i[ 4 ][ 4 ] );

}


template < class T >
T gkg::determinant( T d[ 4 ][ 4 ] )
{

  try
  {

    T determinant = d[ 0 ][ 0 ] *
                    ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ] ) -
                    d[ 1 ][ 0 ] *
                    ( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ] ) +
                    d[ 2 ][ 0 ] *
                    ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ] );

    return determinant;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::determinant( T d[ 4 ][ 4 ] )" );


}

template < class T >
void gkg::invert( T d[ 4 ][ 4 ], T i[ 4 ][ 4 ] )
{

  try
  {

    T determinant = gkg::determinant( d );

    if ( determinant == 0.0 )
    {

      throw gkg::NonReversibleMatrixException( "gkg::invert< T >: " );

    }
    else
    {

    i[ 0 ][ 0 ] = +( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ]  ) /
                  determinant;
    i[ 1 ][ 0 ] = -( d[ 1 ][ 0 ] * d[ 2 ][ 2 ] - d[ 2 ][ 0 ] * d[ 1 ][ 2 ]  ) /
                  determinant;
    i[ 2 ][ 0 ] = +( d[ 1 ][ 0 ] * d[ 2 ][ 1 ] - d[ 2 ][ 0 ] * d[ 1 ][ 1 ]  ) /
                  determinant;
    i[ 3 ][ 0 ] = 0.0;

    i[ 0 ][ 1 ] = -( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ]  ) /
                  determinant;
    i[ 1 ][ 1 ] = +( d[ 0 ][ 0 ] * d[ 2 ][ 2 ] - d[ 2 ][ 0 ] * d[ 0 ][ 2 ]  ) /
                  determinant;
    i[ 2 ][ 1 ] = -( d[ 0 ][ 0 ] * d[ 2 ][ 1 ] - d[ 2 ][ 0 ] * d[ 0 ][ 1 ]  ) /
                  determinant;
    i[ 3 ][ 1 ] = 0.0;

    i[ 0 ][ 2 ] = +( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ]  ) /
                  determinant;
    i[ 1 ][ 2 ] = -( d[ 0 ][ 0 ] * d[ 1 ][ 2 ] - d[ 1 ][ 0 ] * d[ 0 ][ 2 ]  ) /
                  determinant;
    i[ 2 ][ 2 ] = +( d[ 0 ][ 0 ] * d[ 1 ][ 1 ] - d[ 1 ][ 0 ] * d[ 0 ][ 1 ]  ) /
                  determinant;
    i[ 3 ][ 2 ] = 0.0;

    i[ 0 ][ 3 ] = -( +d[ 0 ][ 1 ] *
                     ( d[ 1 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 1 ][ 3 ] )
                     -d[ 1 ][ 1 ] *
                     ( d[ 0 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 0 ][ 3 ] )
                     +d[ 2 ][ 1 ] *
                     ( d[ 0 ][ 2 ] * d[ 1 ][ 3 ] - d[ 1 ][ 2 ] * d[ 0 ][ 3 ] )
                   ) /
                  determinant;
    i[ 1 ][ 3 ] = +( +d[ 0 ][ 0 ] *
                     ( d[ 1 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 1 ][ 3 ] )
                     -d[ 1 ][ 0 ] *
                     ( d[ 0 ][ 2 ] * d[ 2 ][ 3 ] - d[ 2 ][ 2 ] * d[ 0 ][ 3 ] )
                     +d[ 2 ][ 0 ] *
                     ( d[ 0 ][ 2 ] * d[ 1 ][ 3 ] - d[ 1 ][ 2 ] * d[ 0 ][ 3 ] )
                   ) /
                  determinant;
    i[ 2 ][ 3 ] = -( +d[ 0 ][ 0 ] *
                     ( d[ 1 ][ 1 ] * d[ 2 ][ 3 ] - d[ 2 ][ 1 ] * d[ 1 ][ 3 ] )
                     -d[ 1 ][ 0 ] *
                     ( d[ 0 ][ 1 ] * d[ 2 ][ 3 ] - d[ 2 ][ 1 ] * d[ 0 ][ 3 ] )
                     +d[ 2 ][ 0 ] *
                     ( d[ 0 ][ 1 ] * d[ 1 ][ 3 ] - d[ 1 ][ 1 ] * d[ 0 ][ 3 ] )
                   ) /
                  determinant;
    i[ 3 ][ 3 ] = +( +d[ 0 ][ 0 ] *
                     ( d[ 1 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 1 ][ 2 ] )
                     -d[ 1 ][ 0 ] *
                     ( d[ 0 ][ 1 ] * d[ 2 ][ 2 ] - d[ 2 ][ 1 ] * d[ 0 ][ 2 ] )
                     +d[ 2 ][ 0 ] *
                     ( d[ 0 ][ 1 ] * d[ 1 ][ 2 ] - d[ 1 ][ 1 ] * d[ 0 ][ 2 ] )
                   ) /
                  determinant;

      int32_t ii, jj;
      for ( jj = 0; jj < 4; jj++ )
      {

        for ( ii = 0; ii < 4; ii++ )
        {

          i[ ii ][ jj ] /= i[ 3 ][ 3 ];

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::invert( T d[ 4 ][ 4 ], T i[ 4 ][ 4 ] )" );

}


template < class T >
gkg::HomogeneousTransform3d< T >::HomogeneousTransform3d()
                                 : gkg::Transform3d< T >()
{

  setIdentity( true, false );

}


template < class T >
gkg::HomogeneousTransform3d< T >::HomogeneousTransform3d(
                                 const gkg::HomogeneousTransform3d< T >& other )
                                 : gkg::Transform3d< T >()
{

  int32_t i, j;
  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

        _directItems[ i ][ j ] = other._directItems[ i ][ j ];
        _inverseItems[ i ][ j ] = other._inverseItems[ i ][ j ];

    }

  }

}


template < class T >
gkg::HomogeneousTransform3d< T >::HomogeneousTransform3d(
                                                    T d00, T d01, T d02, T d03,
                                                    T d10, T d11, T d12, T d13,
                                                    T d20, T d21, T d22, T d23,
                                                    T d30, T d31, T d32, T d33,
                                                    bool direct )
                                 : gkg::Transform3d< T >()
{

  if ( direct )
  {

    setDirectItems( d00, d01, d02, d03,
                    d10, d11, d12, d13,
                    d20, d21, d22, d23,
                    d30, d31, d32, d33,
                    true, false );
                    
  }
  else
  {

    setInverseItems( d00, d01, d02, d03,
                     d10, d11, d12, d13,
                     d20, d21, d22, d23,
                     d30, d31, d32, d33,
                     true, false );
                    
  }  

}


template < class T >
gkg::HomogeneousTransform3d< T >::HomogeneousTransform3d(
                                    const std::vector< T >& directCoefficients,
                                    bool direct )
                                 : gkg::Transform3d< T >()
{

  try
  {

    if ( directCoefficients.size() != 16U )
    {

      throw std::runtime_error(
                     "direct coefficient vector must have a size equal to 16" );

    }
    if ( direct )
    {
    
      setDirectItems( directCoefficients[  0 ],
                      directCoefficients[  1 ],
                      directCoefficients[  2 ],
                      directCoefficients[  3 ],
                      directCoefficients[  4 ],
                      directCoefficients[  5 ],
                      directCoefficients[  6 ],
                      directCoefficients[  7 ],
                      directCoefficients[  8 ],
                      directCoefficients[  9 ],
                      directCoefficients[ 10 ],
                      directCoefficients[ 11 ],
                      directCoefficients[ 12 ],
                      directCoefficients[ 13 ],
                      directCoefficients[ 14 ],
                      directCoefficients[ 15 ],
                      true, false );
                      
    }
    else
    {
    
      setInverseItems( directCoefficients[  0 ],
                       directCoefficients[  1 ],
                       directCoefficients[  2 ],
                       directCoefficients[  3 ],
                       directCoefficients[  4 ],
                       directCoefficients[  5 ],
                       directCoefficients[  6 ],
                       directCoefficients[  7 ],
                       directCoefficients[  8 ],
                       directCoefficients[  9 ],
                       directCoefficients[ 10 ],
                       directCoefficients[ 11 ],
                       directCoefficients[ 12 ],
                       directCoefficients[ 13 ],
                       directCoefficients[ 14 ],
                       directCoefficients[ 15 ],
                       true, false );
    
    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::HomogeneousTransform3d< T >::HomogeneousTransform3d( "
             "const std::vector< T >& directCoefficients )" );

}


template < class T >
gkg::HomogeneousTransform3d< T >::~HomogeneousTransform3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::HomogeneousTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::HomogeneousTransform3d< T >( *this ) );
  return transform;

}


template < class T >
bool gkg::HomogeneousTransform3d< T >::operator == (
                                 const gkg::HomogeneousTransform3d< T >& other )
{

  try
  {

    static float epsilon = 1e-5;
    return ( std::fabs( _directItems[ 0 ][ 0 ] -
                        other._directItems[ 0 ][ 0 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 0 ][ 1 ] -
                        other._directItems[ 0 ][ 1 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 0 ][ 2 ] -
                        other._directItems[ 0 ][ 2 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 0 ][ 3 ] -
                        other._directItems[ 0 ][ 3 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 1 ][ 0 ] -
                        other._directItems[ 1 ][ 0 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 1 ][ 1 ] -
                        other._directItems[ 1 ][ 1 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 1 ][ 2 ] -
                        other._directItems[ 1 ][ 2 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 1 ][ 3 ] -
                        other._directItems[ 1 ][ 3 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 2 ][ 0 ] -
                        other._directItems[ 2 ][ 0 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 2 ][ 1 ] -
                        other._directItems[ 2 ][ 1 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 2 ][ 2 ] -
                        other._directItems[ 2 ][ 2 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 2 ][ 3 ] -
                        other._directItems[ 2 ][ 3 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 3 ][ 0 ] -
                        other._directItems[ 3 ][ 0 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 3 ][ 1 ] -
                        other._directItems[ 3 ][ 1 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 3 ][ 2 ] -
                        other._directItems[ 3 ][ 2 ] ) < epsilon ) &&
           ( std::fabs( _directItems[ 3 ][ 3 ] -
                        other._directItems[ 3 ][ 3 ] ) < epsilon );

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::HomogeneousTransform3d< T >::operator == ( "
             "const gkg::HomogeneousTransform3d< T >& other )" );

}


template < class T >
const T& gkg::HomogeneousTransform3d< T >::getDirectCoefficient(
                                                    int32_t i, int32_t j ) const
{

  try
  {

    if ( ( i < 0 ) || ( i > 3 ) || ( j < 0 ) || ( j > 3 ) )
    {

      throw std::runtime_error( "index out of range" );

    }

    return _directItems[ i ][ j ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::HomogeneousTransform3d< T >::getDirectCoefficient( "
             "int i, int j ) const" );

}


template < class T >
const T& gkg::HomogeneousTransform3d< T >::getInverseCoefficient(
                                                    int32_t i, int32_t j ) const
{

  try
  {

    if ( ( i < 0 ) || ( i > 3 ) || ( j < 0 ) || ( j > 3 ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    return _inverseItems[ i ][ j ];

  }
  GKG_CATCH( "template < class T > "
            "const T& gkg::HomogeneousTransform3d< T >::getInverseCoefficient( "
            "int i, int j ) const" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setDirectCoefficient( 
                                                           int32_t i, int32_t j,
                                                           T value,
                                                           bool update,
                                                           bool notify )
{

  try
  {

    if ( ( i < 0 ) || ( i > 3 ) || ( j < 0 ) || ( j > 3 ) )
    {

      throw std::runtime_error( "index out of range" );

    }

    _directItems[ i ][ j ] = value;

    if ( update )
    {

      gkg::invert( _directItems, _inverseItems );

      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >::setDirectCoefficient( "
             "int32_t i, int32_t j, T value, bool update, bool notify" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setInverseCoefficient( 
                                                           int32_t i, int32_t j,
                                                           T value,
                                                           bool update,
                                                           bool notify )
{
  try
  {

    if ( ( i < 0 ) || ( i > 3 ) || ( j < 0 ) || ( j > 3 ) )
    {

      throw std::runtime_error( "index out of range" );

    }

    _inverseItems[ i ][ j ] = value;

    if ( update )
    {

      gkg::invert( _inverseItems, _directItems );
      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >::setInverseCoefficient( "
             "int32_t i, int32_t j, T value, bool update, bool notify )" );
}


template < class T >
std::vector< T > gkg::HomogeneousTransform3d< T >::getDirectCoefficients() const
{

  try
  {

    std::vector< T > directCoefficients( 16 );

    directCoefficients[  0 ] = _directItems[ 0 ][ 0 ];
    directCoefficients[  1 ] = _directItems[ 0 ][ 1 ];
    directCoefficients[  2 ] = _directItems[ 0 ][ 2 ];
    directCoefficients[  3 ] = _directItems[ 0 ][ 3 ];
    directCoefficients[  4 ] = _directItems[ 1 ][ 0 ];
    directCoefficients[  5 ] = _directItems[ 1 ][ 1 ];
    directCoefficients[  6 ] = _directItems[ 1 ][ 2 ];
    directCoefficients[  7 ] = _directItems[ 1 ][ 3 ];
    directCoefficients[  8 ] = _directItems[ 2 ][ 0 ];
    directCoefficients[  9 ] = _directItems[ 2 ][ 1 ];
    directCoefficients[ 10 ] = _directItems[ 2 ][ 2 ];
    directCoefficients[ 11 ] = _directItems[ 2 ][ 3 ];
    directCoefficients[ 12 ] = _directItems[ 3 ][ 0 ];
    directCoefficients[ 13 ] = _directItems[ 3 ][ 1 ];
    directCoefficients[ 14 ] = _directItems[ 3 ][ 2 ];
    directCoefficients[ 15 ] = _directItems[ 3 ][ 3 ];

    return directCoefficients;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< T > "
             "gkg::HomogeneousTransform3d< T >::getDirectCoefficients() "
             "const" );

}


template < class T >
std::vector< T > 
gkg::HomogeneousTransform3d< T >::getInverseCoefficients() const
{

  try
  {

    std::vector< T > inverseCoefficients( 16 );

    inverseCoefficients[  0 ] = _inverseItems[ 0 ][ 0 ];
    inverseCoefficients[  1 ] = _inverseItems[ 0 ][ 1 ];
    inverseCoefficients[  2 ] = _inverseItems[ 0 ][ 2 ];
    inverseCoefficients[  3 ] = _inverseItems[ 0 ][ 3 ];
    inverseCoefficients[  4 ] = _inverseItems[ 1 ][ 0 ];
    inverseCoefficients[  5 ] = _inverseItems[ 1 ][ 1 ];
    inverseCoefficients[  6 ] = _inverseItems[ 1 ][ 2 ];
    inverseCoefficients[  7 ] = _inverseItems[ 1 ][ 3 ];
    inverseCoefficients[  8 ] = _inverseItems[ 2 ][ 0 ];
    inverseCoefficients[  9 ] = _inverseItems[ 2 ][ 1 ];
    inverseCoefficients[ 10 ] = _inverseItems[ 2 ][ 2 ];
    inverseCoefficients[ 11 ] = _inverseItems[ 2 ][ 3 ];
    inverseCoefficients[ 12 ] = _inverseItems[ 3 ][ 0 ];
    inverseCoefficients[ 13 ] = _inverseItems[ 3 ][ 1 ];
    inverseCoefficients[ 14 ] = _inverseItems[ 3 ][ 2 ];
    inverseCoefficients[ 15 ] = _inverseItems[ 3 ][ 3 ];

    return inverseCoefficients;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< T > "
             "gkg::HomogeneousTransform3d< T >::getInverseCoefficients() "
             "const" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::getDirect(
                                                 const gkg::Vector3d< T >& from, 
                                                 gkg::Vector3d< T >& to ) const
{

  if ( &from == &to )
  {

    gkg::Vector3d< T > tmp( from );

    T h = _directItems[ 3 ][ 0 ] * tmp.x +
          _directItems[ 3 ][ 1 ] * tmp.y +
          _directItems[ 3 ][ 2 ] * tmp.z +
          _directItems[ 3 ][ 3 ];

    to.x = ( _directItems[ 0 ][ 0 ] * tmp.x +
             _directItems[ 0 ][ 1 ] * tmp.y +
             _directItems[ 0 ][ 2 ] * tmp.z +
             _directItems[ 0 ][ 3 ] ) / h;
    to.y = ( _directItems[ 1 ][ 0 ] * tmp.x +
             _directItems[ 1 ][ 1 ] * tmp.y +
             _directItems[ 1 ][ 2 ] * tmp.z +
             _directItems[ 1 ][ 3 ] ) / h;
    to.z = ( _directItems[ 2 ][ 0 ] * tmp.x +
             _directItems[ 2 ][ 1 ] * tmp.y +
             _directItems[ 2 ][ 2 ] * tmp.z +
             _directItems[ 2 ][ 3 ] ) / h;

  }
  else
  {

    T h = _directItems[ 3 ][ 0 ] * from.x +
          _directItems[ 3 ][ 1 ] * from.y +
          _directItems[ 3 ][ 2 ] * from.z +
          _directItems[ 3 ][ 3 ];

    to.x = ( _directItems[ 0 ][ 0 ] * from.x +
             _directItems[ 0 ][ 1 ] * from.y +
             _directItems[ 0 ][ 2 ] * from.z +
             _directItems[ 0 ][ 3 ] ) / h;
    to.y = ( _directItems[ 1 ][ 0 ] * from.x +
             _directItems[ 1 ][ 1 ] * from.y +
             _directItems[ 1 ][ 2 ] * from.z +
             _directItems[ 1 ][ 3 ] ) / h;
    to.z = ( _directItems[ 2 ][ 0 ] * from.x +
             _directItems[ 2 ][ 1 ] * from.y +
             _directItems[ 2 ][ 2 ] * from.z +
             _directItems[ 2 ][ 3 ] ) / h;

  }

}


template < class T >
void gkg::HomogeneousTransform3d< T >::getInverse(
                                                const gkg::Vector3d< T >& to, 
                                                gkg::Vector3d< T >& from ) const
{

  if ( &to == &from )
  {

    gkg::Vector3d< T > tmp( to );

    T h = _inverseItems[ 3 ][ 0 ] * tmp.x +
          _inverseItems[ 3 ][ 1 ] * tmp.y +
          _inverseItems[ 3 ][ 2 ] * tmp.z +
          _inverseItems[ 3 ][ 3 ];

    from.x = ( _inverseItems[ 0 ][ 0 ] * tmp.x +
               _inverseItems[ 0 ][ 1 ] * tmp.y +
               _inverseItems[ 0 ][ 2 ] * tmp.z +
               _inverseItems[ 0 ][ 3 ] ) / h;
    from.y = ( _inverseItems[ 1 ][ 0 ] * tmp.x +
               _inverseItems[ 1 ][ 1 ] * tmp.y +
               _inverseItems[ 1 ][ 2 ] * tmp.z +
               _inverseItems[ 1 ][ 3 ] ) / h;
    from.z = ( _inverseItems[ 2 ][ 0 ] * tmp.x +
               _inverseItems[ 2 ][ 1 ] * tmp.y +
               _inverseItems[ 2 ][ 2 ] * tmp.z +
               _inverseItems[ 2 ][ 3 ] ) / h;

  }
  else
  {

    T h = _inverseItems[ 3 ][ 0 ] * to.x +
          _inverseItems[ 3 ][ 1 ] * to.y +
          _inverseItems[ 3 ][ 2 ] * to.z +
          _inverseItems[ 3 ][ 3 ];

    from.x = ( _inverseItems[ 0 ][ 0 ] * to.x +
               _inverseItems[ 0 ][ 1 ] * to.y +
               _inverseItems[ 0 ][ 2 ] * to.z +
               _inverseItems[ 0 ][ 3 ] ) / h;
    from.y = ( _inverseItems[ 1 ][ 0 ] * to.x +
               _inverseItems[ 1 ][ 1 ] * to.y +
               _inverseItems[ 1 ][ 2 ] * to.z +
               _inverseItems[ 1 ][ 3 ] ) / h;
    from.z = ( _inverseItems[ 2 ][ 0 ] * to.x +
               _inverseItems[ 2 ][ 1 ] * to.y +
               _inverseItems[ 2 ][ 2 ] * to.z +
               _inverseItems[ 2 ][ 3 ] ) / h;

  }

}


template < class T >
bool gkg::HomogeneousTransform3d< T >::isComposable() const
{

  return true;

}


template < class T >
gkg::HomogeneousTransform3d< T >& gkg::HomogeneousTransform3d< T >::operator=(
                                 const gkg::HomogeneousTransform3d< T >& other )
{

  this->copy( other );
  return *this;

}


// careful : T1.compose( T2 ) = T2 x T1
template < class T >
gkg::HomogeneousTransform3d< T > gkg::HomogeneousTransform3d< T >::compose(
                                const HomogeneousTransform3d< T >& other ) const
{

  try
  {

    gkg::HomogeneousTransform3d< T > composed;

    int32_t i, j, k;
    for ( j = 0; j < 4; j++ )
    {

      for ( i = 0; i < 4; i++ )
      {

        composed._directItems[ i ][ j ] = 0.0;
        for ( k = 0; k < 4; k++ )
        {

          composed._directItems[ i ][ j ] += other._directItems[ i ][ k ] *
                                             _directItems[ k ][ j ];

        }

      }

    }
    gkg::invert( composed._directItems, composed._inverseItems );

    return composed;

  }
  GKG_CATCH( "template < class T > "
             "gkg::HomogeneousTransform3d< T > "
             "gkg::HomogeneousTransform3d< T >::compose( "
             "const HomogeneousTransform3d< T >& other ) const" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setIdentity( bool update,
                                                    bool notify )
{

  setDirectItems( 1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, 0.0, 0.0, 1.0,
                  update,
                  notify );

}


template < class T >
gkg::HomogeneousTransform3d< T > 
gkg::HomogeneousTransform3d< T >::getInverseHomogeneousTransform3d() const
{

  gkg::HomogeneousTransform3d< T > inverseHomegeneousTransform3d;
  int32_t i, j;
  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

      inverseHomegeneousTransform3d._directItems[ i ][ j ] =
        _inverseItems[ i ][ j ];
      inverseHomegeneousTransform3d._inverseItems[ i ][ j ] =
        _directItems[ i ][ j ];

    }

  }
  return inverseHomegeneousTransform3d;

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setDirectItems(
                                                    T d00, T d01, T d02, T d03,
                                                    T d10, T d11, T d12, T d13,
                                                    T d20, T d21, T d22, T d23,
                                                    T d30, T d31, T d32, T d33,
                                                    bool update,
                                                    bool notify )
{

  try
  {

    _directItems[ 0 ][ 0 ] = d00;
    _directItems[ 0 ][ 1 ] = d01;
    _directItems[ 0 ][ 2 ] = d02;
    _directItems[ 0 ][ 3 ] = d03;
    _directItems[ 1 ][ 0 ] = d10;
    _directItems[ 1 ][ 1 ] = d11;
    _directItems[ 1 ][ 2 ] = d12;
    _directItems[ 1 ][ 3 ] = d13;
    _directItems[ 2 ][ 0 ] = d20;
    _directItems[ 2 ][ 1 ] = d21;
    _directItems[ 2 ][ 2 ] = d22;
    _directItems[ 2 ][ 3 ] = d23;
    _directItems[ 3 ][ 0 ] = d30;
    _directItems[ 3 ][ 1 ] = d31;
    _directItems[ 3 ][ 2 ] = d32;
    _directItems[ 3 ][ 3 ] = d33;

    if ( update )
    {

      gkg::invert( _directItems, _inverseItems );

      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >::setDirectItems( "
             "T d00, T d01, T d02, T d03, "
             "T d10, T d11, T d12, T d13, "
             "T d20, T d21, T d22, T d23, "
             "T d30, T d31, T d32, T d33, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setInverseItems(
                                                    T i00, T i01, T i02, T i03,
                                                    T i10, T i11, T i12, T i13,
                                                    T i20, T i21, T i22, T i23,
                                                    T i30, T i31, T i32, T i33,
                                                    bool update,
                                                    bool notify )
{

  try
  {

    _inverseItems[ 0 ][ 0 ] = i00;
    _inverseItems[ 0 ][ 1 ] = i01;
    _inverseItems[ 0 ][ 2 ] = i02;
    _inverseItems[ 0 ][ 3 ] = i03;
    _inverseItems[ 1 ][ 0 ] = i10;
    _inverseItems[ 1 ][ 1 ] = i11;
    _inverseItems[ 1 ][ 2 ] = i12;
    _inverseItems[ 1 ][ 3 ] = i13;
    _inverseItems[ 2 ][ 0 ] = i20;
    _inverseItems[ 2 ][ 1 ] = i21;
    _inverseItems[ 2 ][ 2 ] = i22;
    _inverseItems[ 2 ][ 3 ] = i23;
    _inverseItems[ 3 ][ 0 ] = i30;
    _inverseItems[ 3 ][ 1 ] = i31;
    _inverseItems[ 3 ][ 2 ] = i32;
    _inverseItems[ 3 ][ 3 ] = i33;

    if ( update )
    {

      gkg::invert( _inverseItems, _directItems );
      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >::setInverseItems( "
             "T i00, T i01, T i02, T i03, "
             "T i10, T i11, T i12, T i13, "
             "T i20, T i21, T i22, T i23, "
             "T i30, T i31, T i32, T i33, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setDirectItems( T items[ 4 ][ 4 ],
                                                       bool update,
                                                       bool notify )
{

  try
  {

    int32_t i, j;
    for ( j = 0; j < 4; j++ )
    {

      for ( i = 0; i < 4; i++ )
      {

          _directItems[ i ][ j ] = items[ i ][ j ];

      }

    }
    if ( update )
    {

      gkg::invert( _directItems, _inverseItems );
      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >::setDirectItems( "
             "T items[ 4 ][ 4 ], bool update, bool notify )" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::setInverseItems( T items[ 4 ][ 4 ],
                                                        bool update,
                                                        bool notify )
{

  try
  {

    int32_t i, j;
    for ( j = 0; j < 4; j++ )
    {

      for ( i = 0; i < 4; i++ )
      {

          _inverseItems[ i ][ j ] = items[ i ][ j ];

      }

    }
    if ( update )
    {

      gkg::invert( _inverseItems, _directItems );
      this->setChanged();
      if ( notify )
      {

        this->notifyObservers( this );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >::setInverseItems( "
             "T items[ 4 ][ 4 ], "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::readTrm( std::istream& is )
{

  try
  {

    is >> _directItems[ 0 ][ 3 ];
    is >> _directItems[ 1 ][ 3 ];
    is >> _directItems[ 2 ][ 3 ];

    is >> _directItems[ 0 ][ 0 ];
    is >> _directItems[ 0 ][ 1 ];
    is >> _directItems[ 0 ][ 2 ];

    is >> _directItems[ 1 ][ 0 ];
    is >> _directItems[ 1 ][ 1 ];
    is >> _directItems[ 1 ][ 2 ];

    is >> _directItems[ 2 ][ 0 ];
    is >> _directItems[ 2 ][ 1 ];
    is >> _directItems[ 2 ][ 2 ];

    _directItems[ 3 ][ 0 ] = 0.0;
    _directItems[ 3 ][ 1 ] = 0.0;
    _directItems[ 3 ][ 2 ] = 0.0;
    _directItems[ 3 ][ 3 ] = 1.0;

    gkg::invert( _directItems, _inverseItems );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >readTrm( "
             "std::istream& is )" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::writeTrm( std::ostream& os ) const
{

  try
  {

    os << this->getDirectCoefficient( 0, 3 ) << " "
       << this->getDirectCoefficient( 1, 3 ) << " "
       << this->getDirectCoefficient( 2, 3 ) << std::endl;
    os << this->getDirectCoefficient( 0, 0 ) << " "
       << this->getDirectCoefficient( 0, 1 ) << " "
       << this->getDirectCoefficient( 0, 2 ) << std::endl;
    os << this->getDirectCoefficient( 1, 0 ) << " "
       << this->getDirectCoefficient( 1, 1 ) << " "
       << this->getDirectCoefficient( 1, 2 ) << std::endl;
    os << this->getDirectCoefficient( 2, 0 ) << " "
       << this->getDirectCoefficient( 2, 1 ) << " "
       << this->getDirectCoefficient( 2, 2 ) << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::HomogeneousTransform3d< T >writeTrm( "
             "std::ostream& os ) const" );

}


template < class T >
bool gkg::HomogeneousTransform3d< T >::isIndirect() const
{

  try
  {

    bool isIndirect = false;
    T determinant = gkg::determinant( _directItems );

    if ( determinant < 0.0 )
    {

      isIndirect = true;

    }
    return isIndirect;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::HomogeneousTransform3d< T >::isIndirect() const" );

}


template < class T >
void gkg::HomogeneousTransform3d< T >::copy( 
                                 const gkg::HomogeneousTransform3d< T >& other )
{

  int32_t i, j;
  for ( j = 0; j < 4; j++ )
  {

    for ( i = 0; i < 4; i++ )
    {

      _directItems[ i ][ j ] = other._directItems[ i ][ j ];
      _inverseItems[ i ][ j ] = other._inverseItems[ i ][ j ];

    }

  }
  this->setChanged();
  this->notifyObservers( this );

}


template < class T >
std::ostream& 
std::operator << ( std::ostream& os,
                   const gkg::HomogeneousTransform3d< T >& thing )
{

  try
  {

    int32_t i = 0;
    int32_t j = 0;
    for ( i = 0; i < 4; i++ )
    {

      for ( j = 0; j < 4; j++ )
      {

        os << thing.getDirectCoefficient( i, j ) << " ";

      }
      if ( j != 3 )
      {

        os << std::endl;

      }

    }
    return os;

  }
  GKG_CATCH( "template < class T > "
             "std::ostream& "
             "std::operator << ( std::ostream& os, "
             "const gkg::HomogeneousTransform3d< T >& thing )" );

}


template class gkg::HomogeneousTransform3d< float >;
template class gkg::HomogeneousTransform3d< double >;

template std::ostream& 
std::operator << ( std::ostream& os,
                   const gkg::HomogeneousTransform3d< float >& thing );
template std::ostream& 
std::operator << ( std::ostream& os,
                   const gkg::HomogeneousTransform3d< double >& thing );
