#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <list>


//
// helpers
//


namespace gkg
{


std::string quoteString( const std::string s );


template < class T >
void genericHelper( std::ostream& os,
                    const BaseObject& baseObject,
                    const BaseObjectWriter&,
                    int32_t );

template < class T >
void genericSequenceHelper( std::ostream& os,
                            const BaseObject& baseObject,
                            const BaseObjectWriter& baseObjectWriter,
                            int32_t indentationLevel );

template < class T >
void genericDictionaryHelper( std::ostream& os,
                              const BaseObject& baseObject,
                              const BaseObjectWriter& baseObjectWriter,
                              int32_t indentationLevel );


void genericRCDictionaryHelper( std::ostream& os,
                                const BaseObject& baseObject,
                                const BaseObjectWriter& baseObjectWriter,
                                int32_t indentationLevel );

void genericGenericObjectListHelper( std::ostream& os,
                                     const BaseObject& baseObject,
                                     const BaseObjectWriter& baseObjectWriter,
                                     int32_t indentationLevel );

}


std::string gkg::quoteString( std::string s )
{

  size_t position = s.rfind( '\'' );
  while ( position != std::string::npos )
  {

    // replace ' by \'
    s.insert( position, "\\" );
    position = s.rfind( '\'', position );

  }
  return std::string( "\'" ) + s + "\'";

}



template < class T >
void gkg::genericHelper( std::ostream& os,
                         const gkg::BaseObject& baseObject,
                         const gkg::BaseObjectWriter&,
                         int32_t )
{

  os << baseObject.gkg::BaseObject::getValue< T >();

}


// specializations
namespace gkg
{


template <>
void genericHelper< int8_t >( std::ostream& os,
                              const BaseObject& baseObject,
                              const BaseObjectWriter&,
                              int32_t )
{

  os << ( int32_t )baseObject.BaseObject::getValue< int8_t >();

}


template <>
void genericHelper< uint8_t >( std::ostream& os,
                               const BaseObject& baseObject,
                               const BaseObjectWriter&,
                               int32_t )
{

  os << ( uint32_t )baseObject.BaseObject::getValue< uint8_t >();

}


template <>
void genericHelper< std::string >( std::ostream& os,
                                   const BaseObject& baseObject,
                                   const BaseObjectWriter&,
                                   int32_t )
{

  os << quoteString( baseObject.BaseObject::getValue< std::string >() );

}


}


template < class T >
void gkg::genericSequenceHelper( std::ostream& os,
                                 const gkg::BaseObject& baseObject,
                                 const gkg::BaseObjectWriter& baseObjectWriter,
                                 int32_t indentationLevel )
{

  const T& t = baseObject.gkg::BaseObject::getValue< T >();
  bool first = true;
  gkg::TypedObject< typename T::value_type > item;

  os << "[";
  typename T::const_iterator i = t.begin(), ie = t.end();
  while ( i != ie )
  {

    if ( first )
    {

      first = false;
      os << ' ';

    }
    else
    {

      os << ", ";

    }
    item.setValue( *i );
    genericHelper< typename T::value_type >( os,
                                             item,
                                             baseObjectWriter,
                                             indentationLevel ); 
    ++ i;

  }
  os << " ]";

}


template < class T >
void gkg::genericDictionaryHelper(
                                  std::ostream& os,
                                  const gkg::BaseObject& baseObject,
                                  const gkg::BaseObjectWriter& baseObjectWriter,
                                  int32_t indentationLevel )
{

  char separator;
  std::string indentation;

  if ( baseObjectWriter.getSingleLine() )
  {

    separator = ' ';

  }
  else
  {

    separator = '\n';
    for ( int32_t i = 0; i < indentationLevel; i++ )
    {

      indentation += "    ";

    }
    ++ indentationLevel;

  }

  os << "{" << separator << indentation;

  bool hasSyntax = false;
  std::string syntax;
  if ( baseObject.hasSyntax() )
  {

    hasSyntax = true;
    syntax = baseObject.getSyntax();

  }
  else
  {

    try
    {

      baseObject.getAttribute( "__syntax__", syntax );
      hasSyntax = true;

    }
    catch ( std::exception& )
    {

      hasSyntax = false;

    }

  }

  bool first = true;
  if ( hasSyntax )
  {

    os << "'__syntax__' : ";
    os << quoteString( syntax );
    first = false;

  }

  const std::map< T, gkg::GenericObject >& m = 
    baseObject.gkg::BaseObject::getValue< std::map< T, gkg::GenericObject > >(); 
  typename std::map< T, gkg::GenericObject >::const_iterator g = m.begin(),
                                                             ge = m.end();
  while ( g != ge )
  {

    if ( first )
    {

      first = false;

    }
    else
    {

      os << "," << separator << indentation;

    }
    baseObjectWriter.write( os,
                            gkg::TypedObject< T >( g->first ),
                            indentationLevel,
                            "",
                            "" );
    os << " : ";
    baseObjectWriter.write( os,
                            *g->second,
                            indentationLevel,
                            "",
                            "" );
    ++ g;

  }
  os << separator << indentation << "}";

}


