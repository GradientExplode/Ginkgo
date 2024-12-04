#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-signal/Apodization.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <cmath>


template < class T >
gkg::FourierTransform< T >::FourierTransform()
{
}


template < class T >
gkg::FourierTransform< T >::~FourierTransform()
{
}


template < class T >
void gkg::FourierTransform< T >::applyRawDirect(
                              std::vector< std::complex< T > >& data,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );
  
    }
    // using Fast Fourier Transform if data size is a power of two
    if ( gkg::PowerOfTwo::getInstance().getLowerOrEqual(
                                                     ( int32_t )data.size() ) ==
         ( int32_t )data.size() )
    {

      fft( data, 1 );

    }
    // using Discrete Fourier Transform if not a power of two
    else
    {

      dft( data, 1 );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::applyRawDirect( "
              "std::vector< std::complex< T > >& data, "
              "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::FourierTransform< T >::applyRawInverse(
                              std::vector< std::complex< T > >& data,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );

    }
    // using Inverse Fast Fourier Transform if data size is a power of two
    if ( gkg::PowerOfTwo::getInstance().getLowerOrEqual(
                                                     ( int32_t )data.size() ) ==
         ( int32_t )data.size() )
    {

      fft( data, -1 );

    }
    // using Inverse Discrete Fourier Transform if not a power of two
    else
    {

      dft( data, -1 );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::applyRawInverse( "
              "std::vector< std::complex< T > >& data, "
              "const gkg::Apodization< T >* apodization ) const" );

}


