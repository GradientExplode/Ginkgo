#include <gkg-processing-resampling/QuarticResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::QuarticResampler< T >::QuarticResampler()
                           : gkg::SplineResampler< T >()
{

  this->_poles.push_back( std::sqrt( 664.0 - std::sqrt( 438976.0 ) ) +
                          std::sqrt( 304.0 ) - 19.0 );
  this->_poles.push_back( std::sqrt( 664.0 + std::sqrt( 438976.0 ) ) -
                          std::sqrt( 304.0 ) - 19.0 );
  this->_gain = ( 1.0 - this->_poles[ 0 ] ) * ( 1.0 - this->_poles[ 1 ] );
  this->_gain *= this->_gain;

}


template < class T >
gkg::QuarticResampler< T >::~QuarticResampler()
{
}


template < class T >
std::string gkg::QuarticResampler< T >::getName() const
{

  return "Quartic Resampler";

}


template < class T >
int32_t gkg::QuarticResampler< T >::getOrder() const
{

  return 4;

}


template < class T >
double gkg::QuarticResampler< T >::getBSplineWeight( int32_t i, double x ) const
{

  x = std::fabs( x - ( double )i );
  if ( x < 0.5 )
  {

    x *= x;
    return x * ( x * 0.25 - 0.625 ) + 115.0 / 192.0;

  }
  if ( x < 1.5 )
  {

    return x * ( x * ( x * ( 5.0 / 6.0 - x * ( 1.0 / 6.0 ) ) - 1.25 ) +
           5.0 / 24.0 ) + 55.0 / 96.0;

  }
  if ( x < 2.5 )
  {

    x -= 2.5;
    x *= x;
    return x * x * ( 1.0 / 24.0 );

  }
  return 0.0;

}


template class gkg::QuarticResampler< int8_t >;
template class gkg::QuarticResampler< uint8_t >;
template class gkg::QuarticResampler< int16_t >;
template class gkg::QuarticResampler< uint16_t >;
template class gkg::QuarticResampler< int32_t >;
template class gkg::QuarticResampler< uint32_t >;
template class gkg::QuarticResampler< int64_t >;
template class gkg::QuarticResampler< uint64_t >;
template class gkg::QuarticResampler< float >;
template class gkg::QuarticResampler< double >;
template class gkg::QuarticResampler< std::complex< float > >;
template class gkg::QuarticResampler< std::complex< double > >;
template class gkg::QuarticResampler< gkg::RGBComponent >;


RegisterResampler( gkg, QuarticResampler );
