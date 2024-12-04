#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>


#define EPSILON   1.192092896e-7


template < class T >
double gkg::SplineResamplerTraits< T >::norm2(
                const gkg::SplineResamplerTraits< T >::coefficient_type& value )
{

  return value * value;

}


double gkg::SplineResamplerTraits< std::complex< float > >::norm2(
    const gkg::SplineResamplerTraits< std::complex< float > >::coefficient_type&
                                                                         value )
{

  return std::norm< double >( value );

}


double gkg::SplineResamplerTraits< std::complex< double > >::norm2(
   const gkg::SplineResamplerTraits< std::complex< double > >::coefficient_type&
                                                                         value )
{

  return std::norm< double >( value );

}




double gkg::SplineResamplerTraits< gkg::RGBComponent >::norm2(
   const gkg::SplineResamplerTraits< gkg::RGBComponent >::coefficient_type&
                                                                         value )
{

  return std::sqrt( ( double )value.r * ( double )value.r +
                    ( double )value.g * ( double )value.g +
                    ( double )value.b * ( double )value.b );

}




template < class T >
gkg::SplineResampler< T >::SplineResampler()
                          : gkg::Resampler< T >(),
                            _reset( true )
{
}


template < class T >
gkg::SplineResampler< T >::~SplineResampler()
{
}


template < class T >
void gkg::SplineResampler< T >::resample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< float >& transform3d,
                                  const T& outBackground,
                                  gkg::Volume< T >& outVolume,
                                  bool verbose )
{

  try
  {

    updateSplineCoefficients( inVolume, verbose );
    this->gkg::Resampler< T >::resample( inVolume,
                                         transform3d,
                                         outBackground,
                                         outVolume,
                                         verbose );

  }
  GKG_CATCH( "template < class T > void gkg::SplineResampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< float >& transform3d, "
             "const T& outBackground, gkg::Volume< T >& outVolume, "
             "bool verbose )" );

}


template < class T >
void gkg::SplineResampler< T >::resample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< double >& transform3d,
                                  const T& outBackground,
                                  gkg::Volume< T >& outVolume,
                                  bool verbose )
{

  try
  {

    updateSplineCoefficients( inVolume, verbose );
    this->gkg::Resampler< T >::resample( inVolume,
                                         transform3d,
                                         outBackground,
                                         outVolume,
                                         verbose );

  }
  GKG_CATCH( "template < class T > void gkg::SplineResampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< double >& transform3d, "
             "const T& outBackground, gkg::Volume< T >& outVolume, "
             "bool verbose )" );

}


template < class T >
void gkg::SplineResampler< T >::resample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< float >& transform3d,
                                  const T& outBackground,
                                  const gkg::Vector3d< float >& outLocation,
                                  T& outValue )
{

  try
  {

    updateSplineCoefficients( inVolume, false );
    this->gkg::Resampler< T >::resample( inVolume,
                                         transform3d,
                                         outBackground,
                                         outLocation,
                                         outValue );

  }
  GKG_CATCH( " template < class T > void gkg::SplineResampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< float >& transform3d, "
             "const T& outBackground, "
             "const gkg::Vector3d< float >& outLocation,  T& outValue )" );

}


template < class T >
void gkg::SplineResampler< T >::resample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< double >& transform3d,
                                  const T& outBackground,
                                  const gkg::Vector3d< double >& outLocation,
                                  T& outValue )
{

  try
  {

    updateSplineCoefficients( inVolume, false );
    this->gkg::Resampler< T >::resample( inVolume,
                                         transform3d,
                                         outBackground,
                                         outLocation,
                                         outValue );

  }
  GKG_CATCH( " template < class T > void gkg::SplineResampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< double >& transform3d, "
             "const T& outBackground, "
             "const gkg::Vector3d< double >& outLocation,  T& outValue )" );

}


template < class T >
void gkg::SplineResampler< T >::reset()
{

  _reset = true;

}