//
// We know that:   S(k) = SUM[n=0;n=N-1]( s(n)*exp(-jn(2Pi*k/N))
// if we multiply s(n) by (-1)^n, then we get:
//  S'(k) = SUM[n=0;n=N-1]( s(n)(-1)^n*exp(-jn(2Pi*k/N))
//        = SUM[n=0;n=N-1]( s(n)*exp(-jPi*n)*exp(-jn(2Pi*k/N))
//        = SUM[n=0;n=N-1]( s(n)*exp(-jn(2Pi*(k+N/2)/N))
//        = S(k+N/2)
// It is equivalent to centering the Fourier transform
//
template < class T >
void gkg::FourierTransform< T >::applyCenteredDirect(
                              std::vector< std::complex< T > >& data,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );

    }

    // using Fast Fourier Transform if data size is a power of two
    if ( gkg::PowerOfTwo::getInstance().getLowerOrEqual(
                                                     ( int32_t )data.size() ) ==
         ( int32_t )data.size() )
    {

      int32_t i;
      for ( i = 1; i < ( int32_t )data.size(); i += 2 )
      {

        data[ i ] *= -1.0f;

      }
      fft( data, 1 );
      for ( i = 1; i < ( int32_t )data.size(); i += 2 )
      {

        data[ i ] *= -1.0f;

      }

    }
    // using Discrete Fourier Transform if not a power of two
    else
    {

      dft( data, 1 );
      int32_t size = ( int32_t )data.size();
      std::vector< std::complex< T > > databis( size );

      for ( int32_t i = 0; i < size / 2; i++ )
      {

        databis[ i ] = data[ i + ( size + ( size % 2 ) ) / 2 ];

      }
      for ( int32_t i = size / 2; i < size; i++ )
      {

        databis[ i ] = data[ i - size / 2 ];

      }
      data = databis;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::applyCenteredDirect( "
             "std::vector< std::complex< T > >& data, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::FourierTransform< T >::applyCenteredInverse(
                              std::vector< std::complex< T > >& data,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );

    }

    // using Inverse Fast Fourier Transform if data size is a power of two
    if ( gkg::PowerOfTwo::getInstance().getLowerOrEqual(
                                                     ( int32_t )data.size() ) ==
         ( int32_t )data.size() )
    {

      int32_t i;
      for ( i = 1; i < ( int32_t )data.size(); i += 2 )
      {

        data[ i ] *= -1.0f;

      }
      fft( data, -1 );
      for ( i = 1; i < ( int32_t )data.size(); i += 2 )
      {

        data[ i ] *= -1.0f;

      }

    }
    // using Inverse Discrete Fourier Transform if not a power of two
    else
    {

      int32_t size = ( int32_t )data.size();
      std::vector< std::complex< T > > databis( size );

      if ( size % 2 )
      {

        for ( int32_t i = 0; i < ( size + 1 ) / 2; i++ )
        {

          databis[ i ] = data[ i + size / 2 ];

        }
        for ( int32_t i = ( size + 1 ) / 2; i < size; i++ )
        {

          databis[ i ] = data[ i - ( size + 1 ) / 2 ];

        }

      }
      else
      {

        for ( int32_t i = 0; i < size / 2; i++ )
        {

          databis[ i + size / 2 ] = data[ i ];

        }
        for ( int32_t i = size / 2; i < size; i++ )
        {

          databis[ i - size / 2 ] = data[ i ];

        }

      }
      dft( databis, -1 );

      if ( size % 2 )
      {

        for ( int32_t i = 0; i < size / 2; i++ )
        {

          data[ size - 1 - i ] = databis[ i + ( size + 1 )/ 2 ];

        }
        for ( int32_t i = size / 2; i < size; i++ )
        {

          data[ size - 1 - i ] = databis[ i - size / 2 ];

        }

      }
      else
      {

        for ( int32_t i = 0; i < size / 2; i++ )
        {

          data[ size - 1 - i ] = databis[ i + size / 2 ];

        }
        for ( int32_t i = size / 2; i < size; i++ )
        {

          data[ size - 1 - i ] = databis[ i - size / 2 ];

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::applyCenteredInverse( "
             "std::vector< std::complex< T > >& data, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::FourierTransform< T >::apodize(
                              std::vector< std::complex< T > >& data,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization->getCount() > ( int32_t )data.size() )
    {

      throw std::runtime_error( "apodization count is greater than data size" );

    }
    int32_t i = 0, ie = apodization->getCount();
    while ( i != ie )
    {

      data[ i ] *= apodization->getCoefficient( i );
      ++ i;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::apodize( "
             "std::vector< std::complex< T > >& data, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::FourierTransform< T >::fft(
                                    std::vector< std::complex< T > >& data,
                                    int32_t sign ) const
{

  try
  {

    int32_t n, mmax, m, j, istep, i;
    T theta, sinTheta_2;
    std::complex< T > temp, w, wp;

    n = ( ( int32_t )data.size() ) << 1;
    j = 1;
    for ( i = 1; i < n; i += 2 )
    {

      if ( j > i )
      {

        std::swap( data[ ( j - 1 ) / 2 ], data[ ( i - 1 ) / 2 ] );

      }
      m = n >> 1;
      while ( m >= 2 && j > m )
      {

        j -= m;
        m >>= 1;

      }
      j += m;

    }

    mmax = 2;
    while ( n > mmax )
    {

      istep = 2 * mmax;
      theta = 2.0 * M_PI / ( sign * mmax );
      sinTheta_2 = std::sin( 0.5 * theta );
      wp = std::complex< T >( -2.0 * sinTheta_2 * sinTheta_2,
                              std::sin( theta ) );
      w = std::complex< T >( 1.0, 0.0 );
      for ( m = 1; m < mmax; m += 2 )
      {

        for ( i = m; i <= n; i += istep )
        {

          j = i + mmax;
          temp = w * data[ ( j - 1 ) / 2 ];
          data[ ( j - 1 ) / 2 ] = data[ ( i - 1 ) / 2 ] - temp;
          data[ ( i - 1 ) / 2 ] += temp;

        }
        w = w * wp + w;

      }
      mmax = istep;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::fft( "
             "std::vector< std::complex< T > >& data, "
             "int sign ) const" );

}


template < class T >
void gkg::FourierTransform< T >::dft(
                                    std::vector< std::complex< T > >& data,
                                    int32_t sign ) const
{

  try
  {

    int32_t size = ( int32_t )data.size();
    std::vector< std::complex< T > > copy( size );
    int32_t i, j;
    T phi;
    std::complex< T > harmonic;

    // applying Discrete Fourier Transform
    for ( i = 0; i < size; i++ )
    {

      copy[ i ] = ( T )0;
      for ( j = 0; j < size; j++ )
      {

        phi = -2 * M_PI * i * j / ( T )size;
        harmonic = std::polar( ( T )1.0, ( T )sign * phi );
        copy[ i ] += data[ j ] * harmonic;

      }

    }

    // copying back result to input data
    data = copy;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FourierTransform< T >::dft( "
             "std::vector< std::complex< T > >& data, "
             "int sign ) const" );

}


template class gkg::FourierTransform< float >;
template class gkg::FourierTransform< double >;
