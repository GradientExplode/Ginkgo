#ifndef _gkg_communication_algorithm_TypedAlgorithmParameter_i_h_
#define _gkg_communication_algorithm_TypedAlgorithmParameter_i_h_


#include <gkg-communication-algorithm/TypedAlgorithmParameter.h>
#include <gkg-communication-algorithm/AlgorithmParameter_i.h>
#include <gkg-core-io/TypeOf.h>


template < class T >
inline
gkg::TypedAlgorithmParameter< T >::TypedAlgorithmParameter(
                                                const std::string& name,
                                                bool isInput,
                                                bool isOutput,
                                                const std::string& description )
                                  : gkg::AlgorithmParameter(
                                                name,
                                                gkg::TypeOf< T >::getName(),
                                                isInput,
                                                isOutput,
                                                description ),
                                    _pointee( new T )
{
}


template < class T >
inline
gkg::TypedAlgorithmParameter< T >::~TypedAlgorithmParameter()
{
}


template < class T >
inline
const gkg::RCPointer< T >&
gkg::TypedAlgorithmParameter< T >::getPointee() const
{

  return _pointee;

}


template < class T >
inline
void gkg::TypedAlgorithmParameter< T >::set( const T& value )
{

  *_pointee = value;

}


template < class T >
inline
void gkg::TypedAlgorithmParameter< T >::connect(
                                      gkg::TypedAlgorithmParameter< T >* other )
{

  _pointee = other->_pointee;

}


template < class T >
inline
gkg::TypedAlgorithmParameter< T >*
gkg::TypedAlgorithmParameter< T >::getTypedAlgorithmParameter( 
                                   gkg::AlgorithmParameter* algorithmParameter )
{

  return static_cast<gkg::TypedAlgorithmParameter< T >* >( algorithmParameter );

}


#endif

