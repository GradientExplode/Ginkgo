#include <gkg-dartel-core/DartelExponentiation.h>
#include <gkg-dartel-core/DartelCompositionContext.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>

#include <cstring>


static int32_t pow2( int32_t k )
{

    int32_t v = 1;

    while ( k-- )
    {

      v <<= 1;

    }

    return v;

}


template < class T >
gkg::DartelExponentiation< T >::DartelExponentiation( int32_t k )
                              : _k( k ),
                                _sizeX( 0 ),
                                _sizeY( 0 ),
                                _sizeZ( 0 ),
                                _sizeT( 0 ),
                                _sizeXYZ( 0 )
{
}


template < class T >
gkg::DartelExponentiation< T >::~DartelExponentiation()
{
}


template < class T >
void gkg::DartelExponentiation< T >::getDirectVectorField( 
                                              const gkg::Volume< T >& flowfield,
                                              gkg::Volume< T >& expU )
{

  try
  {
  
    dartel_integrate( flowfield, 0.0, 1.0, expU );
    
  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::getDirect( "
             "const gkg::Volume< T >& flowfield, gkg::Volume< T >& expU )" );

}


template < class T >
void gkg::DartelExponentiation< T >::getInverseVectorField(
                                              const gkg::Volume< T >& flowfield,
                                              gkg::Volume< T >& expU )
{

  try
  {
  
    dartel_integrate( flowfield, 1.0, 0.0, expU );
    
  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::getInverse( "
             "const gkg::Volume< T >& flowfield, gkg::Volume< T >& expU )" );

}


/*
 * t0 = Id + v0 * sc
 */
template < class T >
void gkg::DartelExponentiation< T >::smalldef( double sc, const T *v0, 
                                               T* t0, float* m )
{

  try
  {

    int32_t j0, j1, j2;
    const T *v1 = v0 + _sizeXYZ;
    const T *v2 = v1 + _sizeXYZ;
    T *t1 = t0 + _sizeXYZ;
    T *t2 = t1 + _sizeXYZ;
    float r00 = m[ 0 ];
    float r01 = m[ 1 ];
    float r02 = m[ 2 ];
    float r10 = m[ 4 ];
    float r11 = m[ 5 ];
    float r12 = m[ 6 ];
    float r20 = m[ 8 ];
    float r21 = m[ 9 ];
    float r22 = m[ 10 ];

    for ( j2 = 0; j2 < _sizeZ; j2++ )
    {

      for ( j1 = 0; j1 < _sizeY; j1++ )
      {

        for ( j0 = 0; j0 < _sizeX; j0++ )
        {

          T a0 = *v0++;
          T a1 = *v1++;
          T a2 = *v2++;
          *t0++ = j0 + sc * ( a0 * r00 + a1 * r01 + a2 * r02 );
          *t1++ = j1 + sc * ( a0 * r10 + a1 * r11 + a2 * r12 );
          *t2++ = j2 + sc * ( a0 * r20 + a1 * r21 + a2 * r22 );

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::smalldef( "
             "double sc, T *v0, T* t0, float* m )" );

}


template < class T >
void gkg::DartelExponentiation< T >::expdef( int32_t k, double sc, const T* v, 
                                             T* t0, T* t1, float* m )
{

  try
  {

    T *optr = t0;
    int32_t j;

    smalldef( sc / pow2( k ), v, t0, m );

    for ( j = 0; j < k; j++ )
    {

      // Composition operation  C(Id) = B(A(Id))
      gkg::DartelCompositionContext< T > context( _sizeX, _sizeY, _sizeZ, 
                                                  t0, t0, t1 );
      gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, _sizeXYZ );

      threadedLoop.launch();

      T* tmpp = t0;
      t0 = t1;
      t1 = tmpp;

    }

    if ( optr != t0 )
    {

      std::memcpy( t1, t0, 3 * _sizeXYZ * sizeof( T ) );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::expdef( "
             "int32_t k, double sc, T* v, T* t0, T* t1, float* m )" );

}


template < class T >
void gkg::DartelExponentiation< T >::unwrap( T* f )
{

  try
  {

    int32_t sizeXY = _sizeX * _sizeY;
    int32_t i0, i1, i2;

    T* pt = f + 2 * _sizeXYZ;
    for ( i1 = 0; i1 < sizeXY; i1++ )
    {

      pt[ i1 ] = pt[ i1 ] - floor( pt[ i1 ] / _sizeZ + 0.5 ) * _sizeZ;

    }
    for( i2 = 1, pt += sizeXY; i2 < _sizeZ; i2++, pt += sizeXY )
    {

      for ( i1 = 0; i1 < sizeXY; i1++ )
      {

        pt[ i1 ] = pt[ i1 ] - 
                   floor( ( pt[ i1 ] - pt[ i1 - sizeXY ] ) / _sizeZ + 
                          0.5 ) * _sizeZ;

      }

    }

    pt = f + _sizeXYZ;
    T* pt1 = pt;
    for ( i2 = 0; i2 < _sizeZ; i2++, pt1 += sizeXY )
    {

      for ( i0 = 0; i0 < _sizeX; i0++ )
      {

        pt1[ i0 ] = pt1[ i0 ] - floor( pt1[ i0 ] / _sizeY + 0.5 ) * _sizeY;

      }

    }
    for ( i1 = 1, pt += _sizeX; i1 < _sizeY; i1++, pt += _sizeX)
    {

      for (i2 = 0, pt1 = pt; i2 < _sizeZ; i2++, pt1 += sizeXY )
      {

        for ( i0 = 0; i0 < _sizeX; i0++ )
        {

          pt1[ i0 ] = pt1[ i0 ] - 
                      floor( ( pt1[ i0 ] - pt1[ i0 - _sizeX ] ) / _sizeY + 
                             0.5 ) * _sizeY;

        }

      }

    }

    pt = f;
    pt1 = pt;
    for ( i2 = 0; i2 < _sizeZ; i2++, pt1 += sizeXY )
    {
 
      for ( i1 = 0; i1 < sizeXY; i1 += _sizeX )
      {

        pt1[ i1 ] = pt1[ i1 ] - floor( pt1[ i1 ] / _sizeX + 0.5 ) * _sizeX;

      }

    }
    for ( i0 = 1, pt++; i0 < _sizeX; i0++, pt++ )
    {

      for ( i2 = 0, pt1 = pt; i2 < _sizeZ; i2++, pt1 += sizeXY )
      {

        for ( i1 = 0; i1 < sizeXY; i1 += _sizeX )
        {

          pt1[ i1 ] = pt1[ i1 ] -
                      floor( ( pt1[ i1 ] - pt1[ i1 - 1 ] ) / _sizeX +
                             0.5 ) * _sizeX;

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::unwrap( T* f )" );

}


template < class T >
void gkg::DartelExponentiation< T >::dartel3_exp( const gkg::Volume< T >& U,
                                                  int32_t k, double sc,
                                                  gkg::Volume< T >& expU )
{

  try
  {

    gkg::Vector3d< double > resolution;
    float m[] = { 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f };

    _sizeX = U.getSizeX();
    _sizeY = U.getSizeY();
    _sizeZ = U.getSizeZ();
    _sizeT = U.getSizeT();
    _sizeXYZ = U.getSizeXYZ();

    U.getResolution( resolution );
    expU.reallocate( _sizeX, _sizeY, _sizeZ, _sizeT );
    expU.setResolution( resolution );

    gkg::Volume< T > t1( _sizeX, _sizeY, _sizeZ, _sizeT );
    t1.setResolution( resolution );

    if ( U.getHeader().hasAttribute( "storage_to_memory" ) )
    {

      gkg::GenericObjectList s2m;
      int32_t i;

      U.getHeader().getAttribute( "storage_to_memory", s2m );

      for ( i = 0; i < 16; i++ )
      {

        m[ i ] = s2m[ i ]->getScalar();

      }

    }

    expdef( k, sc, &U( 0 ), &expU( 0 ), &t1( 0 ), m );
    unwrap( &expU( 0 ) );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::dartel3_exp( "
             "gkg::Volume< T >& U, int32_t k, double sc, "
             "gkg::Volume< T >& expU )" );

}


template < class T >
void gkg::DartelExponentiation< T >::dartel_integrate( 
                                                      const gkg::Volume< T >& U,
                                                      double t0, double t1,
                                                      gkg::Volume< T >& expU )
{

  try
  {

    int32_t nt = 1;
    int32_t k = (int32_t)std::ceil( ::log2( pow2( _k ) / nt ) );
    double big = pow2( 12 );
    double tt0 = std::min( std::max( t0, 0.0 ), 1.0 );
    double tt1 = std::min( std::max( t1, 0.0 ), 1.0 );
    double sc = 0.0;
    int32_t ts = 0;

    if ( tt1 > tt0 )
    {

      sc = std::max( ::round( ( std::min( tt1, 1.0 ) - 
                                std::max( tt0, 0.0 ) ) * big ) / big, 0.0 );

    }
    else
    {

      sc = std::max( ::round( ( std::min( tt0, 1.0 ) - 
                                std::max( tt1, 0.0 ) ) * big ) / big, 0.0 );

    }

    if ( tt0 == tt1 )
    {

      sc = 0.0;
      ts = 0;

    }
    else
    {

      ts = (int32_t)std::ceil( ::log2( pow2( k ) * sc ) - 1.0 / big );
      sc *= pow2( k - ts );

      if ( tt0 > tt1 )
      {

        sc *= -1.0;

      }

    }

    dartel3_exp( U, ts, sc, expU );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelExponentiation< T >::dartel_integrate( "
             "const gkg::Volume< T >& U, double t0, double t1, "
             "gkg::Volume< T >& expU )" );

}


template class gkg::DartelExponentiation< float >;
template class gkg::DartelExponentiation< double >;
