#include <gkg-processing-resampling/LinearResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::LinearResampler< T >::LinearResampler()
                          : gkg::SplineResampler< T >()
{
}


template < class T >
gkg::LinearResampler< T >::~LinearResampler()
{
}


template < class T >
std::string gkg::LinearResampler< T >::getName() const
{

  return "Linear Resampler";

}


template < class T >
int32_t gkg::LinearResampler< T >::getOrder() const
{

  return 1;

}


template < class T >
void gkg::LinearResampler< T >::doResample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< float >& transform3d,
                                  const T& outBackground,
                                  const gkg::Vector3d< float >& outLocation,
                                  T& outValue )
{

  typename gkg::Volume< T >::const_iterator i = inVolume.begin();
  typename gkg::Volume< T >::const_iterator pi, pj;

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

    double weightX0, weightY0, weightX1, weightY1;
    int32_t foldX0, foldY0, foldX1, foldY1;
    typename gkg::SplineResamplerTraits< T >::coefficient_type
      intensity, qi, qj;

    // first y contribution
    normalizedInLocation.y -= 0.5;
    y = ( int32_t )normalizedInLocation.y;
    if ( ( normalizedInLocation.y < 0.0 ) &&
         ( ( float )y != normalizedInLocation.y ) )
    {

      -- y;

    }
    weightY0 = getBSplineWeight( y, ( double )normalizedInLocation.y );
    foldY0 = this->getFold( y, inVolume.getSizeY() ) * inVolume.getSizeX();

    // second y contribution
    ++ y;
    weightY1 = getBSplineWeight( y, ( double )normalizedInLocation.y );
    foldY1 = this->getFold( y, inVolume.getSizeY() ) * inVolume.getSizeX();

    // first x contribution
    normalizedInLocation.x -= 0.5;
    x = ( int32_t )normalizedInLocation.x;
    if ( ( normalizedInLocation.x < 0.0 ) &&
         ( ( float )x != normalizedInLocation.x ) )
    {

      -- x;

    }
    weightX0 = getBSplineWeight( x, normalizedInLocation.x );
    foldX0 = this->getFold( x, inVolume.getSizeX() );

    // second x contribution
    ++ x;
    weightX1 = getBSplineWeight( x, normalizedInLocation.x );
    foldX1 = this->getFold( x, inVolume.getSizeX() );

    if ( inVolume.getSizeZ() == 1 )
    {

      //summing contributions
      pj = i;
      pi = pj + foldY0;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj = weightY0 * qi;
      pi = pj + foldY1;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      intensity = qj + weightY1 * qi;

    }
    else
    {

      // first z contribution
      normalizedInLocation.z -= 0.5;
      z = ( int32_t )normalizedInLocation.z;
      if ( ( normalizedInLocation.z < 0.0 ) &&
           ( ( float )z != normalizedInLocation.z ) )
      {

        -- z;

      }
      pj = i + this->getFold( z, inVolume.getSizeZ() ) * inVolume.getSizeX() *
           inVolume.getSizeY();
      pi = pj + foldY0;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj = weightY0 * qi;
      pi = pj + foldY1;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj += weightY1 * qi;
      intensity = getBSplineWeight( z, normalizedInLocation.z ) * qj;

      // first z contribution
      ++ z;
      pj = i + this->getFold( z, inVolume.getSizeZ() ) * inVolume.getSizeX() *
           inVolume.getSizeY();
      pi = pj + foldY0;
      qi = weightX0 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj = weightY0 * qi;
      pi = pj + foldY1;
      qi = weightX0 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj += weightY1 * qi;
      intensity += getBSplineWeight( z, normalizedInLocation.z ) * qj;
    }
    outValue = ( T )intensity;

  }
  else
  {

    outValue = outBackground;

  }

}


