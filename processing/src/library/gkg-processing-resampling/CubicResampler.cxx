#include <gkg-processing-resampling/CubicResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::CubicResampler< T >::CubicResampler()
                         : gkg::SplineResampler< T >()
{

  this->_poles.push_back( std::sqrt( 3.0 ) - 2.0 );
  this->_gain = 12.0 - std::sqrt( 108.0 );

}


template < class T >
gkg::CubicResampler< T >::~CubicResampler()
{
}


template < class T >
std::string gkg::CubicResampler< T >::getName() const
{

  return "Cubic Resampler";

}


template < class T >
int32_t gkg::CubicResampler< T >::getOrder() const
{

  return 3;

}


template < class T >
double gkg::CubicResampler< T >::getBSplineWeight( int32_t i, double x ) const
{

  x = std::fabs( x - ( double )i );
  if ( x < 1.0 )
  {

    return ( x * x * ( x - 2.0 ) * 3.0 + 4.0 ) * ( 1.0 / 6.0 );

  }
  if ( x < 2.0 )
  {

    x = 2.0 - x;
    return x * x * x * ( 1.0 / 6.0 );

  }
  return 0.0;

}


template class gkg::CubicResampler< int8_t >;
template class gkg::CubicResampler< uint8_t >;
template class gkg::CubicResampler< int16_t >;
template class gkg::CubicResampler< uint16_t >;
template class gkg::CubicResampler< int32_t >;
template class gkg::CubicResampler< uint32_t >;
template class gkg::CubicResampler< int64_t >;
template class gkg::CubicResampler< uint64_t >;
template class gkg::CubicResampler< float >;
template class gkg::CubicResampler< double >;
template class gkg::CubicResampler< std::complex< float > >;
template class gkg::CubicResampler< std::complex< double > >;
template class gkg::CubicResampler< gkg::RGBComponent >;


RegisterResampler( gkg, CubicResampler );
