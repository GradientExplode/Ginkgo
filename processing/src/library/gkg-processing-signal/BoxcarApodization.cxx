#include <gkg-processing-signal/BoxcarApodization.h>
#include <gkg-processing-signal/ApodizationFactory.h>


RegisterApodization( gkg, BoxcarApodization, float );
RegisterApodization( gkg, BoxcarApodization, double );


template < class T >
gkg::BoxcarApodization< T >::BoxcarApodization(
                                          int32_t count,
                                          typename gkg::Apodization< T >::Type )
                            : gkg::Apodization< T >( count )
{

  // centered and standard apodization(s) are the same!
  size_t i;
  for ( i = 0; i < this->_coefficients.size(); i++ )
  {

    this->_coefficients[ i ] = ( T )1.0;

  } 

}


template < class T >
gkg::BoxcarApodization< T >::~BoxcarApodization()
{
}


template < class T >
std::string gkg::BoxcarApodization< T >::getName() const
{

  return getStaticName();

}


template < class T >
std::string gkg::BoxcarApodization< T >::getStaticName()
{

  return "boxcar";

}


template class gkg::BoxcarApodization< float >;
template class gkg::BoxcarApodization< double >;
