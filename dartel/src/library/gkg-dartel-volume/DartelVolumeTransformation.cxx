#include <gkg-dartel-volume/DartelVolumeTransformation.h>
#include <gkg-dartel-core/DartelExponentiation.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <cstdlib>


static double wt2( double x )
{

  x = std::fabs( x );

  if ( x < 0.5 )
  {

    return( 0.75 - x * x );

  }
  if ( x < 1.5 )
  {

    x = 1.5 - x;

    return 0.5 * x * x;

  }

  return 0.0;

}


static double wt3( double x )
{

  x = std::fabs( x );

  if ( x < 1.0 )
  {

    return ( x * x * ( x - 2.0 ) * 0.5 + 2.0 / 3.0 );

  }
  if ( x < 2.0 )
  {

    x = 2.0 - x;

    return x * x * x / 6.0;

  }

  return 0.0;

}


static double wt4( double x )
{

  x = std::fabs( x );

  if ( x < 0.5 )
  {

    x *= x;

    return ( x * ( x * 0.25 - 0.625 ) + 115.0 / 192.0 );

  }
  if ( x < 1.5 )
  {

    return ( x * ( x * ( x * ( 5.0 / 6.0 - x / 6.0 ) - 1.25 ) + 
                   5.0 / 24.0 ) + 55.0 / 96.0 );

  }
  if ( x < 2.5 )
  {

    x -= 2.5;
    x *= x;

    return x * x / 24.0;

  }

  return 0.0;

}


static double wt5( double x )
{

  double y;

  x = std::fabs( x );

  if ( x < 1.0 )
  {

    y = x * x;

    return ( y * ( y * ( 0.25 - x / 12.0 ) - 0.5 ) + 0.55 );

  }
  if ( x < 2.0 )
  {

    return ( x * ( x * ( x * ( x * ( x / 24.0 - 0.375 ) + 1.25 ) - 1.75 ) + 
                   0.625 ) + 0.425 );

  }
  if ( x < 3.0 )
  {

    y = 3.0 - x;
    x = y * y;

    return y * x * x / 120.0;

  }

  return 0.0;

}


static double wt6( double x )
{
  x = std::fabs( x );

  if ( x < 0.5 )
  {

    x *= x;

    return ( x * ( x * ( 7.0 / 48.0 - x / 36.0 ) - 77.0 / 192.0 ) +
             5887.0 / 11520.0 );

  }
  if ( x < 1.5 )
  {

    return ( x * ( x * ( x * ( x * ( x * ( x / 48.0 - 7.0 / 48.0 ) + 
                                     0.328125 ) -
	           35.0 / 288.0 ) - 91.0 / 256.0 ) - 7.0 / 768.0 ) + 
             7861.0 / 15360.0 );

  }
  if ( x < 2.5 )
  {

    return ( x * ( x * ( x * ( x * ( x * ( 7.0 / 60.0 - x / 120.0 ) - 
                                     0.65625 ) + 133.0 / 72.0 ) - 2.5703125 ) +
                   1267.0 / 960.0 ) + 1379.0 / 7680.0 );

  }
  if ( x < 3.5 )
  {

    x -= 3.5;
    x *= x * x;

    return x * x / 720.0;

  }

  return 0.0;

}


static double wt7( double x )
{

  double y;

  x = std::fabs( x );

  if ( x < 1.0 )
  {

    y = x * x;

    return ( y * ( y * ( y * ( x / 144.0 - 1.0 / 36.0 ) + 1.0 / 9.0 ) - 
                   1.0 / 3.0 ) + 151.0 / 315.0 );

  }
  if ( x < 2.0 )
  {

    return ( x * ( x * ( x * ( x * ( x * ( x * ( 0.05 - x / 240.0 ) - 
                                           7.0 / 30.0 ) + 0.5 ) -
	                       7.0 / 18.0 ) - 0.1 ) - 7.0 / 90.0 ) +
             103.0 / 210.0 );

  }
  if ( x < 3.0 )
  {

    return ( x * ( x * ( x * ( x * ( x * ( x * ( x / 720.0 - 1.0 / 36.0 ) +
                                           7.0 / 30.0 ) - 19.0 / 18.0 ) +
                               49.0 / 18.0 ) - 23.0 / 6.0 ) + 217.0 / 90.0 ) - 
             139.0 / 630.0 );

  }
  if ( x < 4.0 )
  {

    y = 4.0 - x;
    x = y * y * y;

    return x * x * y / 5040.0;

  }

  return 0.0;

}


