#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
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
gkg::PartialVolumingQuickResampler< T >::PartialVolumingQuickResampler()
                                        : gkg::QuickResampler< T >()
{
}


template < class T >
gkg::PartialVolumingQuickResampler< T >::~PartialVolumingQuickResampler()
{
}


template < class T >
std::string gkg::PartialVolumingQuickResampler< T >::getName() const
{

  return "Partial Voluming QuickResampler";

}


template < class T >
void gkg::PartialVolumingQuickResampler< T >::resample(
                                     const gkg::Volume< T >& inVolume,
                                     const T& outBackground,
                                     const gkg::Vector3d< float >& outLocation,
                                     T& outValue,
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

      if ( inVolume.getHeader().hasAttribute( "resolutionX" ) )
      {

        inVolume.getHeader().getAttribute( "resolutionX", resolutionX );

      }
      if ( inVolume.getHeader().hasAttribute( "resolutionY" ) )
      {

        inVolume.getHeader().getAttribute( "resolutionY", resolutionY );

      }
      if ( inVolume.getHeader().hasAttribute( "resolutionZ" ) )
      {

        inVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

      }

    }
    else
    {

      resolutionX = resolution->x;
      resolutionY = resolution->y;
      resolutionZ = resolution->z;

    }

    gkg::Vector3d< double > normalizedOutLocation(
                              ( double )outLocation.x,
                              ( double )outLocation.y,
                              ( double )outLocation.z );
    normalizedOutLocation.x /= resolutionX;
    normalizedOutLocation.y /= resolutionY;
    normalizedOutLocation.z /= resolutionZ;

    if ( ( normalizedOutLocation.x < 0 ) ||
         ( normalizedOutLocation.y < 0 ) ||
         ( normalizedOutLocation.z < 0 ) ||
         ( normalizedOutLocation.x > ( double )( inVolume.getSizeX() - 1 ) ) ||
         ( normalizedOutLocation.y > ( double )( inVolume.getSizeY() - 1 ) ) ||
         ( normalizedOutLocation.z > ( double )( inVolume.getSizeZ() - 1 ) ) )
    {

      outValue = outBackground;

    }
    else
    {

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
/*
      std::cout << "outLocation=" << outLocation << std::endl;
      std::cout << "resolution=" << *resolution << std::endl;
      std::cout << "normalizedOutLocation=" << normalizedOutLocation << std::endl;
      std::cout << "f1x, if1x =" << f1x << ", " << if1x << std::endl;
      std::cout << "f1y, if1y =" << f1y << ", " << if1y << std::endl;
      std::cout << "f1z, if1z =" << f1z << ", " << if1z << std::endl;
*/
      outValue = ( T )( if1x * if1y * if1z * inVolume( x, y, z, rank ) );
      if ( x + 1 < inVolume.getSizeX() )
      {

        outValue += ( T )( f1x * if1y * if1z *
                           inVolume( x + 1, y, z, rank ) );

      }
      if ( y + 1 < inVolume.getSizeY() )
      {

        outValue += ( T )( if1x * f1y * if1z *
                           inVolume( x, y + 1, z, rank ) );

      }
      if ( z + 1 < inVolume.getSizeZ() )
      {

        outValue += ( T )( if1x * if1y * f1z *
                           inVolume( x, y, z + 1, rank ) );

      }
      if ( ( x + 1 < inVolume.getSizeX() ) &&
           ( y + 1 < inVolume.getSizeY() ) )
      {

        outValue += ( T )( f1x * f1y * if1z *
                           inVolume( x + 1, y + 1, z, rank ) );

      }
      if ( ( x + 1 < inVolume.getSizeX() ) &&
           ( z + 1 < inVolume.getSizeZ() ) )
      {

        outValue += ( T )( f1x * if1y * f1z *
                           inVolume( x + 1, y, z + 1, rank ) );

      }
      if ( ( y + 1 < inVolume.getSizeY() ) &&
           ( z + 1 < inVolume.getSizeZ() ) )
      {

        outValue += ( T )( if1x * f1y * f1z *
                           inVolume( x, y + 1, z + 1, rank ) );

      }
      if ( ( x + 1 < inVolume.getSizeX() ) &&
           ( y + 1 < inVolume.getSizeY() ) &&
           ( z + 1 < inVolume.getSizeZ() ) )
      {

        outValue += ( T )( f1x * f1y * f1z *
                           inVolume( x + 1, y + 1, z + 1, rank ) );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PartialVolumingQuickResampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const T& outBackground, "
             "const gkg::Vector3d< float >& outLocation, "
             "T& outValue, "
             "const gkg::Vector3d< double >* resolution, "
             "int rank ) const" );

}



