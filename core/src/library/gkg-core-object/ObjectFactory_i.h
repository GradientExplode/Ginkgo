#ifndef _gkg_core_object_ObjectFactory_i_h_
#define _gkg_core_object_ObjectFactory_i_h_


#include <gkg-core-object/ObjectFactory.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <typeinfo>


template < class T >
inline
gkg::ObjectFactory::ObjectFactory( const T& t )
                   : _id( typeid( T ).name() )
{
}


template < class T >
inline
gkg::BaseObject* gkg::ObjectFactory::create( T& t ) const
{

  return create( gkg::TypedObject< T >( t ) );

}


template < class T >
inline
gkg::BaseObject* 
gkg::ObjectFactory::create( gkg::TypedObject< T >& thing ) const
{

  const std::map< std::string,
                  std::map< std::string,
                            gkg::ObjectFactory::Creator > >::const_iterator
    m = getCreators().find( _id );
  if ( m == getCreators().end() )
  {

    // failure: no creator registered
    return 0;

  }
  const std::map< std::string, gkg::ObjectFactory::Creator >::const_iterator
    c = m->second.find( typeid( thing.getTypedValue() ).name() );
  return c->second( thing );

}


template < class T, class U >
inline
void gkg::ObjectFactory::registerCreator( const T& outType,
                                          const U& inType,
                                          gkg::ObjectFactory::Creator creator )
{

  registerCreatorIds( typeid( outType ).name(),
                      typeid( inType ).name(),
                      creator );

}


template < class T >
inline
void gkg::ObjectFactory::registerCreatorForId(
                                           const std::string& outType,
                                           const T& inType,
                                           gkg::ObjectFactory::Creator creator )
{

  registerCreatorIds( outType,
                      typeid( inType ).name(),
                      creator );

}


#endif