template < class S, class T >
gkg::DartelVolumeTransformation< S, T >::DartelVolumeTransformation(
                                                                int32_t k,
                                                                int32_t degree )
                                       : gkg::DartelTransformation< S, T >(
                                                                        k,
                                                                        degree )
{
}


template < class S, class T >
gkg::DartelVolumeTransformation< S, T >::~DartelVolumeTransformation()
{
}


template < class S, class T >
void gkg::DartelVolumeTransformation< S, T >::wrap(
                                              const gkg::Volume< S >& inVolume,
                                              const gkg::Volume< T >& flowField,
                                              gkg::Volume< S >& outVolume )
{

  try
  {

    gkg::Volume< T > expU;
    gkg::Volume< S > templateProxy;
    gkg::Vector3d< T > p;
    gkg::Vector3d< double > resolution;
    gkg::Vector3d< double > s0;
    gkg::Vector3d< double > s1;
    int32_t n0 = flowField.getSizeX();
    int32_t n1 = flowField.getSizeY();
    int32_t n2 = flowField.getSizeZ();
    int32_t x, y, z;
    int32_t m0 = inVolume.getSizeX();
    int32_t m1 = inVolume.getSizeY();
    int32_t m2 = inVolume.getSizeZ();
    const S* c = &inVolume( 0 );
    float min = -1e-6;
    float max0 = float( m0 ) + 1e-6;
    float max1 = float( m1 ) + 1e-6;
    float max2 = float( m2 ) + 1e-6;

    flowField.getResolution( resolution );
    outVolume.reallocate( n0, n1, n2 );
    outVolume.setResolution( resolution );
    outVolume.fill( 0 );

    gkg::DartelExponentiation< T > dartel( this->_k );
    dartel.getInverseVectorField( flowField, expU );
    this->affineTransformation( inVolume, flowField, templateProxy, false );

    expU.getResolution( s0 );
    inVolume.getResolution( s1 );

    for ( z = 0; z < n2; z++ )
    {

      for ( y = 0; y < n1; y++ )
      {

        for ( x = 0; x < n0; x++ )
        {

          T x0 = expU( x, y, z, 0 ) * s0.x;
          T x1 = expU( x, y, z, 1 ) * s0.y;
          T x2 = expU( x, y, z, 2 ) * s0.z;

          this->_m.getInverse( gkg::Vector3d< T >( x0, x1, x2 ), p );
	  p.x /= s1.x;
	  p.y /= s1.y;
	  p.z /= s1.z;

          if ( ( p.x >= min ) && ( p.x < max0 ) && 
               ( p.y >= min ) && ( p.y < max1 ) &&
               ( p.z >= min ) && ( p.z < max2 ) )
          {

            outVolume( x, y, z ) = sample3( c, m0, m1, m2, p.x, p.y, p.z );

          }

        }

      }

    }

    setHeader( flowField, outVolume, templateProxy, true );

  }
  GKG_CATCH( "template < class S, class T > "
             "void gkg::DartelVolumeTransformation< S, T >::wrap( "
             "const gkg::Volume< S >& inVolume, "
             "const gkg::Volume< T >& flowField, "
             "gkg::Volume< S >& outVolume )" );

}