// specialization
namespace gkg
{


template <>
void genericDictionaryHelper< std::string >(
                                       std::ostream& os,
                                       const BaseObject& baseObject,
                                       const BaseObjectWriter& baseObjectWriter,
                                       int32_t indentationLevel )
{

  char separator;
  std::string indentation;

  if ( baseObjectWriter.getSingleLine() )
  {

    separator = ' ';

  }
  else
  {

    separator = '\n';
    for ( int32_t i = 0; i < indentationLevel; i++ )
    {

      indentation += "    ";

    }
    ++ indentationLevel;

  }

  os << "{" << separator << indentation;

  bool hasSyntax = false;
  std::string syntax;
  if ( baseObject.hasSyntax() )
  {

    hasSyntax = true;
    syntax = baseObject.getSyntax();

  }
  else
  {

    try
    {

      baseObject.getAttribute( "__syntax__", syntax );
      hasSyntax = true;

    }
    catch ( std::exception& )
    {

      hasSyntax = false;

    }

  }

  bool first = true;
  if ( hasSyntax )
  {

    os << "'__syntax__' : ";
    os << quoteString( syntax );
    first = false;

  }

  const Dictionary& d = baseObject.BaseObject::getValue< Dictionary >(); 
  Dictionary::const_iterator g = d.begin(),
                             ge = d.end();
  while ( g != ge )
  {

    if ( ( baseObjectWriter.getWriteInternals() ||
           !baseObjectWriter.isInternal( "", g->first ) ) &&
         ( ( g->first != "__syntax__" ) || !hasSyntax ) )  
    {

      if ( first )
      {

        first = false;

      }
      else
      {

        os << "," << separator << indentation;

      }
      os << quoteString( g->first );
      os << " : ";
      baseObjectWriter.write( os,
                              *g->second,
                              indentationLevel,
                              "",
                              "" );

    }
    ++ g;

  }
  os << separator << indentation << "}";

}


}


void gkg::genericRCDictionaryHelper(
                                  std::ostream& os,
                                  const gkg::BaseObject& baseObject,
                                  const gkg::BaseObjectWriter& baseObjectWriter,
                                  int32_t indentationLevel )
{

  char separator;
  std::string indentation;

  if ( baseObjectWriter.getSingleLine() )
  {

    separator = ' ';

  }
  else
  {

    separator = '\n';
    for ( int32_t i = 0; i < indentationLevel; i++ )
    {

      indentation += "    ";

    }
    ++ indentationLevel;

  }

  os << "{" << separator << indentation;

  bool hasSyntax = false;
  std::string syntax;
  if ( baseObject.hasSyntax() )
  {

    hasSyntax = true;
    syntax = baseObject.getSyntax();

  }
  else
  {

    try
    {

      baseObject.getAttribute( "__syntax__", syntax );
      hasSyntax = true;

    }
    catch ( std::exception& )
    {

      hasSyntax = false;

    }

  }

  bool first = true;
  if ( hasSyntax )
  {

    os << "'__syntax__' : ";
    os << quoteString( syntax );
    first = false;

  }

  const gkg::RCDictionary& r = 
    baseObject.gkg::BaseObject::getValue< gkg::RCDictionary >(); 
  gkg::RCDictionary::const_iterator g = r.begin(),
                                    ge = r.end();
  while ( g != ge )
  {

    if ( first )
    {

      first = false;

    }
    else
    {

      os << "," << separator << indentation;

    }
    baseObjectWriter.write( os,
                            *g->first,
                            indentationLevel,
                            "",
                            "" );
    os << " : ";
    baseObjectWriter.write( os,
                            *g->second,
                            indentationLevel,
                            "",
                            "" );
    ++ g;

  }
  os << separator << indentation << "}";

}