template < class T >
void 
gkg::SplineResampler< T >::doResample( const Volume< T >& inVolume,
                                       const Transform3d< float >& transform3d,
                                       const T& outBackground,
                                       const Vector3d< float >& outLocation,
                                       T& outValue )
{

  try
  {

    int32_t order = this->getOrder();
    int32_t half = order / 2;
    int32_t width = order + 1;

    typename gkg::Volume< typename
             gkg::SplineResamplerTraits< T >::coefficient_type >::const_iterator
      s = _splineCoefficients.begin();
    typename gkg::Volume< typename
             gkg::SplineResamplerTraits< T >::coefficient_type >::const_iterator
      pi, pj;

    gkg::Vector3d< float > normalizedInLocation;
    transform3d.getInverse( outLocation, normalizedInLocation );

    normalizedInLocation.x += 0.5;
    normalizedInLocation.y += 0.5;
    normalizedInLocation.z += 0.5;

    int32_t x = ( int32_t )normalizedInLocation.x;
    int32_t y = ( int32_t )normalizedInLocation.y;
    int32_t z = ( int32_t )normalizedInLocation.z;

    if ( ( normalizedInLocation.x < 0.0 ) &&
         ( ( float )x != normalizedInLocation.x ) )
    {

      -- x;

    }
    if ( ( normalizedInLocation.y < 0.0 ) &&
         ( ( float )y != normalizedInLocation.y ) )
    {

      -- y;

    }
    if ( ( normalizedInLocation.z < 0.0 ) &&
         ( ( float )z != normalizedInLocation.z ) )
    {

      -- z;

    }

    if ( ( x >= 0 ) && ( x < inVolume.getSizeX() ) &&
         ( y >= 0 ) && ( y < inVolume.getSizeY() ) &&
         ( z >= 0 ) && ( z < inVolume.getSizeZ() ) )
    {

      std::vector< double > weightX( width ), weightY( width ), weightZ( width);
      std::vector< int32_t > foldX( width ), foldY( width ), foldZ( width );

      typename gkg::SplineResamplerTraits< T >::coefficient_type
        intensity, qi, qj;
      int32_t i, j, k;

      if ( inVolume.getSizeZ() == 1 )
      {

        weightZ[ 0 ] = 1.0;
        foldZ[ 0 ] = 0;

      }
      else
      {

        if ( order % 2 )
        {

          normalizedInLocation.z -= 0.5;
          z = ( int32_t )normalizedInLocation.z;
          if ( ( normalizedInLocation.z < 0.0 ) &&
               ( ( float )z != normalizedInLocation.z ) )
          {

            -- z;

          }
          z -= half;  

        }
        else
        {

          z -= half;
          normalizedInLocation.z -= 0.5;

        }
        for ( k = 0; k < width; k++ )
        {

          weightZ[ k ] = getBSplineWeight( z + k,
                                          ( double )normalizedInLocation.z );
          foldZ[ k ] = getFold( z + k, inVolume.getSizeZ() ) *
                       inVolume.getSizeX() * inVolume.getSizeY();

        }

      }

      if ( order % 2 )
      {

        normalizedInLocation.y -= 0.5;
        y = ( int32_t )normalizedInLocation.y;
        if ( ( normalizedInLocation.y < 0.0 ) &&
             ( ( float )y != normalizedInLocation.y ) )
        {

          -- y;

        }
        y -= half;  

      }
      else
      {

        y -= half;
        normalizedInLocation.y -= 0.5;

      }
      for ( j = 0; j < width; j++ )
      {

        weightY[ j ] = getBSplineWeight( y + j, 
                                         ( double )normalizedInLocation.y );
        foldY[ j ] = getFold( y +j, inVolume.getSizeY() ) * inVolume.getSizeX();

      }

      if ( order % 2 )
      {

        normalizedInLocation.x -= 0.5;
        x = ( int32_t )normalizedInLocation.x;
        if ( ( normalizedInLocation.x < 0.0 ) &&
             ( ( float )x != normalizedInLocation.x ) )
        {

          -- x;

        }
        x -= half;  

      }
      else
      {

        x -= half;
        normalizedInLocation.x -= 0.5;

      }
      for ( i = 0; i < width; i++ )
      {

        weightX[ i ] = getBSplineWeight( x + i,
                                         ( double )normalizedInLocation.x );
        foldX[ i ] = getFold( x + i, inVolume.getSizeX() );

      }

      intensity = 0.0;
      for ( k = 0; k < ( ( inVolume.getSizeZ() == 1 ) ? 1 : width ); k++ )
      {

        pj = s + foldZ[ k ];
        qj = 0.0;
        for ( j = 0; j < width; j++ )
        {

          pi = pj + foldY[ j ];
          qi = 0.0;
          for ( i = 0; i < width; i++ )
          {

            qi += weightX[ i ] * *( pi + foldX[ i ] );

          }
          qj += weightY[ j ] * qi;

        }
        intensity += weightZ[ k ] * qj;

      }
      outValue = convert( intensity );

    }
    else
    {

      outValue = outBackground;

    }

  }
  GKG_CATCH( "template < class T > void gkg::SplineResampler< T >::doResample( "
             "const Volume< T >& inVolume, "
             "const Transform3d< double >& transform3d, "
             "const T& outBackground, "
             "const Vector3d< double >& outLocation, "
             "T& outValue )" );

}