template < class S, class T >
void gkg::DartelVolumeTransformation< S, T >::unwrap(
                 			  const gkg::Volume< S >& inVolume,
                 			  const gkg::Volume< T >& flowField, 
                 			  const gkg::Volume< S >& templateProxy,
                 			  gkg::Volume< S >& outVolume )
{

  try
  {

    gkg::Volume< T > expU;
    gkg::Vector3d< T > p;
    gkg::Vector3d< double > s0;
    gkg::Vector3d< double > s1;
    int32_t n0, n1, n2, x, y, z;
    int32_t m0 = inVolume.getSizeX();
    int32_t m1 = inVolume.getSizeY();
    int32_t m2 = inVolume.getSizeZ();
    const S* c = &inVolume( 0 );
    float min = -1e-6;
    float max0 = float( m0 ) + 1e-6;
    float max1 = float( m1 ) + 1e-6;
    float max2 = float( m2 ) + 1e-6;

    if ( templateProxy.getSizeXYZ() > 1 )
    {

      n0 = templateProxy.getSizeX();
      n1 = templateProxy.getSizeY();
      n2 = templateProxy.getSizeZ();
      templateProxy.getResolution( s1 );

    }
    else
    {

      n0 = flowField.getSizeX();
      n1 = flowField.getSizeY();
      n2 = flowField.getSizeZ();
      flowField.getResolution( s1 );

    }

    outVolume.reallocate( n0, n1, n2 );
    outVolume.setResolution( s1 );
    outVolume.fill( 0 );

    gkg::DartelExponentiation< T > dartel( this->_k );
    dartel.getDirectVectorField( flowField, expU );
    this->affineTransformation( inVolume, flowField, templateProxy, true );

    expU.getResolution( s0 );

    for ( z = 0; z < n2; z++ )
    {

      for ( y = 0; y < n1; y++ )
      {

        for ( x = 0; x < n0; x++ )
        {

          this->_m.getDirect( gkg::Vector3d< T >( x * s1.x,
                                                  y * s1.y,
                                                  z * s1.z ),
	                      p );

          gkg::Vector3d< int32_t > q( int32_t( p.x / s0.x + 0.5f ),
                                      int32_t( p.y / s0.y + 0.5f ),
                                      int32_t( p.z / s0.z + 0.5f ) );

          if ( ( q.x >= 0 ) && ( q.y >= 0 ) && ( q.z >= 0 ) &&
               ( q.x < m0 ) && ( q.y < m1 ) && ( q.z < m2 ) )
          {

            float x0 = expU( q, 0 );
            float x1 = expU( q, 1 );
            float x2 = expU( q, 2 );

            if ( ( x0 >= min ) && ( x0 < max0 ) && 
                 ( x1 >= min ) && ( x1 < max1 ) &&
                 ( x2 >= min ) && ( x2 < max2 ) )
            {

              outVolume( x, y, z ) = sample3( c, m0, m1, m2, x0, x1, x2 );

            }

          }

        }

      }

    }

    setHeader( flowField, outVolume, templateProxy, true );

  }
  GKG_CATCH( "template < class S, class T > "
             "void gkg::DartelVolumeTransformation< S, T >::unwrap( "
             "const gkg::Volume< S >& inVolume, "
             "const gkg::Volume< T >& flowField, "
             "const gkg::Volume< S >& templateProxy, "
             "gkg::Volume< S >& outVolume )" );

}


template < class S, class T >
void gkg::DartelVolumeTransformation< S, T >::weights( float x, 
                                                       int32_t* i, 
                                                       float w[] )
{

  try
  {

    int32_t k;
    *i = (int32_t)floor( x - ( this->_degree - 1 ) * 0.5f );
    x -= (float)*i;

    switch ( this->_degree )
    {

      case 0:
        w[ 0 ] = 1.0; /* Not correct at discontinuities */
        break;

      case 1:
        w[ 0 ] = 1.0 - x;
        w[ 1 ] = x;
        break;

      case 2:
        for ( k = 0; k <= 2; k++ )
        {

          w[ k ] = wt2( x - k );

        }
        break;

      case 3:
        for ( k = 0; k <= 3; k++ )
        {

          w[ k ] = wt3( x - k );

        }
        break;

      case 4:
        for ( k = 0; k <= 4; k++ )
        {

          w[ k ] = wt4( x - k );

        }
        break;

      case 5:
        for ( k = 0; k <= 5; k++ )
        {

          w[ k ] = wt5( x - k );

        }
        break;

      case 6:
        for ( k = 0; k <= 6; k++ )
        {

          w[ k ] = wt6( x - k );

        }
        break;

      case 7:
      default:
        for ( k = 0; k <= 7; k++ )
        {

          w[ k ] = wt7( x - k );

        }
        break;

    }

  }
  GKG_CATCH( "template < class S, class T > "
             "void gkg::DartelVolumeTransformation< S, T >::weights( "
             "float x, int32_t* i, float w[] )" );

}


