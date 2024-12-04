#include <gkg-processing-signal/HammingApodization.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <cmath>


RegisterApodization( gkg, HammingApodization, float );
RegisterApodization( gkg, HammingApodization, double );


template < class T >
gkg::HammingApodization< T >::HammingApodization(
                                     int32_t count,
                                     typename gkg::Apodization< T >::Type type )
                      : gkg::Apodization< T >( count )
{

  // standard apodization
  size_t i;
  for ( i = 0; i < this->_coefficients.size(); i++ )
  {

    this->_coefficients[ i ] = ( T )( 0.54 + 0.46 * 
                             std::cos( 2 * M_PI *
                                       ( double )i  /
                                       ( double )this->_coefficients.size() ) );

  } 

  if ( type == gkg::Apodization< T >::Centered )
  {

    this->rotateCoefficients();

  }

}


template < class T >
gkg::HammingApodization< T >::~HammingApodization()
{
}


template < class T >
std::string gkg::HammingApodization< T >::getName() const
{

  return getStaticName();

}


template < class T >
std::string gkg::HammingApodization< T >::getStaticName()
{

  return "hamming";

}


template class gkg::HammingApodization< float >;
template class gkg::HammingApodization< double >;
