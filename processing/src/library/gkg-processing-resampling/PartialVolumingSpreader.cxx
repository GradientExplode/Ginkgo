#include <gkg-processing-resampling/PartialVolumingSpreader.h>
#include <gkg-processing-resampling/SpreaderFactory.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>
#include <cmath>


namespace std
{

inline complex< float >
operator*( const double& x, const complex< float >& y )
{ return y * ( float )x; }

}

template < class T >
gkg::PartialVolumingSpreader< T >::PartialVolumingSpreader()
                                  : gkg::Spreader< T >()
{
}


template < class T >
gkg::PartialVolumingSpreader< T >::~PartialVolumingSpreader()
{
}


template < class T >
std::string gkg::PartialVolumingSpreader< T >::getName() const
{

  return "Partial Voluming Spreader";

}


inline
static bool isOk( int32_t x, int32_t y, int32_t z, 
                  int32_t sizeX, int32_t sizeY, int32_t sizeZ )
{

  return ( ( x >= 0 ) &&
           ( y >= 0 ) &&
           ( z >= 0 ) &&
           ( x < sizeX ) &&
           ( y < sizeY ) &&
           ( z < sizeZ ) );

}


template < class T >
void gkg::PartialVolumingSpreader< T >::spread(
                               const T& value,
                               const gkg::Vector3d< float >& outLocation,
                               gkg::Volume< T >& outVolume,
                               const gkg::Vector3d< double >* resolution,
                               int32_t rank ) const
{

  try
  {

    // getting output resolution
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( !resolution )
    {

      if ( outVolume.getHeader().hasAttribute( "resolutionX" ) )
      {

        outVolume.getHeader().getAttribute( "resolutionX", resolutionX );

      }
      if ( outVolume.getHeader().hasAttribute( "resolutionY" ) )
      {

        outVolume.getHeader().getAttribute( "resolutionY", resolutionY );

      }
      if ( outVolume.getHeader().hasAttribute( "resolutionZ" ) )
      {

        outVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

      }

    }
    else
    {

      resolutionX = resolution->x;
      resolutionY = resolution->y;
      resolutionZ = resolution->z;

    }

    gkg::Vector3d< double > normalizedOutLocation( ( double )outLocation.x,
                                                   ( double )outLocation.y,
                                                   ( double )outLocation.z );
    normalizedOutLocation.x /= resolutionX;
    normalizedOutLocation.y /= resolutionY;
    normalizedOutLocation.z /= resolutionZ;

    // getting inferior point of the bounding box
    int32_t x = ( int32_t )normalizedOutLocation.x;
    int32_t y = ( int32_t )normalizedOutLocation.y;
    int32_t z = ( int32_t )normalizedOutLocation.z;

    // calculating partial volume(s)
    double f1x = ( normalizedOutLocation.x - ( double )x );
    double f1y = ( normalizedOutLocation.y - ( double )y );
    double f1z = ( normalizedOutLocation.z - ( double )z );

    double if1x = ( 1.0 - f1x );
    double if1y = ( 1.0 - f1y );
    double if1z = ( 1.0 - f1z );

    int32_t sizeX = outVolume.getSizeX();
    int32_t sizeY = outVolume.getSizeY();
    int32_t sizeZ = outVolume.getSizeZ();

    if ( isOk( x, y, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y, z, rank ) += ( T )( if1x * if1y * if1z * value );

    }
    if ( isOk( x + 1, y, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y, z, rank ) += ( T )( f1x * if1y * if1z * value );

    }
    if ( isOk( x, y + 1, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y + 1, z, rank ) += ( T )( if1x * f1y * if1z * value );

    }
    if ( isOk( x, y, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y, z + 1, rank ) += ( T )( if1x * if1y * f1z * value );

    }
    if ( isOk( x + 1, y + 1, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y + 1, z, rank ) += ( T )( f1x * f1y * if1z * value );

    }
    if ( isOk( x + 1, y, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y, z + 1, rank ) += ( T )( f1x * if1y * f1z * value );

    }
    if ( isOk( x, y + 1, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y + 1, z + 1, rank ) += ( T )( if1x * f1y * f1z * value );

    }
    if ( isOk( x + 1, y + 1, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y + 1, z + 1, rank ) += ( T )( f1x * f1y * f1z * value);

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PartialVolumingSpreader< T >::spread( "
             "const T& value, "
             "const gkg::Vector3d< float >& outLocation, "
             "gkg::Volume< T >& outVolume, "
             "const gkg::Vector3d< double >* resolution, "
             "int rank ) const" );

}


template < class T >
void gkg::PartialVolumingSpreader< T >::spread(
                               const T& value,
                               const gkg::Vector3d< double >& outLocation,
                               gkg::Volume< T >& outVolume,
                               const gkg::Vector3d< double >* resolution,
                               int32_t rank ) const
{

  try
  {

    // getting output resolution
    double resolutionX = 1.0;
    double resolutionY = 1.0;
    double resolutionZ = 1.0;
    if ( !resolution )
    {

      if ( outVolume.getHeader().hasAttribute( "resolutionX" ) )
      {

        outVolume.getHeader().getAttribute( "resolutionX", resolutionX );

      }
      if ( outVolume.getHeader().hasAttribute( "resolutionY" ) )
      {

        outVolume.getHeader().getAttribute( "resolutionY", resolutionY );

      }
      if ( outVolume.getHeader().hasAttribute( "resolutionZ" ) )
      {

        outVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

      }

    }
    else
    {

      resolutionX = resolution->x;
      resolutionY = resolution->y;
      resolutionZ = resolution->z;

    }

    gkg::Vector3d< double > normalizedOutLocation( outLocation );
    normalizedOutLocation.x /= resolutionX;
    normalizedOutLocation.y /= resolutionY;
    normalizedOutLocation.z /= resolutionZ;

    // getting inferior point of the bounding box
    int32_t x = ( int32_t )normalizedOutLocation.x;
    int32_t y = ( int32_t )normalizedOutLocation.y;
    int32_t z = ( int32_t )normalizedOutLocation.z;

    // calculating partial volume(s)
    double f1x = ( normalizedOutLocation.x - ( double )x );
    double f1y = ( normalizedOutLocation.y - ( double )y );
    double f1z = ( normalizedOutLocation.z - ( double )z );

    double if1x = ( 1.0 - f1x );
    double if1y = ( 1.0 - f1y );
    double if1z = ( 1.0 - f1z );

    int32_t sizeX = outVolume.getSizeX();
    int32_t sizeY = outVolume.getSizeY();
    int32_t sizeZ = outVolume.getSizeZ();

    if ( isOk( x, y, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y, z, rank ) += ( T )( if1x * if1y * if1z * value );

    }
    if ( isOk( x + 1, y, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y, z, rank ) += ( T )( f1x * if1y * if1z * value );

    }
    if ( isOk( x, y + 1, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y + 1, z, rank ) += ( T )( if1x * f1y * if1z * value );

    }
    if ( isOk( x, y, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y, z + 1, rank ) += ( T )( if1x * if1y * f1z * value );

    }
    if ( isOk( x + 1, y + 1, z, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y + 1, z, rank ) += ( T )( f1x * f1y * if1z * value );

    }
    if ( isOk( x + 1, y, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y, z + 1, rank ) += ( T )( f1x * if1y * f1z * value );

    }
    if ( isOk( x, y + 1, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x, y + 1, z + 1, rank ) += ( T )( if1x * f1y * f1z * value );

    }
    if ( isOk( x + 1, y + 1, z + 1, sizeX, sizeY, sizeZ ) )
    {

      outVolume( x + 1, y + 1, z + 1, rank ) += ( T )( f1x * f1y * f1z * value);

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PartialVolumingSpreader< T >::spread( "
             "const T& value, "
             "const gkg::Vector3d< double >& outLocation, "
             "gkg::Volume< T >& outVolume, "
             "const gkg::Vector3d< double >* resolution, "
             "int rank ) const" );

}


template class gkg::PartialVolumingSpreader< int8_t >;
template class gkg::PartialVolumingSpreader< uint8_t >;
template class gkg::PartialVolumingSpreader< int16_t >;
template class gkg::PartialVolumingSpreader< uint16_t >;
template class gkg::PartialVolumingSpreader< int32_t >;
template class gkg::PartialVolumingSpreader< uint32_t >;
template class gkg::PartialVolumingSpreader< int64_t >;
template class gkg::PartialVolumingSpreader< uint64_t >;
template class gkg::PartialVolumingSpreader< float  >;
template class gkg::PartialVolumingSpreader< double >;
template class gkg::PartialVolumingSpreader< std::complex< float > >;
template class gkg::PartialVolumingSpreader< std::complex< double > >;
template class gkg::PartialVolumingSpreader< gkg::RGBComponent >;


RegisterSpreader( gkg, PartialVolumingSpreader );