template < class T >
void 
gkg::SplineResampler< T >::doResample( const Volume< T >& inVolume,
                                       const Transform3d< double >& transform3d,
                                       const T& outBackground,
                                       const Vector3d< double >& outLocation,
                                       T& outValue )
{

  try
  {

    int32_t order = this->getOrder();
    int32_t half = order / 2;
    int32_t width = order + 1;

    typename gkg::Volume< typename
             gkg::SplineResamplerTraits< T >::coefficient_type >::const_iterator
      s = _splineCoefficients.begin();
    typename gkg::Volume< typename
             gkg::SplineResamplerTraits< T >::coefficient_type >::const_iterator
      pi, pj;

    gkg::Vector3d< double > normalizedInLocation;
    transform3d.getInverse( outLocation, normalizedInLocation );

    normalizedInLocation.x += 0.5;
    normalizedInLocation.y += 0.5;
    normalizedInLocation.z += 0.5;

    int32_t x = ( int32_t )normalizedInLocation.x;
    int32_t y = ( int32_t )normalizedInLocation.y;
    int32_t z = ( int32_t )normalizedInLocation.z;

    if ( ( normalizedInLocation.x < 0.0 ) &&
         ( ( double )x != normalizedInLocation.x ) )
    {

      -- x;

    }
    if ( ( normalizedInLocation.y < 0.0 ) &&
         ( ( double )y != normalizedInLocation.y ) )
    {

      -- y;

    }
    if ( ( normalizedInLocation.z < 0.0 ) &&
         ( ( double )z != normalizedInLocation.z ) )
    {

      -- z;

    }

    if ( ( x >= 0 ) && ( x < inVolume.getSizeX() ) &&
         ( y >= 0 ) && ( y < inVolume.getSizeY() ) &&
         ( z >= 0 ) && ( z < inVolume.getSizeZ() ) )
    {

      std::vector< double > weightX( width ), weightY( width ), weightZ( width);
      std::vector< int32_t > foldX( width ), foldY( width ), foldZ( width );

      typename gkg::SplineResamplerTraits< T >::coefficient_type
        intensity, qi, qj;
      int32_t i, j, k;

      if ( inVolume.getSizeZ() == 1 )
      {

        weightZ[ 0 ] = 1.0;
        foldZ[ 0 ] = 0;

      }
      else
      {

        if ( order % 2 )
        {

          normalizedInLocation.z -= 0.5;
          z = ( int32_t )normalizedInLocation.z;
          if ( ( normalizedInLocation.z < 0.0 ) &&
               ( ( double )z != normalizedInLocation.z ) )
          {

            -- z;

          }
          z -= half;  

        }
        else
        {

          z -= half;
          normalizedInLocation.z -= 0.5;

        }
        for ( k = 0; k < width; k++ )
        {

          weightZ[ k ] = getBSplineWeight( z + k, normalizedInLocation.z );
          foldZ[ k ] = getFold( z + k, inVolume.getSizeZ() ) *
                       inVolume.getSizeX() * inVolume.getSizeY();

        }

      }

      if ( order % 2 )
      {

        normalizedInLocation.y -= 0.5;
        y = ( int32_t )normalizedInLocation.y;
        if ( ( normalizedInLocation.y < 0.0 ) &&
             ( ( double )y != normalizedInLocation.y ) )
        {

          -- y;

        }
        y -= half;  

      }
      else
      {

        y -= half;
        normalizedInLocation.y -= 0.5;

      }
      for ( j = 0; j < width; j++ )
      {

        weightY[ j ] = getBSplineWeight( y + j, normalizedInLocation.y );
        foldY[ j ] = getFold( y +j, inVolume.getSizeY() ) * inVolume.getSizeX();

      }

      if ( order % 2 )
      {

        normalizedInLocation.x -= 0.5;
        x = ( int32_t )normalizedInLocation.x;
        if ( ( normalizedInLocation.x < 0.0 ) &&
             ( ( double )x != normalizedInLocation.x ) )
        {

          -- x;

        }
        x -= half;  

      }
      else
      {

        x -= half;
        normalizedInLocation.x -= 0.5;

      }
      for ( i = 0; i < width; i++ )
      {

        weightX[ i ] = getBSplineWeight( x + i, normalizedInLocation.x );
        foldX[ i ] = getFold( x + i, inVolume.getSizeX() );

      }

      intensity = 0.0;
      for ( k = 0; k < ( ( inVolume.getSizeZ() == 1 ) ? 1 : width ); k++ )
      {

        pj = s + foldZ[ k ];
        qj = 0.0;
        for ( j = 0; j < width; j++ )
        {

          pi = pj + foldY[ j ];
          qi = 0.0;
          for ( i = 0; i < width; i++ )
          {

            qi += weightX[ i ] * *( pi + foldX[ i ] );

          }
          qj += weightY[ j ] * qi;

        }
        intensity += weightZ[ k ] * qj;

      }
      outValue = convert( intensity );

    }
    else
    {

      outValue = outBackground;

    }

  }
  GKG_CATCH( "template < class T > void gkg::SplineResampler< T >::doResample( "
             "const Volume< T >& inVolume, "
             "const Transform3d< double >& transform3d, "
             "const T& outBackground, "
             "const Vector3d< double >& outLocation, "
             "T& outValue )" );

}


