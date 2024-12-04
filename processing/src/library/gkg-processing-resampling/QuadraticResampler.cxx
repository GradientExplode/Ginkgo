#include <gkg-processing-resampling/QuadraticResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::QuadraticResampler< T >::QuadraticResampler()
                             : gkg::SplineResampler< T >()
{

  this->_poles.push_back( std::sqrt( 8.0 ) - 3.0 );
  this->_gain = 24.0 - std::sqrt( 512.0 );

}


template < class T >
gkg::QuadraticResampler< T >::~QuadraticResampler()
{
}


template < class T >
std::string gkg::QuadraticResampler< T >::getName() const
{

  return "Quadratic Resampler";

}


template < class T >
int32_t gkg::QuadraticResampler< T >::getOrder() const
{

  return 2;

}


template < class T >
double 
gkg::QuadraticResampler< T >::getBSplineWeight( int32_t i, double x ) const
{

  x = std::fabs( x - ( double )i );
  if ( x < 0.5 )
  {

    return 0.75 - x * x;

  }
  if ( x < 1.5 )
  {

    x = 1.5 - x;
    return 0.5 * x * x;

  }
  return 0.0;

}


template class gkg::QuadraticResampler< int8_t >;
template class gkg::QuadraticResampler< uint8_t >;
template class gkg::QuadraticResampler< int16_t >;
template class gkg::QuadraticResampler< uint16_t >;
template class gkg::QuadraticResampler< int32_t >;
template class gkg::QuadraticResampler< uint32_t >;
template class gkg::QuadraticResampler< int64_t >;
template class gkg::QuadraticResampler< uint64_t >;
template class gkg::QuadraticResampler< float >;
template class gkg::QuadraticResampler< double >;
template class gkg::QuadraticResampler< std::complex< float > >;
template class gkg::QuadraticResampler< std::complex< double > >;
template class gkg::QuadraticResampler< gkg::RGBComponent >;


RegisterResampler( gkg, QuadraticResampler );