void gkg::genericGenericObjectListHelper(
                                  std::ostream& os,
                                  const gkg::BaseObject& baseObject,
                                  const gkg::BaseObjectWriter& baseObjectWriter,
                                  int32_t indentationLevel )
{

  ++ indentationLevel;

  os << "[";

  bool first = true;

  const gkg::GenericObjectList& l = 
    baseObject.gkg::BaseObject::getValue< gkg::GenericObjectList >(); 
  gkg::GenericObjectList::const_iterator g = l.begin(),
                                         ge = l.end();
  while ( g != ge )
  {

    if ( first )
    {

      first = false;

    }
    else
    {

      os << ',';

    }
    os << ' ';
    baseObjectWriter.write( os,
                            **g,
                            indentationLevel,
                            "",
                            "" );
    ++ g;

  }
  os << " ]";

}


//
// BaseObjectWriter methods
//

gkg::BaseObjectWriter::BaseObjectWriter(
                                const gkg::SyntaxSet& rules,
                                const gkg::BaseObjectWriter::HelperSet& helpers,
                                bool writeInternals,
                                bool singleLine )
                      : _rules( rules ),
                        _helpers( helpers ),
                        _writeInternals( writeInternals ),
                        _singleLine( singleLine )
{

  // char type
  if ( _helpers.find( gkg::TypeOf< int8_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< int8_t >::getName() ] =
    &gkg::genericHelper< int8_t >;

  }
  {

    gkg::TypedObject< int8_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< int8_t >;

    }

  }

  // unsigned char type
  if ( _helpers.find( gkg::TypeOf< uint8_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< uint8_t >::getName() ] =
    &gkg::genericHelper< uint8_t >;

  }
  {

    gkg::TypedObject< uint8_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< uint8_t >;

    }

  }

  // short type
  if ( _helpers.find( gkg::TypeOf< int16_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< int16_t >::getName() ] =
    &gkg::genericHelper< int16_t >;

  }
  {

    gkg::TypedObject< int16_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< int16_t >;

    }

  }

  // unsigned short type
  if ( _helpers.find( gkg::TypeOf< uint16_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< uint16_t >::getName() ] =
    &gkg::genericHelper< uint16_t >;

  }
  {

    gkg::TypedObject< uint16_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< uint16_t >;

    }

  }

  // int type
  if ( _helpers.find( gkg::TypeOf< int32_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< int32_t >::getName() ] =
    &gkg::genericHelper< int32_t >;

  }
  {

    gkg::TypedObject< int32_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< int32_t >;

    }

  }

  // unsigned int type
  if ( _helpers.find( gkg::TypeOf< uint32_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< uint32_t >::getName() ] =
    &gkg::genericHelper< uint32_t >;

  }
  {

    gkg::TypedObject< uint32_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< uint32_t >;

    }

  }

  // long type
  if ( _helpers.find( gkg::TypeOf< int64_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< int64_t >::getName() ] =
    &gkg::genericHelper< int64_t >;

  }
  {

    gkg::TypedObject< int64_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< int64_t >;

    }

  }

  // unsigned long type
  if ( _helpers.find( gkg::TypeOf< uint64_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< uint64_t >::getName() ] =
    &gkg::genericHelper< uint64_t >;

  }
  {

    gkg::TypedObject< uint64_t > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< uint64_t >;

    }

  }

  // float type
  if ( _helpers.find( gkg::TypeOf< float >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< float >::getName() ] =
    &gkg::genericHelper< float >;

  }
  {

    gkg::TypedObject< float > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< float >;

    }

  }

  // double type
  if ( _helpers.find( gkg::TypeOf< double >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< double >::getName() ] =
    &gkg::genericHelper< double >;

  }
  {

    gkg::TypedObject< double > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< double >;

    }

  }

  // bool type
  if ( _helpers.find( gkg::TypeOf< bool >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< bool >::getName() ] =
    &gkg::genericHelper< bool >;

  }
  {

    gkg::TypedObject< bool > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< bool >;

    }

  }

  // std::string type
  if ( _helpers.find( gkg::TypeOf< std::string >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::string >::getName() ] =
    &gkg::genericHelper< std::string >;

  }
  {

    gkg::TypedObject< std::string > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericHelper< std::string >;

    }

  }

  // Dictionary type
  if ( _helpers.find( "dictionary" ) ==
       _helpers.end() )
  {

    _helpers[ "dictionary" ] =
    &gkg::genericDictionaryHelper< std::string >;

  }
  {

    gkg::TypedObject< gkg::Dictionary > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericDictionaryHelper< std::string >;

    }

  }

  // IntDictionary type
  if ( _helpers.find( "int_dictionary" ) ==
       _helpers.end() )
  {

    _helpers[ "int_dictionary" ] =
    &gkg::genericDictionaryHelper< int32_t >;

  }
  {

    gkg::TypedObject< gkg::IntDictionary > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericDictionaryHelper< int32_t >;

    }

  }

  // FloatDictionary type
  if ( _helpers.find( "float_dictionary" ) ==
       _helpers.end() )
  {

    _helpers[ "float_dictionary" ] =
    &gkg::genericDictionaryHelper< float >;

  }
  {

    gkg::TypedObject< gkg::FloatDictionary > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericDictionaryHelper< float >;

    }

  }

  // DoubleDictionary type
  if ( _helpers.find( "double_dictionary" ) ==
       _helpers.end() )
  {

    _helpers[ "double_dictionary" ] =
    &gkg::genericDictionaryHelper< double >;

  }
  {

    gkg::TypedObject< gkg::DoubleDictionary > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericDictionaryHelper< double >;

    }

  }

  // RCDictionary type
  if ( _helpers.find( "rc_dictionary" ) ==
       _helpers.end() )
  {

    _helpers[ "rc_dictionary" ] =
    &gkg::genericRCDictionaryHelper;

  }
  {

    gkg::TypedObject< gkg::RCDictionary > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericRCDictionaryHelper;

    }

  }

  // GenericObjectList type
  if ( _helpers.find( "list" ) ==
       _helpers.end() )
  {

    _helpers[ "list" ] =
    &gkg::genericGenericObjectListHelper;

  }
  {

    gkg::TypedObject< gkg::GenericObjectList > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericGenericObjectListHelper;

    }

  }

  // std::vector< short > type
  if ( _helpers.find( gkg::TypeOf< std::vector< int16_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< int16_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< int16_t > >;

  }
  {

    gkg::TypedObject< std::vector< int16_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< int16_t > >;

    }

  }

  // std::vector< unsigned short > type
  if ( _helpers.find( gkg::TypeOf< std::vector< uint16_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< uint16_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< uint16_t > >;

  }
  {

    gkg::TypedObject< std::vector< uint16_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< uint16_t > >;

    }

  }

  // std::vector< int > type
  if ( _helpers.find( gkg::TypeOf< std::vector< int32_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< int32_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< int32_t > >;

  }
  {

    gkg::TypedObject< std::vector< int32_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< int32_t > >;

    }

  }

  // std::vector< unsigned int > type
  if ( _helpers.find( gkg::TypeOf< std::vector< uint32_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector<  uint32_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector<  uint32_t > >;

  }
  {

    gkg::TypedObject< std::vector<  uint32_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< uint32_t > >;

    }

  }

  // std::vector< int > type
  if ( _helpers.find( gkg::TypeOf< std::vector< int64_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< int64_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< int64_t > >;

  }
  {

    gkg::TypedObject< std::vector< int64_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< int64_t > >;

    }

  }

  // std::vector< unsigned int > type
  if ( _helpers.find( gkg::TypeOf< std::vector< uint64_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< uint64_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< uint64_t > >;

  }
  {

    gkg::TypedObject< std::vector< uint64_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< uint64_t > >;

    }

  }

  // std::vector< float > type
  if ( _helpers.find( gkg::TypeOf< std::vector< float > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< float > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< float > >;

  }
  {

    gkg::TypedObject< std::vector< float > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< float > >;

    }

  }

  // std::vector< double > type
  if ( _helpers.find( gkg::TypeOf< std::vector< double > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< double > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< double > >;

  }
  {

    gkg::TypedObject< std::vector< double > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< double > >;

    }

  }

  // std::vector< std::string > type
  if ( _helpers.find( gkg::TypeOf< std::vector< std::string > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::vector< std::string > >::getName() ] =
    &gkg::genericSequenceHelper< std::vector< std::string > >;

  }
  {

    gkg::TypedObject< std::vector< std::string > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::vector< std::string > >;

    }

  }

  // std::list< short > type
  if ( _helpers.find( gkg::TypeOf< std::list< int16_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< int16_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< int16_t > >;

  }
  {

    gkg::TypedObject< std::list< int16_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< int16_t > >;

    }

  }

  // std::list< unsigned short > type
  if ( _helpers.find( gkg::TypeOf< std::list< uint16_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< uint16_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< uint16_t > >;

  }
  {

    gkg::TypedObject< std::list< uint16_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< uint16_t > >;

    }

  }

  // std::list< int > type
  if ( _helpers.find( gkg::TypeOf< std::list< int32_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< int32_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< int32_t > >;

  }
  {

    gkg::TypedObject< std::list< int32_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< int32_t > >;

    }

  }

  // std::list< unsigned int > type
  if ( _helpers.find( gkg::TypeOf< std::list< uint32_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< uint32_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< uint32_t > >;

  }
  {

    gkg::TypedObject< std::list< uint32_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< uint32_t > >;

    }

  }

  // std::list< long > type
  if ( _helpers.find( gkg::TypeOf< std::list< int64_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< int64_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< int64_t > >;

  }
  {

    gkg::TypedObject< std::list< int64_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< int64_t > >;

    }

  }

  // std::list< unsigned int > type
  if ( _helpers.find( gkg::TypeOf< std::list< uint64_t > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< uint64_t > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< uint64_t > >;

  }
  {

    gkg::TypedObject< std::list< uint64_t > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< uint64_t > >;

    }

  }

  // std::list< float > type
  if ( _helpers.find( gkg::TypeOf< std::list< float > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< float > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< float > >;

  }
  {

    gkg::TypedObject< std::list< float > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< float > >;

    }

  }

  // std::list< double > type
  if ( _helpers.find( gkg::TypeOf< std::list< double > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< double > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< double > >;

  }
  {

    gkg::TypedObject< std::list< double > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< double > >;

    }

  }

  // std::list< std::string > type
  if ( _helpers.find( gkg::TypeOf< std::list< std::string > >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< std::list< std::string > >::getName() ] =
    &gkg::genericSequenceHelper< std::list< std::string > >;

  }
  {

    gkg::TypedObject< std::list< std::string > > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericSequenceHelper< std::list< std::string > >;

    }

  }

}


