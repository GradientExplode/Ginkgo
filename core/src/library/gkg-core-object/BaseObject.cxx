#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-cppext/StringStream.h>
#include <cstdlib>


gkg::BaseObject::BaseObject()
                : gkg::Observable()
{
}


gkg::BaseObject::~BaseObject()
{
}


gkg::GenericObject& gkg::BaseObject::operator [] ( int32_t index )
{

  gkg::TypedObject< gkg::GenericObjectList >*
    t1 = dynamic_cast< gkg::TypedObject< gkg::GenericObjectList >* >( this );
  if ( t1 )
  {

    return t1->getTypedValue()[ index ];

  }
  gkg::TypedObject< gkg::IntDictionary >*
    t2 = dynamic_cast< gkg::TypedObject< gkg::IntDictionary >* >( this );
  if ( t2 )
  {

    return t2->getTypedValue()[ index ];

  }
  gkg::TypedObject< gkg::RCDictionary >*
    t3 = dynamic_cast< gkg::TypedObject< gkg::RCDictionary >* >( this );
  if ( t3 )
  {

    return t3->getTypedValue()[ index ];

  }
  throw std::invalid_argument( "wrong type" );

}


const gkg::GenericObject& gkg::BaseObject::operator [] ( int32_t index ) const
{

  const gkg::TypedObject< gkg::GenericObjectList >*
    t1 = dynamic_cast< const gkg::TypedObject< 
                                  gkg::GenericObjectList >* >( this );
  if ( t1 )
  {

    return t1->getTypedValue()[ index ];

  }
  const gkg::TypedObject< gkg::IntDictionary >*
    t2 = dynamic_cast< const gkg::TypedObject< gkg::IntDictionary >* >( this );
  if ( t2 )
  {

    gkg::IntDictionary::const_iterator i = t2->getTypedValue().find( index );
    if ( i == t2->getTypedValue().end() )
    {

      throw std::range_error( "no such (int) key in dictionary" );

    }
    return i->second;

  }
  const gkg::TypedObject< gkg::RCDictionary >*
    t3 = dynamic_cast< const gkg::TypedObject< gkg::RCDictionary >* >( this );
  if ( t3 )
  {

    gkg::RCDictionary::const_iterator i = t3->getTypedValue().find( index );
    if ( i == t3->getTypedValue().end() )
    {

      throw std::range_error( "no such (int) key in dictionary" );

    }
    return i->second;

  }
  throw std::invalid_argument( "wrong type" );

}


gkg::GenericObject& gkg::BaseObject::operator [] ( const std::string& name )
{

  gkg::TypedObject< gkg::Dictionary >*
    t = dynamic_cast< gkg::TypedObject< gkg::Dictionary  >* >( this );
  if ( t )
  {

    return t->getTypedValue()[ name ];

  }
  throw std::invalid_argument( "wrong_type" );

}


const gkg::GenericObject& 
gkg::BaseObject::operator [] ( const std::string& name ) const
{

  const gkg::TypedObject< gkg::Dictionary >*
    t = dynamic_cast< const gkg::TypedObject< gkg::Dictionary  >* >( this );
  if ( t )
  {

    gkg::Dictionary::const_iterator i = t->getTypedValue().find( name );
    if ( i == t->getTypedValue().end() )
    {

      throw std::range_error( std::string( "no key " ) + name +
                              " in dictionary" );

    }
    return i->second;

  }
  throw std::invalid_argument( "wrong_type" );

}


