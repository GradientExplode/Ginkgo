#include <gkg-processing-signal/TukeyApodization.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define TUKEY_DEFAULT_ALPHA    0.68


RegisterApodization( gkg, TukeyApodization, float );
RegisterApodization( gkg, TukeyApodization, double );


template < class T >
gkg::TukeyApodization< T >::TukeyApodization(
                                     int32_t count,
                                     typename gkg::Apodization< T >::Type type )
                      : gkg::Apodization< T >( count ),
                        _type( type ),
                        _alpha( TUKEY_DEFAULT_ALPHA )
{

  try
  {

    this->computeCoefficients();


  }
  GKG_CATCH( "template < class T > "
             "gkg::TukeyApodization< T >::TukeyApodization( "
             "int32_t count, "
             "typename gkg::Apodization< T >::Type type )" );

}


template < class T >
gkg::TukeyApodization< T >::~TukeyApodization()
{
}


template < class T >
void gkg::TukeyApodization< T >::setAlpha( double alpha )
{

  try
  {

    if ( ( alpha < 0.0 ) || ( alpha > 1.0 ) )
    {

      throw std::runtime_error( "alpha must lie between 0.0 and 1.0" );

    }
    _alpha = alpha;
    this->computeCoefficients();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::TukeyApodization< T >::setAlpha( double alpha )" );

}


template < class T >
std::string gkg::TukeyApodization< T >::getName() const
{

  return getStaticName();

}


template < class T >
std::string gkg::TukeyApodization< T >::getStaticName()
{

  return "tukey";

}


template < class T >
void gkg::TukeyApodization< T >::computeCoefficients()
{

  try
  {

    // collecting the number of coefficients
    size_t count = this->_coefficients.size();

    // centered apodization
    size_t i;
    T limit = ( T )_alpha * ( T )count / ( T )2.0;

    for ( i = 0; i < count; i++ )
    {

      if ( std::fabs( ( T )count / 2 - i ) < limit )
      {

        this->_coefficients[ i ] = ( T )1.0;

      }
      else
      {

        this->_coefficients[ i ] =
          ( T )( 0.5 + 0.5 * std::cos( M_PI *
                                       ( std::fabs( ( double )i -
                                                    ( double )count / 2.0 ) -
                                         _alpha * ( double )count / 2.0 ) /
                                       ( ( 1 - _alpha ) * ( double )count ) ) );

      }

    } 

    if ( _type == gkg::Apodization< T >::Standard )
    {

      this->rotateCoefficients();

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::TukeyApodization< T >::computeCoefficients()" );

}



template class gkg::TukeyApodization< float >;
template class gkg::TukeyApodization< double >;


#undef TUKEY_DEFAULT_ALPHA
