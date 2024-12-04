#ifndef _gkg_core_object_GenericObject_i_h_
#define _gkg_core_object_GenericObject_i_h_


#include <gkg-core-object/GenericObject.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-pattern/RCPointer.h>


template < class T >
inline
gkg::GenericObject::GenericObject( const T& t )
                   : gkg::RCPointer< gkg::BaseObject >( 
                                     new gkg::TypedObject< T >( t ) )
{

}


template < class T >
inline
gkg::GenericObject& gkg::GenericObject::operator=( const T& t )
{

  gkg::RCPointer< gkg::BaseObject >::operator=(
          gkg::RCPointer< gkg::BaseObject >( new gkg::TypedObject< T >( t ) ) );
  return *this;

}


#endif
