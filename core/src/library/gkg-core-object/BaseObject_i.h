#ifndef _gkg_core_object_BaseObject_i_h_
#define _gkg_core_object_BaseObject_i_h_


#include <gkg-core-object/BaseObject.h>
#include <gkg-core-object/TypedObject.h>
#include <gkg-core-object/GenericObject.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/TypeOf.h>


template < class T >
inline
const T& gkg::BaseObject::getValue() const
{

  try
  {

    const gkg::TypedObject< T >*
      t = dynamic_cast< const TypedObject< T >* >( this );
    if ( t )
    {

      return t->getTypedValue();

    }
    else
    {

      throw std::invalid_argument( std::string( "wrong type: not a " ) +
                                   gkg::TypeOf< T >::getName() );

    }

  }
  GKG_CATCH(
    "template < class T > const T& gkg::BaseObject::getValue() const" );

}


template < class T >
inline
T& gkg::BaseObject::getValue()
{

  try
  {

    gkg::TypedObject< T >*
      t = dynamic_cast< TypedObject< T >* >( this );
    if ( t )
    {

      return t->getTypedValue();

    }
    else
    {

      throw std::invalid_argument( std::string( "wrong type: not a " ) +
                                   gkg::TypeOf< T >::getName() );

    }

  }
  GKG_CATCH(
    "template < class T > T& gkg::BaseObject::getValue()" );

}


template < class T >
inline
void gkg::BaseObject::setValue( const T& value )
{

  try
  {

    gkg::TypedObject< T >*
      t = dynamic_cast< TypedObject< T >* >( this );
    if ( !t )
    {

      throw std::invalid_argument( std::string( "wrong type: not a " ) +
                                   gkg::TypeOf< T >::getName() );

    }
    else
    {

      t->getTypedValue() = value;

    }
  }
  GKG_CATCH(
    "template < class T > void gkg::BaseObject::setValue( const T& value )" );

}


template < class T >
inline
void gkg::BaseObject::addAttribute( const std::string& name, const T& value )
{

  try
  {

    gkg::TypedObject< gkg::Dictionary >*
      t = dynamic_cast< gkg::TypedObject< gkg::Dictionary >* >( this );
    if ( t )
    {

      t->getTypedValue()[ name ] = value;

    }
    else
    {

      throw std::invalid_argument( std::string( "wrong type: not a " ) +
                                   gkg::TypeOf< T >::getName() );

    }

  }
  GKG_CATCH(
    "template < class T > void gkg::BaseObject::addAttribute("
    "const std::string& name, const T& value )" );

}


template < class T >
inline
void gkg::BaseObject::getAttribute( const std::string& name, T& value ) const
{

  try
  {

    gkg::GenericObject rcObject = ( *this )[ name ];
    const gkg::TypedObject< T >*
      t = dynamic_cast< const gkg::TypedObject< T >* >( rcObject.get() );
    if ( !t )
    {

      throw std::invalid_argument( std::string( "not a " ) +
                                   gkg::TypeOf< T >::getName() );

    }
    else
    {

      value = t->getTypedValue();

    }

  }
  GKG_CATCH(
    "void gkg::BaseObject::getAttribute( const std::string& name, T& value )"
    " const" );

}


template < class T  >
inline
void gkg::BaseObject::getAttribute( const std::string& name,
                                    std::list< T >& value ) const
{

  try
  {

    gkg::GenericObject rcObject = ( *this )[ name ];
    const gkg::TypedObject< gkg::GenericObjectList >* typedObject =
      dynamic_cast< const gkg::TypedObject< gkg::GenericObjectList >* >(
                                                         rcObject.get() );
    if ( !typedObject )
    {

      const gkg::TypedObject< std::vector< T > >* typedObject =
      dynamic_cast< const gkg::TypedObject< std::list< T > >* >(
                                                               rcObject.get() );
      if ( !typedObject )
      {

        throw std::invalid_argument(
                             "not a GenericObjectList nor a std::vector< T >" );

      }
      else
      {

        value = typedObject->getTypedValue();

      }

    }
    else
    {

      const gkg::GenericObjectList& goList = typedObject->getTypedValue();
    
      std::list< T > sequence;
      gkg::GenericObjectList::const_iterator g = goList.begin(),
                                             ge = goList.end();
      while ( g != ge )
      {

        sequence.push_back( ( *g )->template getValue< T >() );
        ++ g;

      }
      value = sequence;

    }

  }
  GKG_CATCH(
    "void gkg::BaseObject::getAttribute( const std::string& name,"
    " std::list< T >& value ) const" );

}