template < class T >
void gkg::PartialVolumingQuickResampler< T >::resample(
                                     const gkg::Volume< T >& inVolume,
                                     const T& outBackground,
                                     const gkg::Vector3d< double >& outLocation,
                                     T& outValue,
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

      if ( inVolume.getHeader().hasAttribute( "resolutionX" ) )
      {

        inVolume.getHeader().getAttribute( "resolutionX", resolutionX );

      }
      if ( inVolume.getHeader().hasAttribute( "resolutionY" ) )
      {

        inVolume.getHeader().getAttribute( "resolutionY", resolutionY );

      }
      if ( inVolume.getHeader().hasAttribute( "resolutionZ" ) )
      {

        inVolume.getHeader().getAttribute( "resolutionZ", resolutionZ );

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

    if ( ( normalizedOutLocation.x < 0 ) ||
         ( normalizedOutLocation.y < 0 ) ||
         ( normalizedOutLocation.z < 0 ) ||
         ( normalizedOutLocation.x > ( double )( inVolume.getSizeX() - 1 ) ) ||
         ( normalizedOutLocation.y > ( double )( inVolume.getSizeY() - 1 ) ) ||
         ( normalizedOutLocation.z > ( double )( inVolume.getSizeZ() - 1 ) ) )
    {

      outValue = outBackground;

    }
    else
    {

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
/*
      std::cout << "outLocation=" << outLocation << std::endl;
      std::cout << "resolution=" << *resolution << std::endl;
      std::cout << "normalizedOutLocation=" << normalizedOutLocation << std::endl;
      std::cout << "f1x, if1x =" << f1x << ", " << if1x << std::endl;
      std::cout << "f1y, if1y =" << f1y << ", " << if1y << std::endl;
      std::cout << "f1z, if1z =" << f1z << ", " << if1z << std::endl;
*/
      outValue = ( T )( if1x * if1y * if1z * inVolume( x, y, z, rank ) );
      if ( x + 1 < inVolume.getSizeX() )
      {

        outValue += ( T )( f1x * if1y * if1z *
                           inVolume( x + 1, y, z, rank ) );

      }
      if ( y + 1 < inVolume.getSizeY() )
      {

        outValue += ( T )( if1x * f1y * if1z *
                           inVolume( x, y + 1, z, rank ) );

      }
      if ( z + 1 < inVolume.getSizeZ() )
      {

        outValue += ( T )( if1x * if1y * f1z *
                           inVolume( x, y, z + 1, rank ) );

      }
      if ( ( x + 1 < inVolume.getSizeX() ) &&
           ( y + 1 < inVolume.getSizeY() ) )
      {

        outValue += ( T )( f1x * f1y * if1z *
                           inVolume( x + 1, y + 1, z, rank ) );

      }
      if ( ( x + 1 < inVolume.getSizeX() ) &&
           ( z + 1 < inVolume.getSizeZ() ) )
      {

        outValue += ( T )( f1x * if1y * f1z *
                           inVolume( x + 1, y, z + 1, rank ) );

      }
      if ( ( y + 1 < inVolume.getSizeY() ) &&
           ( z + 1 < inVolume.getSizeZ() ) )
      {

        outValue += ( T )( if1x * f1y * f1z *
                           inVolume( x, y + 1, z + 1, rank ) );

      }
      if ( ( x + 1 < inVolume.getSizeX() ) &&
           ( y + 1 < inVolume.getSizeY() ) &&
           ( z + 1 < inVolume.getSizeZ() ) )
      {

        outValue += ( T )( f1x * f1y * f1z *
                           inVolume( x + 1, y + 1, z + 1, rank ) );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::PartialVolumingQuickResampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const T& outBackground, "
             "const gkg::Vector3d< double >& outLocation, "
             "T& outValue, "
             "const gkg::Vector3d< double >* resolution, "
             "int rank ) const" );

}



template class gkg::PartialVolumingQuickResampler< int8_t >;
template class gkg::PartialVolumingQuickResampler< uint8_t >;
template class gkg::PartialVolumingQuickResampler< int16_t >;
template class gkg::PartialVolumingQuickResampler< uint16_t >;
template class gkg::PartialVolumingQuickResampler< int32_t >;
template class gkg::PartialVolumingQuickResampler< uint32_t >;
template class gkg::PartialVolumingQuickResampler< int64_t >;
template class gkg::PartialVolumingQuickResampler< uint64_t >;
template class gkg::PartialVolumingQuickResampler< float  >;
template class gkg::PartialVolumingQuickResampler< double >;
template class gkg::PartialVolumingQuickResampler< std::complex< float > >;
template class gkg::PartialVolumingQuickResampler< std::complex< double > >;
template class gkg::PartialVolumingQuickResampler< gkg::RGBComponent >;


RegisterQuickResampler( gkg, PartialVolumingQuickResampler );