template < class T >
void gkg::LinearResampler< T >::doResample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< double >& transform3d,
                                  const T& outBackground,
                                  const gkg::Vector3d< double >& outLocation,
                                  T& outValue )
{

  typename gkg::Volume< T >::const_iterator i = inVolume.begin();
  typename gkg::Volume< T >::const_iterator pi, pj;

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

    double weightX0, weightY0, weightX1, weightY1;
    int32_t foldX0, foldY0, foldX1, foldY1;
    typename gkg::SplineResamplerTraits< T >::coefficient_type
      intensity, qi, qj;

    // first y contribution
    normalizedInLocation.y -= 0.5;
    y = ( int32_t )normalizedInLocation.y;
    if ( ( normalizedInLocation.y < 0.0 ) &&
         ( ( double )y != normalizedInLocation.y ) )
    {

      -- y;

    }
    weightY0 = getBSplineWeight( y, normalizedInLocation.y );
    foldY0 = this->getFold( y, inVolume.getSizeY() ) * inVolume.getSizeX();

    // second y contribution
    ++ y;
    weightY1 = getBSplineWeight( y, normalizedInLocation.y );
    foldY1 = this->getFold( y, inVolume.getSizeY() ) * inVolume.getSizeX();

    // first x contribution
    normalizedInLocation.x -= 0.5;
    x = ( int32_t )normalizedInLocation.x;
    if ( ( normalizedInLocation.x < 0.0 ) &&
         ( ( double )x != normalizedInLocation.x ) )
    {

      -- x;

    }
    weightX0 = getBSplineWeight( x, normalizedInLocation.x );
    foldX0 = this->getFold( x, inVolume.getSizeX() );

    // second x contribution
    ++ x;
    weightX1 = getBSplineWeight( x, normalizedInLocation.x );
    foldX1 = this->getFold( x, inVolume.getSizeX() );

    if ( inVolume.getSizeZ() == 1 )
    {

      //summing contributions
      pj = i;
      pi = pj + foldY0;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj = weightY0 * qi;
      pi = pj + foldY1;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      intensity = qj + weightY1 * qi;

    }
    else
    {

      // first z contribution
      normalizedInLocation.z -= 0.5;
      z = ( int32_t )normalizedInLocation.z;
      if ( ( normalizedInLocation.z < 0.0 ) &&
           ( ( double )z != normalizedInLocation.z ) )
      {

        -- z;

      }
      pj = i + this->getFold( z, inVolume.getSizeZ() ) * inVolume.getSizeX() *
           inVolume.getSizeY();
      pi = pj + foldY0;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj = weightY0 * qi;
      pi = pj + foldY1;
      qi = weightX0 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 * ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj += weightY1 * qi;
      intensity = getBSplineWeight( z, normalizedInLocation.z ) * qj;

      // first z contribution
      ++ z;
      pj = i + this->getFold( z, inVolume.getSizeZ() ) * inVolume.getSizeX() *
           inVolume.getSizeY();
      pi = pj + foldY0;
      qi = weightX0 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj = weightY0 * qi;
      pi = pj + foldY1;
      qi = weightX0 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX0 );
      qi += weightX1 *  ( typename
           gkg::SplineResamplerTraits< T >::coefficient_type )*( pi + foldX1 );
      qj += weightY1 * qi;
      intensity += getBSplineWeight( z, normalizedInLocation.z ) * qj;
    }
    outValue = ( T )intensity;

  }
  else
  {

    outValue = outBackground;

  }

}


template < class T >
double gkg::LinearResampler< T >::getBSplineWeight( int32_t i, double x ) const
{

  x = std::fabs( x - ( double )i );
  return ( x > 1.0 ) ? 0.0 : 1.0 - x;

}


template class gkg::LinearResampler< int8_t >;
template class gkg::LinearResampler< uint8_t >;
template class gkg::LinearResampler< int16_t >;
template class gkg::LinearResampler< uint16_t >;
template class gkg::LinearResampler< int32_t >;
template class gkg::LinearResampler< uint32_t >;
template class gkg::LinearResampler< int64_t >;
template class gkg::LinearResampler< uint64_t >;
template class gkg::LinearResampler< float >;
template class gkg::LinearResampler< double >;
template class gkg::LinearResampler< std::complex< float > >;
template class gkg::LinearResampler< std::complex< double > >;
template class gkg::LinearResampler< gkg::RGBComponent >;


RegisterResampler( gkg, LinearResampler );