template < class T >
void gkg::SplineResampler< T >::updateSplineCoefficients(
                                              const gkg::Volume< T >& inVolume,
                                              bool verbose )
{

  static const gkg::Volume< T >* pointer = 0;

  if ( ( _reset || ( &inVolume != pointer ) ) && this->getOrder() > 1 )
  {

    if ( verbose )
    {

      std::cout << "[ updating spline coefficients : " << std::flush;

    }

    int32_t inSizeX = inVolume.getSizeX();
    int32_t inSizeY = inVolume.getSizeY();
    int32_t inSizeZ = inVolume.getSizeZ();

    gkg::Converter<
      gkg::Volume< T >,
      gkg::Volume< typename
                   gkg::SplineResamplerTraits< T >::coefficient_type > >
        converter;
    converter.convert( inVolume, _splineCoefficients );

    if ( inSizeX > 1 )
    {

      std::vector< typename
                   gkg::SplineResamplerTraits< T >::coefficient_type >
        data( inSizeX );
      int32_t x, y, z;
      for ( z = 0; z < inSizeZ; z++ )
      {

        for ( y = 0; y < inSizeY; y++ )
        {

          for ( x = 0; x < inSizeX; x++ )
          {

            data[ x ] = _splineCoefficients( x, y, z );

          }
          iirConvolveMirror( data );
          for ( x = 0; x < inSizeX; x++ )
          {

            _splineCoefficients( x, y, z ) = data[ x ];

          }

        }

      }

    }

    if ( inSizeY > 1 )
    {

      std::vector< typename gkg::SplineResamplerTraits< T >::coefficient_type >
        data( inSizeY );
      int32_t x, y, z;
      for ( z = 0; z < inSizeZ; z++ )
      {

        for ( x = 0; x < inSizeX; x++ )
        {

          for ( y = 0; y < inSizeY; y++ )
          {

            data[ y ] = _splineCoefficients( x, y, z );

          }
          iirConvolveMirror( data );
          for ( y = 0; y < inSizeY; y++ )
          {

            _splineCoefficients( x, y, z ) = data[ y ];

          }

        }

      }

    }

    if ( inSizeZ > 1 )
    {

      std::vector< typename gkg::SplineResamplerTraits< T >::coefficient_type >
        data( inSizeZ );
      int32_t x, y, z;
      for ( y = 0; y < inSizeY; y++ )
      {

        for ( x = 0; x < inSizeX; x++ )
        {

          for ( z = 0; z < inSizeZ; z++ )
          {

            data[ z ] = _splineCoefficients( x, y, z );

          }
          iirConvolveMirror( data );
          for ( z = 0; z < inSizeZ; z++ )
          {

            _splineCoefficients( x, y, z ) = data[ z ];

          }

        }

      }

    }

    pointer = &inVolume;
    _reset = false;    

    if ( verbose )
    {

      std::cout << "done ]" << gkg::Eraser( 39 ) << std::flush;

    }

  }

}


