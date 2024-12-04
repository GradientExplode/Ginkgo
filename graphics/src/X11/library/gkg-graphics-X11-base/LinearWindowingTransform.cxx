#include <gkg-graphics-X11-base/LinearWindowingTransform.h>


template < class T >
gkg::LinearWindowingTransform< T >::LinearWindowingTransform(
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
gkg::LinearWindowingTransform< T >::~LinearWindowingTransform()
{
}


template < class T >
T gkg::LinearWindowingTransform< T >::applyFunction( const T& value ) const
{

  return value;

}


template < class T >
T 
gkg::LinearWindowingTransform< T >::applyInverseFunction( const T& value ) const
{

  return value;

}


template class gkg::LinearWindowingTransform< float >;
template class gkg::LinearWindowingTransform< double >;
