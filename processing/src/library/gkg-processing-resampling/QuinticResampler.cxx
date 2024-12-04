#include <gkg-processing-resampling/QuinticResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::QuinticResampler< T >::QuinticResampler()
                           : gkg::SplineResampler< T >()
{

  this->_poles.push_back( 0.5 * ( std::sqrt( 270.0 - std::sqrt( 70980.0 ) ) +
                          std::sqrt( 105.0 ) - 13.0 ) );
  this->_poles.push_back( 0.5 * ( std::sqrt( 270.0 + std::sqrt( 70980.0 ) ) -
                          std::sqrt( 105.0 ) - 13.0 ) );
  this->_gain = ( 1.0 - this->_poles[ 0 ] ) * ( 1.0 - this->_poles[ 1 ] );
  this->_gain *= this->_gain;

}


template < class T >
gkg::QuinticResampler< T >::~QuinticResampler()
{
}


template < class T >
std::string gkg::QuinticResampler< T >::getName() const
{

  return "Quintic Resampler";

}


template < class T >
int32_t gkg::QuinticResampler< T >::getOrder() const
{

  return 5;

}


template < class T >
double gkg::QuinticResampler< T >::getBSplineWeight( int32_t i, double x ) const
{

  x = std::fabs( x - ( double )i );
  if ( x < 1.0 )
  {

    double f = x * x;
    return f * ( f * ( 0.25 - x * ( 1.0 / 12.0 ) ) - 0.5 ) + 0.55;

  }
  if ( x < 2.0 )
  {

    return x * ( x * ( x * ( x * ( x * ( 1.0 / 24.0 ) - 0.375 ) + 1.25 ) -
           1.75 ) + 0.625 ) + 0.425;

  }
  if ( x < 3.0 )
  {

    double f = 3.0 - x;
    x = f * f;
    return f * x * x * ( 1.0 / 120.0 );

  }
  return 0.0;

}


template class gkg::QuinticResampler< int8_t >;
template class gkg::QuinticResampler< uint8_t >;
template class gkg::QuinticResampler< int16_t >;
template class gkg::QuinticResampler< uint16_t >;
template class gkg::QuinticResampler< int32_t >;
template class gkg::QuinticResampler< uint32_t >;
template class gkg::QuinticResampler< int64_t >;
template class gkg::QuinticResampler< uint64_t >;
template class gkg::QuinticResampler< float >;
template class gkg::QuinticResampler< double >;
template class gkg::QuinticResampler< std::complex< float > >;
template class gkg::QuinticResampler< std::complex< double > >;
template class gkg::QuinticResampler< gkg::RGBComponent >;


RegisterResampler( gkg, QuinticResampler );
