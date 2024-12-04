#include <gkg-dartel-core/DartelTransformation.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <cstdlib>


template < class S, class T >
gkg::DartelTransformation< S, T >::DartelTransformation( int32_t k,
                                                         int32_t degree )
                                 : _k( k ),
                                   _degree( degree )
{
}


template < class S, class T >
gkg::DartelTransformation< S, T >::~DartelTransformation()
{
}


template < class S, class T >
void gkg::DartelTransformation< S, T >::affineTransformation( 
                                          const gkg::Volume< S >& inVolume,
                                          const gkg::Volume< T >& U,
                                          const gkg::Volume< S >& templateProxy,
                                               bool inverse )
{

  try
  {

    gkg::HomogeneousTransform3d< T > m0, m1;
    gkg::GenericObjectList transformations;
    std::vector< T > c( 16 );
    int32_t i;

    if ( U.getHeader().hasAttribute( "transformations" ) )
    {
  
      int idx = 0;

      if ( inverse && 
           !templateProxy.getHeader().hasAttribute( "transformations" ) )
      {

        idx = 1;

      }

      U.getHeader().getAttribute( "transformations", transformations );
      gkg::GenericObjectList transf = 
                   transformations[ idx ]->getValue< gkg::GenericObjectList >();

      for ( i = 0; i < 16; i++ )
      {

        c[ i ] = transf[ i ]->getScalar();

      }

      m0.setDirectItems( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ],
                         c[ 4 ], c[ 5 ], c[ 6 ], c[ 7 ], 
                         c[ 8 ], c[ 9 ], c[ 10 ], c[ 11 ], 
                         c[ 12 ], c[ 13 ], c[ 14 ], c[ 15 ] );

    }

    if ( inverse && 
         templateProxy.getHeader().hasAttribute( "transformations" ) )
    {

        templateProxy.getHeader().getAttribute( "transformations",
                                                transformations );
        gkg::GenericObjectList transf = 
                     transformations[ 1 ]->getValue< gkg::GenericObjectList >();
  
        for ( i = 0; i < 16; i++ )
        {

          c[ i ] = transf[ i ]->getScalar();

        }

        m1.setDirectItems( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ],
                           c[ 4 ], c[ 5 ], c[ 6 ], c[ 7 ], 
                           c[ 8 ], c[ 9 ], c[ 10 ], c[ 11 ], 
                           c[ 12 ], c[ 13 ], c[ 14 ], c[ 15 ] );

    }
    else if ( inVolume.getHeader().hasAttribute( "transformations" ) )
    {

      inVolume.getHeader().getAttribute( "transformations", transformations );
      gkg::GenericObjectList transf = 
                     transformations[ 0 ]->getValue< gkg::GenericObjectList >();

      for ( i = 0; i < 16; i++ )
      {

        c[ i ] = transf[ i ]->getScalar();

      }

      m1.setDirectItems( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ],
                         c[ 4 ], c[ 5 ], c[ 6 ], c[ 7 ], 
                         c[ 8 ], c[ 9 ], c[ 10 ], c[ 11 ], 
                         c[ 12 ], c[ 13 ], c[ 14 ], c[ 15 ] );

    }
  
    _m = m1.compose( m0.getInverseHomogeneousTransform3d() );

  }
  GKG_CATCH( "template < class S, class T > "
             "void gkg::DartelTransformation< S, T >::affineTransformation( "
             "gkg::Volume< S >& inVolume, gkg::Volume< T >& U, "
             "gkg::Volume< S >& templateProxy, bool inverse )" );

}


template < class S, class T >
void gkg::DartelTransformation< S, T >::weights( float x, 
                                                 int32_t* i, 
                                                 float w[] )
{

  *i = (int32_t)floor( x );
  x -= float( *i );
  w[ 0 ] = 1.0f - x;
  w[ 1 ] = x;

}


template < class S, class T >
int32_t gkg::DartelTransformation< S, T >::mirror( int32_t i, int32_t m )
{

  int32_t m2;

  i = std::abs( i );

  if ( i < m )
  {

    return i;

  }
  if ( m == 1 )
  {

    return 0;

  }

  m2 = ( m - 1 ) * 2;
  i %= m2;

  return ( i < m ) ? i : m2 - i;

}


template < class S, class T >
T gkg::DartelTransformation< S, T >::sample1( T* c, 
                                              int32_t m0, 
                                              int32_t m1, 
                                              int32_t m2,
                                              float x0,
                                              float x1, 
                                              float x2 )
{

  try
  {

    float w0[ 2 ], w1[ 2 ], w2[ 2 ]; /* B-spline weights */
    int32_t o0[ 2 ], o1[ 2 ], o2[ 2 ]; /* Offsets */
    int32_t i0, i1, i2; /* Initial offsets */
    float d0, d1, d2; /* Used by seperable convolution */
    int32_t k;
    T *cp;

    /* Generate seperable B-spline basis functions */
    weights( x0, &i0, w0 );
    weights( x1, &i1, w1 );
    weights( x2, &i2, w2 );

    /* Create lookups of voxel locations - for coping with edges */
    for ( k = 0; k <= 1; k++ )
    {

      o0[ k ] = mirror( k + i0, m0 );
      o1[ k ] = mirror( k + i1, m1 ) * m0;
      o2[ k ] = mirror( k + i2, m2 ) * m0 * m1;

    }

    /* Convolve coefficients with basis functions */
    d2 = 0.0f;
    for ( i2 = 0; i2 <= 1; i2++ )
    {

      d1 = 0.0f;
      for( i1 = 0; i1 <= 1; i1++ )
      {

        cp = c + o2[ i2 ] + o1[ i1 ];
        d0 = cp[ o0[ 0 ] ] * w0[ 0 ] + cp[ o0[ 1 ] ] * w0[ 1 ];
        d1 += d0 * w1[ i1 ];

      }

      d2 += d1 * w2[ i2 ];

    }

    return d2;

  }
  GKG_CATCH( "template < class S, class T > "
             "T gkg::DartelTransformation< S, T >::sample1( T* c, "
             "int32_t m0, int32_t m1, int32_t m2, float x0, "
             "float x1, float x2 )" );

}


template class gkg::DartelTransformation< uint8_t, float >;
template class gkg::DartelTransformation< int8_t, float >;
template class gkg::DartelTransformation< uint16_t, float >;
template class gkg::DartelTransformation< int16_t, float >;
template class gkg::DartelTransformation< uint32_t, float >;
template class gkg::DartelTransformation< int32_t, float >;
template class gkg::DartelTransformation< uint64_t, float >;
template class gkg::DartelTransformation< int64_t, float >;
template class gkg::DartelTransformation< float, float >;
template class gkg::DartelTransformation< double, float >;

template class gkg::DartelTransformation< uint8_t, double >;
template class gkg::DartelTransformation< int8_t, double >;
template class gkg::DartelTransformation< uint16_t, double >;
template class gkg::DartelTransformation< int16_t, double >;
template class gkg::DartelTransformation< uint32_t, double >;
template class gkg::DartelTransformation< int32_t, double >;
template class gkg::DartelTransformation< uint64_t, double >;
template class gkg::DartelTransformation< int64_t, double >;
template class gkg::DartelTransformation< float, double >;
template class gkg::DartelTransformation< double, double >;
