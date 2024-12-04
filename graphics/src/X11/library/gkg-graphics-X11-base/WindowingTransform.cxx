#include <gkg-graphics-X11-base/WindowingTransform.h>
#include <algorithm>


template < class T >
gkg::WindowingTransform< T >::WindowingTransform( const T& fromCoordinate1,
                                                  const T& fromCoordinate2,
                                                  const T& toCoordinate1,
                                                  const T& toCoordinate2 )
                             : gkg::RCGUIObject(),
                               _fromCoordinate1( fromCoordinate1 ),
                               _fromCoordinate2( fromCoordinate2 ),
                               _toCoordinate1( toCoordinate1 ),
                               _toCoordinate2( toCoordinate2 )
                         
{
}


template < class T >
gkg::WindowingTransform< T >::~WindowingTransform()
{
}


template < class T >
void gkg::WindowingTransform< T >::setFromRange( const T& fromCoordinate1,
                                                 const T& fromCoordinate2 )
{

  _fromCoordinate1 = fromCoordinate1;
  _fromCoordinate2 = fromCoordinate2;
  processFactor();

}


template < class T >
void gkg::WindowingTransform< T >::setToRange( const T& toCoordinate1,
                                               const T& toCoordinate2 )
{

  _toCoordinate1 = toCoordinate1;
  _toCoordinate2 = toCoordinate2;
  processFactor();

}


template < class T >
T gkg::WindowingTransform< T >::getFromCoordinate1() const
{

  return _fromCoordinate1;

}


template < class T >
T gkg::WindowingTransform< T >::getFromCoordinate2() const
{

  return _fromCoordinate2;

}


template < class T >
T gkg::WindowingTransform< T >::getToCoordinate1() const
{

  return _toCoordinate1;

}


template < class T >
T gkg::WindowingTransform< T >::getToCoordinate2() const
{

  return _toCoordinate2;

}


template < class T >
bool gkg::WindowingTransform< T >::containsFromCoordinate(
                                                 const T& fromCoordinate ) const
{

  return ( fromCoordinate >= std::min( _fromCoordinate1, _fromCoordinate2 ) ) &&
         ( fromCoordinate <= std::max( _fromCoordinate1, _fromCoordinate2 ) );

}


template < class T >
bool gkg::WindowingTransform< T >::containsToCoordinate(
                                                   const T& toCoordinate ) const
{

  return ( toCoordinate >= std::min( _toCoordinate1, _toCoordinate2 ) ) &&
         ( toCoordinate <= std::max( _toCoordinate1, _toCoordinate2 ) );

}


template < class T >
T gkg::WindowingTransform< T >::getDirect( const T& fromCoordinate ) const
{

  return _toCoordinate1 +
         _factor * ( applyFunction( fromCoordinate ) -
                     applyFunction( _fromCoordinate1 ) );

}


template < class T >
T gkg::WindowingTransform< T >::getInverse( const T& toCoordinate ) const
{

  if ( _factor == ( T )0 )
  {

    return ( T )0;

  }
  else
  {

    return applyInverseFunction( applyFunction( _fromCoordinate1 ) +
                                 ( toCoordinate - _toCoordinate1 ) / _factor );

  }

}


template < class T >
T gkg::WindowingTransform< T >::getLimitTransform(
                                                 const T& fromCoordinate ) const
{

  T fromMinimum = std::min( _fromCoordinate1, _fromCoordinate2 );
  T fromMaximum = std::max( _fromCoordinate1, _fromCoordinate2 );

  if ( fromCoordinate > fromMaximum )
  {

    return getDirect( fromMaximum );

  }
  else if ( fromCoordinate < fromMinimum )
  {

    return getDirect( fromMinimum );

  }
  else
  {

    return getDirect( fromCoordinate );

  }

}


template < class T >
void gkg::WindowingTransform< T >::processFactor()
{

  T f1 = applyFunction( _fromCoordinate1 );
  T f2 = applyFunction( _fromCoordinate2 );

  if ( f1 != f2 )
  {

    _factor = ( _toCoordinate2 - _toCoordinate1 ) / ( f2 - f1 );

  }
  else
  {

    _factor = ( T )0;

  }

}



template class gkg::WindowingTransform< float >;
template class gkg::WindowingTransform< double >;