template < class T  >
inline
void gkg::BaseObject::getAttribute( const std::string& name,
                                    std::vector< T >& value ) const
{

  try
  {

    gkg::GenericObject rcObject = ( *this )[ name ];
    const gkg::TypedObject< gkg::GenericObjectList >* typedObject =
      dynamic_cast< const gkg::TypedObject< gkg::GenericObjectList >* >(
                                                         rcObject.get() );
    if ( !typedObject )
    {

      const gkg::TypedObject< std::vector< T > >* typedObject =
      dynamic_cast< const gkg::TypedObject< std::vector< T > >* >(
                                                               rcObject.get() );
      if ( !typedObject )
      {

        throw std::invalid_argument(
                             "not a GenericObjectList nor a std::vector< T >" );

      }
      else
      {

        value = typedObject->getTypedValue();

      }

    }
    else
    {

      const gkg::GenericObjectList& goList = typedObject->getTypedValue();
    
      std::vector< T > sequence;
      gkg::GenericObjectList::const_iterator g = goList.begin(),
                                             ge = goList.end();
      while ( g != ge )
      {

        sequence.push_back( ( *g )->template getValue< T >() );
        ++ g;

      }
      value = sequence;

    }

  }
  GKG_CATCH(
    "void gkg::BaseObject::getAttribute( const std::string& name,"
    " std::vector< T >& value ) const" );

}


template < class T  >
inline
void gkg::BaseObject::getAttribute( const std::string& name,
                                    std::set< T >& value ) const
{

  try
  {

    gkg::GenericObject rcObject = ( *this )[ name ];
    const gkg::TypedObject< gkg::GenericObjectList >* typedObject =
      dynamic_cast< const gkg::TypedObject< gkg::GenericObjectList >* >(
                                                         rcObject.get() );
    if ( !typedObject )
    {

      const gkg::TypedObject< std::vector< T > >* typedObject =
      dynamic_cast< const gkg::TypedObject< std::set< T > >* >(
                                                               rcObject.get() );
      if ( !typedObject )
      {

        throw std::invalid_argument(
                             "not a GenericObjectList nor a std::vector< T >" );

      }
      else
      {

        value = typedObject->getTypedValue();

      }

    }
    else
    {

      const gkg::GenericObjectList& goList = typedObject->getTypedValue();
    
      std::set< T > sequence;
      gkg::GenericObjectList::const_iterator g = goList.begin(),
                                             ge = goList.end();
      while ( g != ge )
      {

        sequence.push_back( ( *g )->template getValue< T >() );
        ++ g;

      }
      value = sequence;

    }

  }
  GKG_CATCH(
    "void gkg::BaseObject::getAttribute( const std::string& name,"
    " std::set< T >& value ) const" );

}


// we need to overload this function because a gkg::GenericObjectList is
// a vector of gkg::GenericObject, so it would fit with the method
// containing the template vector argument !
namespace gkg
{


template <>
inline
void BaseObject::getAttribute( const std::string& name,
                               GenericObjectList& value ) const
{

  try
  {

    GenericObject rcObject = ( *this )[ name ];
    const TypedObject< GenericObjectList >*
      t = dynamic_cast< const TypedObject< GenericObjectList >* >(
                                                            rcObject.get() );
    if ( !t )
    {

      throw std::invalid_argument( std::string( "not a " ) +
                                      TypeOf< GenericObjectList >::getName() );

    }
    else
    {

      value = t->getTypedValue();

    }

  }
  GKG_CATCH(
    "void gkg::BaseObject::getAttribute( const std::string& name, "
    "gkg::GenericObjectList& value )"
    " const" );

}


}


#endif
