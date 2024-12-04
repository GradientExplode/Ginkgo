#ifndef _gkg_processing_signal_FIRFilter3d_i_h_
#define _gkg_processing_signal_FIRFilter3d_i_h_


#include <gkg-processing-signal/FIRFilter3d.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <cmath>
#include <iomanip>


template < class T >
inline
gkg::FIRFilter3d< T >::FIRFilter3d( typename gkg::FIRFilter3d< T >::Type type,
                                    int32_t filterSize,
                                    const std::vector< double >& frequencies,
                                    const std::string& apodizationName,
                                    bool verbose )
                      : _verbose( verbose )
{

  try
  {

    // filter size must be odd
    if ( filterSize % 2 == 0 )
    {

      throw std::runtime_error( "filter size must be odd" );

    }
    int32_t halfFilterSize = filterSize / 2;

    // allocating impulse response
    _h.resize( filterSize );

    // collecting cut frequency count
    int32_t frequencyCount = ( int32_t )frequencies.size();

    //  in case of low pass filter
    int32_t n = 0;
    if ( type == gkg::FIRFilter3d< T >::LowPass )
    {

      // sanity check
      if ( frequencyCount != 1 )
      {

        throw std::runtime_error( "bad frequency count" );

      }
      if ( ( frequencies[ 0 ] < 0 ) || (  frequencies[ 0 ] > 0.5 ) )
      {

        throw std::runtime_error( "cut frequency must be in the range 0-0.5" );

      }

      // processing impulse response
      for ( n = 0; n < filterSize; n++ )
      {

        if ( n == halfFilterSize )
        {

          _h[ n ] = 2.0 * frequencies[ 0 ];

        }
        else
        {

          _h[ n ] = std::sin( 2.0 * M_PI * frequencies[ 0 ] *
                                                     ( n - halfFilterSize ) ) /
                    ( M_PI * ( n - halfFilterSize ) );

        }

      }

    }
    //  in case of high pass filter
    else if ( type == gkg::FIRFilter3d< T >::HighPass )
    {

      // sanity check
      if ( frequencyCount != 1 )
      {

        throw std::runtime_error( "bad frequency count" );

      }
      if ( ( frequencies[ 0 ] < 0 ) || (  frequencies[ 0 ] > 0.5 ) )
      {

        throw std::runtime_error( "cut frequency must be in the range 0-0.5" );

      }

      // processing impulse response
      int32_t n = 0;
      for ( n = 0; n < filterSize; n++ )
      {

        if ( n == halfFilterSize )
        {

          _h[ n ] = 1.0 - 2.0 * frequencies[ 0 ];

        }
        else
        {

          _h[ n ] = -std::sin( 2.0 * M_PI * frequencies[ 0 ] *
                                                     ( n - halfFilterSize ) ) /
                    ( M_PI * ( n - halfFilterSize ) );

        }

      }

    }
    //  in case of band pass filter
    else if ( type == gkg::FIRFilter3d< T >::BandPass )
    {

      // sanity check
      if ( frequencyCount != 2 )
      {

        throw std::runtime_error( "bad frequency count" );

      }
      if ( frequencies[ 0 ]  >= frequencies[ 1 ] )
      {

        throw std::runtime_error( "first frequency must be lower than second" );

      }
      if ( ( frequencies[ 0 ] < 0 ) || (  frequencies[ 0 ] > 0.5 ) )
      {

        throw std::runtime_error( 
                             "first cut frequency must be in the range 0-0.5" );

      }
      if ( ( frequencies[ 0 ] < 0 ) || (  frequencies[ 0 ] > 0.5 ) )
      {

        throw std::runtime_error( 
                            "second cut frequency must be in the range 0-0.5" );

      }

      // processing impulse response
      int32_t n = 0;
      for ( n = 0; n < filterSize; n++ )
      {

        if ( n == halfFilterSize )
        {

          _h[ n ] = 2.0 * ( frequencies[ 1 ] - frequencies[ 0 ] );

        }
        else
        {

          _h[ n ] = std::sin( 2.0 * M_PI * frequencies[ 1 ] *
                                                     ( n - halfFilterSize ) ) /
                    ( M_PI * ( n - halfFilterSize ) ) -
                    std::sin( 2.0 * M_PI * frequencies[ 0 ] *
                                                     ( n - halfFilterSize ) ) /
                    ( M_PI * ( n - halfFilterSize ) );

        }

      }

    }
    //  in case ofband stop filter
    else if ( type == gkg::FIRFilter3d< T >::BandStop )
    {

      // sanity check
      if ( frequencyCount != 2 )
      {

        throw std::runtime_error( "bad frequency count" );

      }
      if ( frequencies[ 0 ]  >= frequencies[ 1 ] )
      {

        throw std::runtime_error( "first frequency must be lower than second" );

      }
      if ( ( frequencies[ 0 ] < 0 ) || (  frequencies[ 0 ] > 0.5 ) )
      {

        throw std::runtime_error( 
                             "first cut frequency must be in the range 0-0.5" );

      }
      if ( ( frequencies[ 0 ] < 0 ) || (  frequencies[ 0 ] > 0.5 ) )
      {

        throw std::runtime_error( 
                            "second cut frequency must be in the range 0-0.5" );

      }

      // processing impulse response
      int32_t n = 0;
      for ( n = 0; n < filterSize; n++ )
      {

        if ( n == halfFilterSize )
        {

          _h[ n ] = 1.0 - 2.0 * ( frequencies[ 1 ] - frequencies[ 0 ] );

        }
        else
        {

          _h[ n ] = std::sin( 2.0 * M_PI * frequencies[ 0 ] *
                                                     ( n - halfFilterSize ) ) /
                    ( M_PI * ( n - halfFilterSize ) ) -
                    std::sin( 2.0 * M_PI * frequencies[ 1 ] *
                                                     ( n - halfFilterSize ) ) /
                    ( M_PI * ( n - halfFilterSize ) );

        }

      }

    }

    // adding apodization
    gkg::Apodization< double >*
      apodization = gkg::ApodizationFactory< double >::getInstance().
                      createApodization( apodizationName,
                                         filterSize,
                                         gkg::Apodization< double >::Centered );

    for ( n = 0; n < filterSize; n++ )
    {


      _h[ n ] *= apodization->getCoefficient( n );

    }

    delete apodization;


  }
  GKG_CATCH( "template < class T > "             "inline "
             "gkg::FIRFilter3d< T >::FIRFilter3d( "
             "typename gkg::FIRFilter3d< T >::Type type, "
             "int32_t filterSize, "
             "const std::vector< float >& frequencies, "
             "const std::string& apodizationName, "
             "bool verbose )" );

}



