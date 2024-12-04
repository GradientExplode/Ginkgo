#ifndef _gkg_core_object_TypedObject_i_h_
#define _gkg_core_object_TypedObject_i_h_

#include <gkg-core-object/TypedObject.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/ObjectFactory_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-io/TypeOf.h>
#include <typeinfo>


template < class T >
inline
gkg::TypedObject< T >::TypedObject()
                      : gkg::BaseObject()
{
}


template < class T >
inline
gkg::TypedObject< T >::TypedObject( const T& t )
                      : gkg::BaseObject(),
                        _value( t )
{
}


template < class T >
inline
gkg::TypedObject< T >::TypedObject( const gkg::TypedObject< T >& other )
                      : gkg::BaseObject(),
                        _value( other._value )
{
}


template < class T >
inline
gkg::TypedObject< T >::~TypedObject()
{
}


template < class T >
inline
gkg::TypedObject< T >& 
gkg::TypedObject< T >::operator=( const gkg::TypedObject< T >& other )
{

  if ( this != &other )
  {

    _value = other.getTypedValue();
    if ( other.hasSyntax() && hasSyntax() )
    {

      setSyntax( other.getSyntax() );

    }

  }
  return *this;

}


template < class T >
inline
std::string gkg::TypedObject< T >::getTypeName() const
{

  return typeid( _value ).name();

}


template < class T >
inline
std::string gkg::TypedObject< T >::getTypeOf() const
{

  return gkg::TypeOf< T >::getName();

}


template < class T >
inline
gkg::BaseObject* gkg::TypedObject< T >::clone() const
{

  return new gkg::TypedObject< T >( *this );

}


template < class T >
inline
gkg::BaseObject* 
gkg::TypedObject< T >::mute( const gkg::ObjectFactory& objectFactory )
{

  return objectFactory.create( *this );

}


template < class T >
inline
T& gkg::TypedObject< T >::getTypedValue()
{

  return _value;

}


template < class T >
inline
const T& gkg::TypedObject< T >::getTypedValue() const
{

  return _value;

}


#endif