template < class S, class T >
S gkg::DartelVolumeTransformation< S, T >::sample3( const S* c, 
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
    const S *cp;

    /* Generate seperable B-spline basis functions */
    weights( x0, &i0, w0 );
    weights( x1, &i1, w1 );
    weights( x2, &i2, w2 );

    /* Create lookups of voxel locations - for coping with edges */
    for ( k = 0; k <= this->_degree; k++ )
    {

      o0[ k ] = this->mirror( k + i0, m0 );
      o1[ k ] = this->mirror( k + i1, m1 ) * m0;
      o2[ k ] = this->mirror( k + i2, m2 ) * m0 * m1;

    }

    /* Convolve coefficients with basis functions */
    d2 = 0.0f;
    for ( i2 = 0; i2 <= this->_degree; i2++ )
    {

      d1 = 0.0f;
      for ( i1 = 0; i1 <= this->_degree; i1++ )
      {

        cp = c + o2[ i2 ] + o1[ i1 ];
        d0 = 0.0f;

        for ( i0 = 0; i0 <= this->_degree; i0++ )
        {

          d0 += (float)cp[ o0[ i0 ] ] * w0[ i0 ];

        }

        d1 += d0 * w1[ i1 ];

      }

      d2 += d1 * w2[ i2 ];

    }

    return S( d2 );

  }
  GKG_CATCH( "template < class S, class T > "
             "S gkg::DartelVolumeTransformation< S, T >::sample3( S* c, "
             "int32_t m0, int32_t m1, int32_t m2, float x0, float x1, "
             "float x2, int32_t d[] )" );

}


template < class S, class T >
void gkg::DartelVolumeTransformation< S, T >::setHeader( 
                                          const gkg::Volume< T >& flowfield,
                                          gkg::Volume< S >& outVolume,
                                          const gkg::Volume< S >& templateProxy,
                                          bool inverse )
{

  try
  {

    gkg::GenericObjectList glistR, glistT;

    if ( inverse &&
    	 templateProxy.getHeader().hasAttribute( "referentials" ) &&
    	 templateProxy.getHeader().hasAttribute( "transformations" ) )
    {

    	templateProxy.getHeader().getAttribute( "referentials", glistR );
    	templateProxy.getHeader().getAttribute( "transformations", glistT );

    	outVolume.getHeader().addAttribute( "referentials", glistR );
    	outVolume.getHeader().addAttribute( "transformations", glistT );

    }
    else if ( flowfield.getHeader().hasAttribute( "referentials" ) &&
    	      flowfield.getHeader().hasAttribute( "transformations" ) )
    {

      flowfield.getHeader().getAttribute( "referentials", glistR );
      flowfield.getHeader().getAttribute( "transformations", glistT );

      int32_t idx = inverse ? 0 : 1;
      gkg::GenericObjectList glistR2, glistT2;
      glistR2.push_back( glistR[ idx ] );
      glistR2.push_back( glistR[ idx ] );
      glistT2.push_back( glistT[ idx ] );
      glistT2.push_back( glistT[ idx ] );
      outVolume.getHeader().addAttribute( "referentials", glistR2 );
      outVolume.getHeader().addAttribute( "transformations", glistT2 );

    }

  }
  GKG_CATCH( "template < class S, class T > "
             "void gkg::DartelVolumeTransformation< S, T >::set_header(  "
             "const gkg::Volume< T >& flowfield, gkg::Volume< S >& outVolume, "
             "const gkg::Volume< S >& templateProxy, bool inverse )" );

}


template class gkg::DartelVolumeTransformation< uint8_t, float >;
template class gkg::DartelVolumeTransformation< int8_t, float >;
template class gkg::DartelVolumeTransformation< uint16_t, float >;
template class gkg::DartelVolumeTransformation< int16_t, float >;
template class gkg::DartelVolumeTransformation< uint32_t, float >;
template class gkg::DartelVolumeTransformation< int32_t, float >;
template class gkg::DartelVolumeTransformation< uint64_t, float >;
template class gkg::DartelVolumeTransformation< int64_t, float >;
template class gkg::DartelVolumeTransformation< float, float >;
template class gkg::DartelVolumeTransformation< double, float >;

template class gkg::DartelVolumeTransformation< uint8_t, double >;
template class gkg::DartelVolumeTransformation< int8_t, double >;
template class gkg::DartelVolumeTransformation< uint16_t, double >;
template class gkg::DartelVolumeTransformation< int16_t, double >;
template class gkg::DartelVolumeTransformation< uint32_t, double >;
template class gkg::DartelVolumeTransformation< int32_t, double >;
template class gkg::DartelVolumeTransformation< uint64_t, double >;
template class gkg::DartelVolumeTransformation< int64_t, double >;
template class gkg::DartelVolumeTransformation< float, double >;
template class gkg::DartelVolumeTransformation< double, double >;
