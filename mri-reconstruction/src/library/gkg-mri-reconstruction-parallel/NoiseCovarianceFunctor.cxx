#include <gkg-mri-reconstruction-parallel/NoiseCovarianceFunctor.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>




template < class T, class M >
gkg::NoiseCovarianceFunctor< T, M >::NoiseCovarianceFunctor()
{
}


template < class T, class M >
gkg::NoiseCovarianceFunctor< T, M >::~NoiseCovarianceFunctor()
{
}


template < class T, class M >
void gkg::NoiseCovarianceFunctor< T, M >::getCovariance(
                                     const gkg::Volume< T >& coilImages,
                                     const gkg::Volume< M >& noiseMask,
                                     const gkg::TestFunction< M >& testFunction,
                                     gkg::Matrix& noiseCovariance,
                                     bool verbose ) const
{

  try
  {

    // sanity checks
    if ( coilImages.getSizeT() < 2 )
    {

      throw std::runtime_error(
                              "coil images must have a T size greater than 1" );

    }
    if ( noiseMask.getSizeT() != 1 )
    {

      throw std::runtime_error(
                                   "noise mask must have a T size equal to 1" );

    }

    // collecting size(s)
    int32_t sizeX = coilImages.getSizeX();
    int32_t sizeY = coilImages.getSizeY();
    int32_t sizeZ = coilImages.getSizeZ();
    int32_t coilCount = coilImages.getSizeT();


    // processing normalizing factors
    if ( verbose )
    {

      std::cout << "normalizing factors ->" << std::flush;

    }
    gkg::Vector factors( coilCount );
    factors.fill( 0.0 );
    int32_t x, y, z, c;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }
 
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( testFunction.getState( noiseMask( x, y, z ) ) )
          {

            for ( c = 0; c < coilCount; c++ )
            {

              factors( c ) += coilImages( x, y, z, c ) *
                              coilImages( x, y, z, c );

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 42 ) << std::flush;

    }

    if ( verbose )
    {

      std::cout << "calculating variances ->" << std::flush;

    }
    noiseCovariance = gkg::Matrix( coilCount, coilCount );
    noiseCovariance.fill( 0.0 );
    int32_t c1, c2;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }
 
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( testFunction.getState( noiseMask( x, y, z ) ) )
          {

            for ( c1 = 0; c1 < coilCount; c1++ )
            {

              for ( c2 = 0; c2 < coilCount; c2++ )
              {

                noiseCovariance( c1, c2 ) += coilImages( x, y, z, c1 ) *
                                             coilImages( x, y, z, c2 );

              }

            }

          }

        }

      }

    }
    for ( c1 = 0; c1 < coilCount; c1++ )
    {

      for ( c2 = 0; c2 < coilCount; c2++ )
      {

        noiseCovariance( c1, c2 ) /= std::sqrt( factors( c1 ) * factors( c2 ) );

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 45 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T, class M > "
             "void gkg::NoiseCovarianceFunctor::getCovariance( "
             "const gkg::Volume< T >& coilImages, "
             "const gkg::Volume< M >& noiseMask, "
             "const gkg::TestFunction< M >& testFunction, "
             "gkg::Matrix& noiseCovariance, "
             "bool verbose" );

}


template < class T, class M >
void gkg::NoiseCovarianceFunctor< T, M >::getCovariance(
                             const gkg::Volume< std::complex< T > >& coilImages,
                             const gkg::Volume< M >& noiseMask,
                             const gkg::TestFunction< M >& testFunction,
                             gkg::CMatrix& noiseCovariance,
                             bool verbose ) const
{

  try
  {

    // sanity checks
    if ( coilImages.getSizeT() < 2 )
    {

      throw std::runtime_error(
                              "coil images must have a T size greater than 1" );

    }
    if ( noiseMask.getSizeT() != 1 )
    {

      throw std::runtime_error(
                                   "noise mask must have a T size equal to 1" );

    }

    // collecting size(s)
    int32_t sizeX = coilImages.getSizeX();
    int32_t sizeY = coilImages.getSizeY();
    int32_t sizeZ = coilImages.getSizeZ();
    int32_t coilCount = coilImages.getSizeT();


    // processing normalizing factors
    if ( verbose )
    {

      std::cout << "normalizing factors ->" << std::flush;

    }
    gkg::Vector factors( coilCount );
    factors.fill( 0.0 );
    int32_t x, y, z, c;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }
 
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( testFunction.getState( noiseMask( x, y, z ) ) )
          {

            for ( c = 0; c < coilCount; c++ )
            {

              factors( c ) += std::norm( coilImages( x, y, z, c ) );

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 43 ) << std::flush;

    }

    if ( verbose )
    {

      std::cout << "calculating variances ->" << std::flush;

    }
    noiseCovariance = gkg::CMatrix( coilCount, coilCount );
    noiseCovariance.fill( std::complex< double >( 0.0, 0.0 ) );
    int32_t c1, c2;
    for ( z = 0; z < sizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << sizeZ
                  << " ]" << std::flush;

      }
 
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( testFunction.getState( noiseMask( x, y, z ) ) )
          {

            for ( c1 = 0; c1 < coilCount; c1++ )
            {

              for ( c2 = 0; c2 < coilCount; c2++ )
              {

                noiseCovariance( c1, c2 ) += coilImages( x, y, z, c1 ) *
                                         std::conj( coilImages( x, y, z, c2 ) );

              }

            }

          }

        }

      }

    }
    for ( c1 = 0; c1 < coilCount; c1++ )
    {

      for ( c2 = 0; c2 < coilCount; c2++ )
      {

        noiseCovariance( c1, c2 ) /= std::sqrt( factors( c1 ) * factors( c2 ) );

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 45 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T, class M > "
             "void gkg::NoiseCovarianceFunctor::getCovariance( "
             "const gkg::Volume< std::complex< T > >& coilImages, "
             "const gkg::Volume< M >& noiseMask, "
             "const gkg::TestFunction< M >& testFunction, "
             "gkg::CMatrix& noiseCovariance, "
             "bool verbose" );

}


template class gkg::NoiseCovarianceFunctor< float, int8_t >;
template class gkg::NoiseCovarianceFunctor< float, uint8_t >;
template class gkg::NoiseCovarianceFunctor< float, int16_t >;
template class gkg::NoiseCovarianceFunctor< float, uint16_t >;
template class gkg::NoiseCovarianceFunctor< float, int32_t >;
template class gkg::NoiseCovarianceFunctor< float, uint32_t >;
template class gkg::NoiseCovarianceFunctor< float, int64_t >;
template class gkg::NoiseCovarianceFunctor< float, uint64_t >;
template class gkg::NoiseCovarianceFunctor< float, float >;
template class gkg::NoiseCovarianceFunctor< float, double >;


template class gkg::NoiseCovarianceFunctor< double, int8_t >;
template class gkg::NoiseCovarianceFunctor< double, uint8_t >;
template class gkg::NoiseCovarianceFunctor< double, int16_t >;
template class gkg::NoiseCovarianceFunctor< double, uint16_t >;
template class gkg::NoiseCovarianceFunctor< double, int32_t >;
template class gkg::NoiseCovarianceFunctor< double, uint32_t >;
template class gkg::NoiseCovarianceFunctor< double, int64_t >;
template class gkg::NoiseCovarianceFunctor< double, uint64_t >;
template class gkg::NoiseCovarianceFunctor< double, float >;
template class gkg::NoiseCovarianceFunctor< double, double >;
