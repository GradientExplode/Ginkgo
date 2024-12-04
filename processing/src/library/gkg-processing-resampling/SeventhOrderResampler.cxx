#include <gkg-processing-resampling/SeventhOrderResampler.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <cmath>


template < class T >
gkg::SeventhOrderResampler< T >::SeventhOrderResampler()
                         : gkg::SplineResampler< T >()
{

  this->_poles.push_back( 
                -0.5352804307964381655424037816816460718339231523426924148812 );
  this->_poles.push_back( 
              -0.122554615192326690515272264359357343605486549427295558490763 );
  this->_poles.push_back( 
             -0.0091486948096082769285930216516478534156925639545994482648003 );
  this->_gain = 3.0248282036441843886795463832305782146916878615537002580987;

}


template < class T >
gkg::SeventhOrderResampler< T >::~SeventhOrderResampler()
{
}


template < class T >
std::string gkg::SeventhOrderResampler< T >::getName() const
{

  return "Seventh Order Resampler";

}


template < class T >
int32_t gkg::SeventhOrderResampler< T >::getOrder() const
{

  return 7;

}


template < class T >
double 
gkg::SeventhOrderResampler< T >::getBSplineWeight( int32_t i, double x ) const
{

  x = std::fabs( x - ( double )i );
  if ( x < 1.0 )
  {

    double f = x * x;
    return f * ( f * ( f * ( x * ( 1.0 / 144.0 ) - 1.0 / 36.0 ) + 1.0 / 9.0 ) -
           1.0 / 3.0 ) + 151.0 / 315.0;

  }
  if ( x < 2.0 )
  {

    return x * ( x * ( x * ( x * ( x * ( x * ( 0.05 - x * ( 1.0 / 240.0 ) ) -
           7.0 / 30.0 ) + 0.5 ) - 7.0 / 18.0 ) - 0.1 ) - 7.0 / 90.0 ) +
           103.0 / 210.0;

  }
  if ( x < 3.0 )
  {

    return x * ( x * ( x * ( x * ( x * ( x * ( x * ( 1.0 / 720.0 ) -
           1.0 / 36.0 ) + 7.0 / 30.0 ) - 19.0 / 18.0 ) + 49.0 / 18.0 ) -
           23.0 / 6.0 ) + 217.0 / 90.0 ) - 139.0 / 630.0;

  }
  if ( x < 4.0 )
  {

    double f = 4.0 - x;
    x = f * f * f;
    return x * x * f * ( 1.0 / 5040.0 );

  }
  return 0.0;

}


template class gkg::SeventhOrderResampler< int8_t >;
template class gkg::SeventhOrderResampler< uint8_t >;
template class gkg::SeventhOrderResampler< int16_t >;
template class gkg::SeventhOrderResampler< uint16_t >;
template class gkg::SeventhOrderResampler< int32_t >;
template class gkg::SeventhOrderResampler< uint32_t >;
template class gkg::SeventhOrderResampler< int64_t >;
template class gkg::SeventhOrderResampler< uint64_t >;
template class gkg::SeventhOrderResampler< float >;
template class gkg::SeventhOrderResampler< double >;
template class gkg::SeventhOrderResampler< std::complex< float > >;
template class gkg::SeventhOrderResampler< std::complex< double > >;
template class gkg::SeventhOrderResampler< gkg::RGBComponent >;


RegisterResampler( gkg, SeventhOrderResampler );