gkg::BaseObjectWriter::~BaseObjectWriter()
{


}


bool gkg::BaseObjectWriter::getWriteInternals() const
{

  return _writeInternals;

}


bool gkg::BaseObjectWriter::getSingleLine() const
{

  return _singleLine;

}


bool gkg::BaseObjectWriter::isInternal( const std::string& syntax,
                                        const std::string& semantic ) const
{

  if ( !semantic.empty() )
  {

    gkg::SyntaxSet::const_iterator ss;
    if ( syntax.empty() )
    {

      ss = _rules.find( "__generic__" );

    }
    else
    {

      ss = _rules.find( syntax );

    }
    if ( ss != _rules.end() )
    {

      gkg::Syntax::const_iterator s = ss->second.find( semantic );
      if ( s != ss->second.end() && s->second.internal )
      {

        return true;

      }

    }

  }
  return false;

}


void gkg::BaseObjectWriter::write( std::ostream& os,
                                   const gkg::BaseObject& baseObject ) const
{

  os << "attributes = "; 
  write( os, baseObject, 1, "", "" );
  os << std::endl;

}


void gkg::BaseObjectWriter::write( std::ostream& os,
                                   const BaseObject& baseObject,
                                   int32_t indentationLevel,
                                   const std::string & syntax, 
                                   const std::string & semantic ) const
{

  // Remark : this will never happen since baseObject is a reference,
  //          an instance of the class will always exist hence the pointer
  //          to baseObject will never be null.
  // "None" object
  //if ( &baseObject == 0 )
  //{

  //  os << "None";
  //  return;

  //}

  std::string typeName = baseObject.getTypeName();

  // try to find type in syntax if semantic / syntaxic atts are provided
  if ( !semantic.empty() )
  {

    gkg::SyntaxSet::const_iterator ss;
    if ( syntax.empty() )
    {

      ss = _rules.find( "__generic__" );

    }
    else
    {

      ss = _rules.find( syntax );

    }
    if ( ss != _rules.end() )
    {

      gkg::Syntax::const_iterator s = ss->second.find( semantic );
      if ( s != ss->second.end() )
      {

        typeName = s->second.type;

      }

    }

  }

  // try to find type in helpers
  gkg::BaseObjectWriter::HelperSet::const_iterator
    h = _helpers.find( typeName );
  if ( h != _helpers.end() )
  {

    ( h->second )( os, baseObject, *this, indentationLevel );
    return;

  }

  // no helper found
  throw std::runtime_error( std::string( "cannot write data of type " ) +
                            typeName );

}