double gkg::BaseObject::getScalar() const
{

  {

    const gkg::TypedObject< double >*
      t = dynamic_cast< const gkg::TypedObject< double >* >( this );
    if ( t )
    {

      return t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< float >* 
      t = dynamic_cast< const gkg::TypedObject< float >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< int64_t >* 
      t = dynamic_cast< const gkg::TypedObject< int64_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< uint64_t >* 
      t = dynamic_cast< const gkg::TypedObject< uint64_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< int32_t >*
      t = dynamic_cast< const gkg::TypedObject< int32_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< uint32_t >* 
      t = dynamic_cast< const gkg::TypedObject< uint32_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< int16_t >*
      t = dynamic_cast< const gkg::TypedObject< int16_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< uint16_t >*
      t = dynamic_cast< const gkg::TypedObject< uint16_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< int8_t >* 
      t = dynamic_cast< const gkg::TypedObject< int8_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< uint8_t >*
      t = dynamic_cast< const gkg::TypedObject< uint8_t >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< char >* 
      t = dynamic_cast< const gkg::TypedObject< char >* >( this );
    if ( t )
    {

      return ( double )t->getTypedValue();

    }

  }
  {

    const gkg::TypedObject< std::string >*
      t = dynamic_cast< const gkg::TypedObject< std::string >* >( this );
    if ( t )
    {

      char* pend = 0;
      double value = std::strtod( t->getTypedValue().c_str(), &pend );
      if ( pend == t->getTypedValue().c_str() )
      {

        throw std::invalid_argument( "wrong type" );

      }
      return value;

    }

  }
  throw std::invalid_argument( "wrong type" );

}


std::string gkg::BaseObject::getString() const
{

  const gkg::TypedObject< std::string >* 
    t = dynamic_cast< const gkg::TypedObject< std::string >* >( this );
  if ( t )
  {

    return t->getTypedValue();

  }
  std::ostringstream os;
  os << getScalar();
  return os.str();

}


void gkg::BaseObject::copyAttributes( const gkg::BaseObject& other )
{

  gkg::TypedObject< gkg::Dictionary >*
    t1 = dynamic_cast< gkg::TypedObject< gkg::Dictionary >* >( this );
  if ( !t1 )
  {

    throw std::invalid_argument( "wrong type" );

  }
  const gkg::TypedObject< gkg::Dictionary >*
    t2 = dynamic_cast< const gkg::TypedObject< gkg::Dictionary >* >( &other );
  if ( !t2 )
  {

    throw std::invalid_argument( "wrong type" );

  }
  gkg::Dictionary::const_iterator i = t2->getTypedValue().begin(),
                                  ie = t2->getTypedValue().end();
  while ( i != ie )
  {

    t1->getTypedValue()[ i->first ] = i->second->clone();
    ++ i;

  }

}


void gkg::BaseObject::removeAttribute( const std::string& name )
{

  gkg::TypedObject< gkg::Dictionary >*
    t = dynamic_cast< gkg::TypedObject< gkg::Dictionary >* >( this );
  if ( t )
  {

    t->getTypedValue().erase( name );

  }

}


void gkg::BaseObject::removeAttributes()
{

  gkg::TypedObject< gkg::Dictionary >*
    t = dynamic_cast< gkg::TypedObject< gkg::Dictionary >* >( this );
  if ( !t )
  {

    t->getTypedValue().clear();

  }

}


bool gkg::BaseObject::hasAttribute( const std::string& name ) const
{

  const gkg::TypedObject< gkg::Dictionary >*
    t = dynamic_cast< const gkg::TypedObject< gkg::Dictionary >* >( this );
  if ( t )
  {

    gkg::Dictionary::const_iterator i = t->getTypedValue().find( name );
    if ( i != t->getTypedValue().end() )
    {

      return true;

    }

  }
  return false;

}


std::set< std::string > gkg::BaseObject::getAttributes() const
{

  const gkg::TypedObject< gkg::Dictionary >*
    t = dynamic_cast< const gkg::TypedObject< gkg::Dictionary >* >( this );
  if ( !t )
  {

    throw std::invalid_argument( "wrong type" );

  }
  std::set< std::string > attributes;
  gkg::Dictionary::const_iterator i = t->getTypedValue().begin(),
                                  ie = t->getTypedValue().end();
  while ( i != ie )
  {

    attributes.insert( i->first );
    ++ i;

  }
  return attributes;

}


bool gkg::BaseObject::hasSyntax() const
{

  return false;

}


std::string gkg::BaseObject::getSyntax() const
{

  return "";

}


void gkg::BaseObject::setSyntax( const std::string& )
{
}


bool gkg::BaseObject::check( const gkg::SyntaxSet&,
                             std::set< std::string >& ) const
{

  return true;

}