template < class T >
inline
gkg::FIRFilter3d< T >::~FIRFilter3d()
{
}


template < class T >
inline
void 
gkg::FIRFilter3d< T >::filter( const gkg::Volume< T >& in,
                               gkg::Volume< T >& out,
                               typename gkg::FIRFilter3d< T >::Axis axis ) const
{

  try
  {

    int32_t sizeX = in.getSizeX();
    int32_t sizeY = in.getSizeY();
    int32_t sizeZ = in.getSizeZ();
    int32_t sizeT = in.getSizeT();
    int32_t filterSize = ( int32_t )_h.size();
    int32_t halfFilterSize = filterSize / 2;

    gkg::Volume< T >* pout = &out;
    if ( &out == &in )
    {

      pout = new Volume< T >( sizeX, sizeY, sizeZ, sizeT );
      pout->getHeader() = in.getHeader();

    }
    else
    {

      pout->reallocate( sizeX, sizeY, sizeZ, sizeT );
      pout->getHeader() = in.getHeader();

    }
    int32_t x, y, z, t, n;
    double value = 0.0;
    if ( axis == gkg::FIRFilter3d< T >::X )
    {


      int32_t nx = 0;
      for ( t = 0; t < sizeT; t++ )
      {

        if ( _verbose )
        {

          if ( t )
          {

            std::cout << gkg::Eraser( 38 );

          }
          std::cout << "smooting along x [ " << std::setw( 7 ) << t + 1
                    << " / " << std::setw( 7 ) << sizeT
                    << " ]" << std::flush;

        }
        for ( z = 0; z < sizeZ; z++ )
        {

          if ( _verbose )
          {

            if ( z )
            {

              std::cout << gkg::Eraser( 22 );

            }
            std::cout << " [ " << std::setw( 7 ) << z + 1
                      << " / " << std::setw( 7 ) << sizeZ
                      << " ]" << std::flush;

          }
          for ( y = 0; y < sizeY; y++ )
          {

            for ( x = 0; x < sizeX; x++ )
            {

              value = 0.0;
              for ( n = 0; n < filterSize; n++ )
              {

                nx = x - n + halfFilterSize;
                if ( ( nx >= 0 ) && ( nx < sizeX ) )
                {

                  value += _h[ n ] * ( double )in( nx, y, z, t );

                }

              }
              ( *pout )( x, y, z, t ) = ( T )value;

            }

          }

        }

      }

    }
    else if ( axis == gkg::FIRFilter3d< T >::Y )
    {

      int32_t ny = 0;
      for ( t = 0; t < sizeT; t++ )
      {

        if ( _verbose )
        {

          if ( t )
          {

            std::cout << gkg::Eraser( 38 );

          }
          std::cout << "smooting along y [ " << std::setw( 7 ) << t + 1
                    << " / " << std::setw( 7 ) << sizeT
                    << " ]" << std::flush;

        }
        for ( z = 0; z < sizeZ; z++ )
        {

          if ( _verbose )
          {

            if ( z )
            {

              std::cout << gkg::Eraser( 22 );

            }
            std::cout << " [ " << std::setw( 7 ) << z + 1
                      << " / " << std::setw( 7 ) << sizeZ
                      << " ]" << std::flush;

          }
          for ( x = 0; x < sizeX; x++ )
          {

            for ( y = 0; y < sizeY; y++ )
            {

              value = 0.0;
              for ( n = 0; n < filterSize; n++ )
              {

                ny = y - n + halfFilterSize;
                if ( ( ny >= 0 ) && ( ny < sizeY ) )
                {

                  value += _h[ n ] * ( double )in( x, ny, z, t );

                }

              }
              ( *pout )( x, y, z, t ) = ( T )value;

            }

          }

        }

      }

    }
    else if ( axis == gkg::FIRFilter3d< T >::Z )
    {

      int32_t nz = 0;
      for ( t = 0; t < sizeT; t++ )
      {

        if ( _verbose )
        {

          if ( t )
          {

            std::cout << gkg::Eraser( 38 );

          }
          std::cout << "smooting along z [ " << std::setw( 7 ) << t + 1
                    << " / " << std::setw( 7 ) << sizeT
                    << " ]" << std::flush;

        }
        for ( y = 0; y < sizeY; y++ )
        {

          if ( _verbose )
          {

            if ( y )
            {

              std::cout << gkg::Eraser( 22 );

            }
            std::cout << " [ " << std::setw( 7 ) << y + 1
                      << " / " << std::setw( 7 ) << sizeY
                      << " ]" << std::flush;

          }
          for ( x = 0; x < sizeX; x++ )
          {

            for ( z = 0; z < sizeZ; z++ )
            {

              value = 0.0;
              for ( n = 0; n < filterSize; n++ )
              {

                nz = z - n + halfFilterSize;
                if ( ( nz >= 0 ) && ( nz < sizeZ ) )
                {

                  value += _h[ n ] * ( double )in( x, y, nz, t );

                }

              }
              ( *pout )( x, y, z, t ) = ( T )value;

            }

          }

        }

      }

    }

    if ( &out == &in )
    {

      out = *pout;
      delete pout;

    }

    if ( _verbose )
    {

      std::cout << gkg::Eraser( 60 );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::FIRFilter3d< T >::filter( const gkg::Volume< T >& in, "
             "gkg::Volume< T >& out, "
             "typename gkg::FIRFilter3d< T >::Axes axes ) const" );

}


#endif
