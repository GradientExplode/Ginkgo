#include <gkg-processing-signal/Apodization.h>
#include <algorithm>


template < class T >
gkg::Apodization< T >::Apodization( int32_t count )
                      : _coefficients( count )
{
}


template < class T >
gkg::Apodization< T >::~Apodization()
{
}


template < class T >
int32_t gkg::Apodization< T >::getCount() const
{

  return ( int32_t )_coefficients.size();

}


template < class T >
const T& gkg::Apodization< T >::getCoefficient( int32_t index ) const
{

  return _coefficients[ index ];

}


template < class T >
const std::vector< T >& gkg::Apodization< T >::getCoefficients() const
{

  return _coefficients;

}    


template < class T >
void gkg::Apodization< T >::rotateCoefficients()
{

  if ( _coefficients.size() % 2U == 0U )
  {

    for ( size_t i = 0; i < _coefficients.size() / 2; i++ )
    {

      std::swap( _coefficients[ i ],
                 _coefficients[ i  + _coefficients.size() / 2 ] );

    }

  }
  else
  {

    std::vector< T > coefficients( _coefficients.size() );
    for ( size_t i = 0; i < _coefficients.size() / 2; i++ )
    {

      coefficients[ i ] = _coefficients[ i  + _coefficients.size() / 2 + 1 ];

    }

    for ( size_t i = 0; i < _coefficients.size() / 2 + 1; i++ )
    {

      coefficients[ i + _coefficients.size() / 2  ] = _coefficients[ i ];

    }
    _coefficients = coefficients;

  }

}


template class gkg::Apodization< float >;
template class gkg::Apodization< double >;
