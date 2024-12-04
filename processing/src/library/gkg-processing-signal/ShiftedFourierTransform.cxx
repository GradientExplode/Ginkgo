#include <gkg-processing-signal/ShiftedFourierTransform.h>
#include <gkg-processing-signal/Apodization.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <cmath>


template < class T >
gkg::ShiftedFourierTransform< T >::ShiftedFourierTransform()
{
}


template < class T >
gkg::ShiftedFourierTransform< T >::~ShiftedFourierTransform()
{
}


template < class T >
void gkg::ShiftedFourierTransform< T >::applyRawDirect(
                              std::vector< std::complex< T > >& data,
                              const std::vector< T >& shift,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );
  
    }
    shiftedDft( data, shift, 1 );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShiftedFourierTransform< T >::applyRawDirect( "
             "std::vector< std::complex< T > >& data, "
             "const std::vector< T >& shift, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform< T >::applyRawInverse(
                              std::vector< std::complex< T > >& data,
                              const std::vector< T >& shift,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );

    }
    shiftedDft( data, shift, -1 );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShiftedFourierTransform< T >::applyRawInverse( "
             "std::vector< std::complex< T > >& data, "
             "const std::vector< T >& shift, "
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
void gkg::ShiftedFourierTransform< T >::applyCenteredDirect(
                              std::vector< std::complex< T > >& data,
                              const std::vector< T >& shift,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );

    }

    shiftedDft( data, shift, 1 );
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
  GKG_CATCH( "template < class T > "
             "void gkg::ShiftedFourierTransform< T >::applyCenteredDirect( "
             "std::vector< std::complex< T > >& data, "
             "const std::vector< T >& shift, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform< T >::applyCenteredInverse(
                              std::vector< std::complex< T > >& data,
                              const std::vector< T >& shift,
                              const gkg::Apodization< T >* apodization ) const
{

  try
  {

    if ( apodization )
    {

      apodize( data, apodization );

    }

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
    shiftedDft( databis, shift, -1 );

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
  GKG_CATCH( "template < class T > "
             "void gkg::ShiftedFourierTransform< T >::applyCenteredInverse( "
             "std::vector< std::complex< T > >& data, "
             "const std::vector< T >& shift, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform< T >::apodize(
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
             "void gkg::ShiftedFourierTransform< T >::apodize( "
             "std::vector< std::complex< T > >& data, "
             "const gkg::Apodization< T >* apodization ) const" );

}


template < class T >
void gkg::ShiftedFourierTransform< T >::shiftedDft(
                                    std::vector< std::complex< T > >& data,
                                    const std::vector< T >& shift,
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

        phi = -2 * M_PI * ( i + shift[ i ] ) * j / ( T )size;
        harmonic = std::polar( ( T )1.0, ( T )sign * phi );
        copy[ i ] += data[ j ] * harmonic;

      }

    }

    // copying back result to input data
    data = copy;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ShiftedFourierTransform< T >::dft( "
             "std::vector< std::complex< T > >& data, "
             "const std::vector< T >& shift, "
             "int sign ) const" );

}


template class gkg::ShiftedFourierTransform< float >;
template class gkg::ShiftedFourierTransform< double >;