template < class T >
void gkg::SplineResampler< T >::iirConvolveMirror(
      std::vector< typename gkg::SplineResamplerTraits< T >::coefficient_type >&
                                                                    data ) const
{

  double tolerance = std::log10( EPSILON );

  typename std::vector< typename
               gkg::SplineResamplerTraits< T >::coefficient_type >::iterator
    d = data.begin(), de = data.end();
  while ( d != de )
  {

    *d *= _gain;
    ++ d;

  }

  int32_t size = ( int32_t )data.size();
  if ( size == 1 )
  {

    return;

  }

  int32_t size2 = 2 * ( size - 1 );

  std::vector< double >::const_reverse_iterator p = _poles.rbegin(),
                                                pe = _poles.rend();
  typename gkg::SplineResamplerTraits< T >::coefficient_type*
    dataBegin = &data[ 0 ];
  typename gkg::SplineResamplerTraits< T >::coefficient_type*
    dataEnd = dataBegin + data.size();
  typename gkg::SplineResamplerTraits< T >::coefficient_type* ptrD1;
  typename gkg::SplineResamplerTraits< T >::coefficient_type* ptrD2;

  int32_t j, k;
  typename gkg::SplineResamplerTraits< T >::coefficient_type x0;
  while ( p != pe )
  {

    j = ( int32_t )std::ceil( tolerance / std::log10( std::fabs( *p ) ) );
    k = j - size2 * ( j / size2 );
    j -= k;
    if ( k < size )
    {

      ptrD1 = dataBegin + k;
      x0 = *ptrD1;

    }
    else
    {

      k = size2 - k;
      ptrD1 = dataBegin + k;
      x0 = *ptrD1;
      while ( ++ptrD1 < dataEnd )
      {

        x0 = *p * x0 + *ptrD1;

      }
      -- ptrD1;

    }
    while ( --ptrD1 >= dataBegin )
    {

      x0 = *p * x0 + *ptrD1;

    }
    while ( j > 0 )
    {

      ++ ptrD1;
      while ( ++ptrD1 < dataEnd )
      {

        x0 = *p * x0 + *ptrD1;

      }
      -- ptrD1;
      while ( --ptrD1 >= dataBegin )
      {

        x0 = *p * x0 + *ptrD1;

      }
      j -= size2;

    }
    ptrD2 = ptrD1 ++;
    *ptrD1++ = x0;
    x0 = *( ptrD2++ + size );
    while ( ptrD1 < dataEnd )
    {

      *ptrD1++ += *ptrD2++ * *p;

    }
    *ptrD2 = ( 2.0 * *ptrD2 - x0 ) /
             ( 1.0 - gkg::SplineResamplerTraits< T >::norm2( *ptrD1 ) );
    -- ptrD1;
    while ( --ptrD2 >= dataBegin )
    {

      *ptrD2 += *ptrD1-- * *p;

    }

    ++ p ;

  }

}


template < class T >
int32_t gkg::SplineResampler< T >::getFold( int32_t i, int32_t size ) const
{

  i = std::abs( i );
  if ( i < size )
  {

    return i;

  }
  if ( size == 1 )
  {

    return 0;

  }
  int32_t size2 = ( size << 1 ) - 2;
  ldiv_t modOp = std::ldiv( i, size2 );
  return ( modOp.rem < size ) ? modOp.rem : ( size2 - modOp.rem );

}


template < class T >
T gkg::SplineResampler< T >::convert(
   typename gkg::SplineResamplerTraits< T >::coefficient_type& intensity ) const
{

/*
  if ( intensity >
       ( typename gkg::SplineResamplerTraits< T >::coefficient_type )
                                               std::numeric_limits< T >::max() )
  {

    return std::numeric_limits< T >::max();

  }
  if ( intensity < 
       ( typename gkg::SplineResamplerTraits< T >::coefficient_type )
                                            std::numeric_limits< T >::lowest() )
  {

    return std::numeric_limits< T >::lowest();

  }
*/
  return ( T )intensity;

}


namespace gkg
{


template <>
std::complex< float > gkg::SplineResampler< std::complex< float > >::convert(
          gkg::SplineResamplerTraits< std::complex< float > >::coefficient_type&
            intensity ) const
{

  return std::complex< float >( intensity );

}


template <>
std::complex< double > gkg::SplineResampler< std::complex< double > >::convert(
         gkg::SplineResamplerTraits< std::complex< double > >::coefficient_type&
           intensity ) const
{

  return intensity;

}


}


template class gkg::SplineResampler< int8_t >;
template class gkg::SplineResampler< uint8_t >;
template class gkg::SplineResampler< int16_t >;
template class gkg::SplineResampler< uint16_t >;
template class gkg::SplineResampler< int32_t >;
template class gkg::SplineResampler< uint32_t >;
template class gkg::SplineResampler< int64_t >;
template class gkg::SplineResampler< uint64_t >;
template class gkg::SplineResampler< float  >;
template class gkg::SplineResampler< double >;
template class gkg::SplineResampler< std::complex< float > >;
template class gkg::SplineResampler< std::complex< double > >;
template class gkg::SplineResampler< gkg::RGBComponent >;


#undef EPSILON

