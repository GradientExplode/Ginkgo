#include <gkg-graphics-X11-base/LogWindowingTransform.h>
#include <cmath>


template < class T >
const T gkg::LogWindowingTransform< T >::LogMin = ( T )1.0e-38;
template < class T >
const T gkg::LogWindowingTransform< T >::LogMax = ( T )1.0e+38;


template < class T >
gkg::LogWindowingTransform< T >::LogWindowingTransform(
                                                       const T& fromCoordinate1,
                                                       const T& fromCoordinate2,
                                                       const T& toCoordinate1,
                                                       const T& toCoordinate2 )
                                : gkg::WindowingTransform< T >( fromCoordinate1,
                                                                fromCoordinate2,
                                                                toCoordinate1,
                                                                toCoordinate2 )
{

  this->processFactor();

}


template < class T >
gkg::LogWindowingTransform< T >::~LogWindowingTransform()
{
}


template < class T >
T gkg::LogWindowingTransform< T >::applyFunction( const T& value ) const
{

  if ( value < gkg::LogWindowingTransform< T >::LogMin )
  {

    return ( T )std::log( ( double )gkg::LogWindowingTransform< T >::LogMin );

  }
  else if ( value > gkg::LogWindowingTransform< T >::LogMax )
  {

    return ( T )std::log( ( double )gkg::LogWindowingTransform< T >::LogMax );

  }
  else
  {

    return ( T )std::log( ( double )value );

  }

}


template < class T >
T 
gkg::LogWindowingTransform< T >::applyInverseFunction( const T& value ) const
{

  return ( T )std::exp( ( double )value );

}


template class gkg::LogWindowingTransform< float >;
template class gkg::LogWindowingTransform< double >;
