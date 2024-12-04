#include <gkg-processing-resampling/NearestNeighborResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::NearestNeighborResampler< T >::NearestNeighborResampler()
                                   : gkg::Resampler< T >()
{
}


template < class T >
gkg::NearestNeighborResampler< T >::~NearestNeighborResampler()
{
}


template < class T >
std::string gkg::NearestNeighborResampler< T >::getName() const
{

  return "Nearest Neighbor Resampler";

}


template < class T >
int32_t gkg::NearestNeighborResampler< T >::getOrder() const
{

  return 0;

}


template < class T >
void gkg::NearestNeighborResampler< T >::doResample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< float >& transform3d,
                                  const T& outBackground,
                                  const gkg::Vector3d< float >& outLocation,
                                  T& outValue )
{

  typename gkg::Volume< T >::const_iterator i = inVolume.begin();

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

    outValue = *( i + z * inVolume.getSizeX() * inVolume.getSizeY() +
                      y * inVolume.getSizeX() + x );

  }
  else
  {

    outValue = outBackground;

  }

}


template < class T >
void gkg::NearestNeighborResampler< T >::doResample(
                                  const gkg::Volume< T >& inVolume,
                                  const gkg::Transform3d< double >& transform3d,
                                  const T& outBackground,
                                  const gkg::Vector3d< double >& outLocation,
                                  T& outValue )
{

  typename gkg::Volume< T >::const_iterator i = inVolume.begin();

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

    outValue = *( i + z * inVolume.getSizeX() * inVolume.getSizeY() +
                      y * inVolume.getSizeX() + x );

  }
  else
  {

    outValue = outBackground;

  }

}


template class gkg::NearestNeighborResampler< int8_t >;
template class gkg::NearestNeighborResampler< uint8_t >;
template class gkg::NearestNeighborResampler< int16_t >;
template class gkg::NearestNeighborResampler< uint16_t >;
template class gkg::NearestNeighborResampler< int32_t >;
template class gkg::NearestNeighborResampler< uint32_t >;
template class gkg::NearestNeighborResampler< int64_t >;
template class gkg::NearestNeighborResampler< uint64_t >;
template class gkg::NearestNeighborResampler< float >;
template class gkg::NearestNeighborResampler< double >;
template class gkg::NearestNeighborResampler< std::complex< float > >;
template class gkg::NearestNeighborResampler< std::complex< double > >;
template class gkg::NearestNeighborResampler< gkg::RGBComponent >;


RegisterResampler( gkg, NearestNeighborResampler );

