#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-object/SyntaxedTypedObject_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/EofException.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <list>


//
// stream functions
//


namespace gkg
{


static size_t skipWhile( std::istream& is, const std::string& pattern );
static std::string readWhile( std::istream& is, const std::string& pattern );
static std::string readUntil( std::istream& is, const std::string& pattern );
static std::string getLineString( std::istream& is );
static int32_t getLine( std::istream& is );
#if defined (__GNUC__) && (__GNUC__-0 < 3)

static void seek( std::istream& is,
                  streamoff offset,
                  std::ios::seek_dir dir );

#else

static void seek( std::istream& is,
                  std::istream::off_type offset,
                  std::ios_base::seekdir dir );

#endif
static int32_t getCharacter( std::istream& is );
static void ungetCharacter( std::istream& is );
static std::string readString( std::istream& is, char separator, size_t& count);


}


size_t gkg::skipWhile( std::istream& is, const std::string& pattern )
{

  size_t count = 0;
  if ( is.eof() )
  {

    return count;

  }

  int32_t c = gkg::getCharacter( is );
  ++ count;
  while ( ( c != EOF ) && ( pattern.find_first_of( c ) != std::string::npos ) )
  {

    c = gkg::getCharacter( is );
    ++ count;

  }
  if ( c != EOF )
  {

    gkg::ungetCharacter( is );
    -- count;

  }
  return count;

}


std::string gkg::readWhile( std::istream& is, const std::string& pattern )
{

  if ( is.eof() )
  {

    return "";

  }

  std::string token;

  int32_t c = gkg::getCharacter( is );
  while ( ( c != EOF ) && ( pattern.find_first_of( c ) != std::string::npos ) )
  {

    token += static_cast< char >( c );
    c = gkg::getCharacter( is );

  }
  if ( c != EOF )
  {

    gkg::ungetCharacter( is );

  }
  return token;	

}


std::string gkg::readUntil( std::istream& is, const std::string& pattern )
{

  if ( is.eof() )
  {

    return "";

  }

  std::string token;

  int32_t c = gkg::getCharacter( is );
  while ( ( c != EOF ) && ( pattern.find_first_of( c ) == std::string::npos ) )
  {

    token += static_cast< char >( c );
    c = gkg::getCharacter( is );

  }
  if ( c != EOF )
  {

    gkg::ungetCharacter( is );

  }
  return token;	

}


std::string gkg::getLineString( std::istream& is )
{

  return gkg::StringConverter::toString( gkg::getLine( is ) );

}


int32_t gkg::getLine( std::istream& is )
{

  if ( !is )
  {

    return 0;

  }
  gkg::UncommentCounterInputFileStream*
     ucis = dynamic_cast< gkg::UncommentCounterInputFileStream* >( &is );
  if ( !ucis )
  {

    return 0;

  }
  return ucis->getLine();

}


#if defined (__GNUC__) && (__GNUC__-0 < 3)

void gkg::seek( std::istream& is,
                streamoff offset,
                std::ios::seek_dir dir )

#else

void gkg::seek( std::istream& is, 
                std::istream::off_type offset,
                std::ios_base::seekdir dir )

#endif
{

  if ( !is )
  {

    return;

  }
  gkg::UncommentCounterInputFileStream* cuis = 0;
  cuis = dynamic_cast< gkg::UncommentCounterInputFileStream* >( &is );
  if ( cuis )
  {

    cuis->specialSeekg( offset, dir );

  }
  else
  {

    is.seekg( offset, dir );

  }

}


int32_t gkg::getCharacter( std::istream& is )
{

  if ( !is )
  {

    return 0;

  }
  return is.get();

}


void gkg::ungetCharacter( std::istream& is )
{

  if ( !is )
  {

    return;

  }
  is.unget();

}


std::string gkg::readString( std::istream& is, char separator, size_t& count )
{

  char c;
  bool esc = false;
  std::string s;

  do
  {

    if ( !is )
    {

      gkg::EofException();

    }
    c = static_cast< char >( is.get() );
    ++ count;
    if ( esc )
    {

      esc = false;
      s += c;
      c = 0;

    }
    else if ( c == '\\' )
    {

      esc = true;

    }
    else if ( c != separator )
    {

      s += c;

    }

  }
  while ( ( c != separator ) || esc );

  return s;

}


//
// helpers
//


namespace gkg
{

template < class T >
BaseObject* genericHelper( std::istream& is,
                           BaseObject* baseObject,
                           const BaseObjectReader& baseObjectReader,
                           const std::string& semantic );

template < class T >
BaseObject* genericSequenceHelper( std::istream& is,
                                   BaseObject* baseObject,
                                   const BaseObjectReader& baseObjectReader,
                                   const std::string& semantic );

BaseObject* genericGenericObjectListHelper(
                                       std::istream& is,
                                       BaseObject* baseObject,
                                       const BaseObjectReader& baseObjectReader,
                                       const std::string& semantic );

BaseObject* genericDictionaryHelper( std::istream& is,
                                     BaseObject* baseObject,
                                     const BaseObjectReader& baseObjectReader,
                                     const std::string& semantic ); 

BaseObject* genericNoneHelper( std::istream& is,
                               gkg::BaseObject* baseObject,
                               const gkg::BaseObjectReader& baseObjectrReader,
                               const std::string& semantic );

}


template < class T >
gkg::BaseObject* gkg::genericHelper( std::istream& is,
                                     gkg::BaseObject*,
                                     const gkg::BaseObjectReader&,
                                     const std::string& )
{

  T t;
  is >> t;
  return new gkg::TypedObject< T >( t );

}


// specializations
namespace gkg
{


template <>
BaseObject* genericHelper< int8_t >( std::istream& is,
                                     BaseObject*,
                                     const BaseObjectReader&,
                                     const std::string& )
{

  int32_t t;
  is >> t;
  return new TypedObject< int8_t >( ( int8_t )t );

}


template <>
BaseObject* genericHelper< uint8_t >( std::istream& is,
                                      BaseObject*,
                                      const BaseObjectReader&,
                                      const std::string& )
{

  uint32_t t;
  is >> t;
  return new TypedObject< uint8_t >( ( uint8_t )t );

}


template <>
BaseObject* genericHelper< std::string >( std::istream& is,
                                          BaseObject*,
                                          const BaseObjectReader&,
                                          const std::string& )
{

  try
  {

    std::string s;
    char quote;

    readWhile( is, " \t\n\\\r" );
    quote = static_cast< char >( is.get() );
    if ( ( quote != '"' ) && ( quote != '\'' ) )
    {

      throw std::runtime_error( 
                    std::string( "cannot read a string, cannot start with '" ) +
                    quote + "' character, line " + getLineString( is ) );
      return 0;

    }
    size_t count = 0;
    s = readString( is, quote, count );

    return new TypedObject< std::string >( s );

  }
  GKG_CATCH( "template <> gkg::BaseObject* gkg::genericHelper< std::string >( "
             "std::istream& is, gkg::BaseObject*, const gkg::BaseObjectReader&,"
             " const std::string& )" );

}


}


template < class T >
gkg::BaseObject* gkg::genericSequenceHelper(
                                  std::istream& is,
                                  gkg::BaseObject*,
                                  const gkg::BaseObjectReader& baseObjectReader,
                                  const std::string& )
{

  try
  {

    gkg::skipWhile( is, " \t\n\\\r" );

    char c = static_cast< char >( is.get() );

    char mark;
    if ( c == '(' )
    {

      mark = ')';

    }
    else if ( c == '[' )
    {

      mark = ']';

    }
    else
    {

      throw std::runtime_error( std::string( "cannot read a list/tuple" ) +
                                ", line " + gkg::getLineString( is ) );

    }

    gkg::TypedObject< T >* typedObject = new gkg::TypedObject< T >;
    gkg::BaseObject* baseObject;
    gkg::TypedObject< typename T::value_type >* typedObjectValue;

    try
    {

      bool end = false;
      do
      {

        gkg::skipWhile( is, " \t\n\\\r" );
        c = static_cast< char >( is.get() );
        // separator
        if ( c == ',' )
        {

          gkg::skipWhile( is, " \t\n\\\r" );
          c = static_cast< char >( is.get() );

        }
        if ( c == mark )
        {

          end = true;
          break;

        }
        gkg::ungetCharacter( is );
        baseObject = gkg::genericHelper< typename T::value_type >(
                                         is,
                                         typedObject,
                                         baseObjectReader,
                                         "" );
        typedObjectValue = 
        dynamic_cast< gkg::TypedObject< typename T::value_type >* >(
                                                         baseObject );
        if ( typedObjectValue )
        {

          typedObject->getTypedValue().push_back(
                                            typedObjectValue->getTypedValue() );

        }
        else
        {

          delete baseObject;
          throw std::runtime_error(
                               std::string( "in homogeneous sequence, line " ) +
                               gkg::getLineString( is ) );

        }

      }
      while ( !!is );

      return typedObject;

    }
    catch( std::exception & e )
    {

      delete typedObject;
      throw;

    }

  }
  GKG_CATCH( "template < class T > gkg::BaseObject* gkg::genericSequenceHelper("
             " std::istream& is, gkg::BaseObject*, const gkg::BaseObjectReader&"
             " baseObjectReader, const std::string& )" );

}


gkg::BaseObject* gkg::genericGenericObjectListHelper(
                                  std::istream& is,
                                  gkg::BaseObject*,
                                  const gkg::BaseObjectReader& baseObjectReader,
                                  const std::string& )
{

  try
  {

    gkg::skipWhile( is, " \t\n\\\r" );

    char c = static_cast< char >( is.get() );
    char mark;
    if ( c == '(' )
    {

      mark = ')';

    }
    else if ( c == '[' )
    {

      mark = ']';

    }
    else
    {

      throw std::runtime_error(
                               std::string( "cannot not a list/tuple, line " ) +
                               gkg::getLineString( is ) );

    }

    gkg::TypedObject< gkg::GenericObjectList >*
      typedObject = new gkg::TypedObject< gkg::GenericObjectList >;

    try
    {

      do
      {

        gkg::skipWhile( is, " \t\n\\\r" );
        c = static_cast< char >( is.get() );
        // separator
        if ( c == ',' )
        {

          gkg::skipWhile( is, " \t\n\\\r" );
          c = static_cast< char >( is.get() );

        }
        if ( c == mark )
        {

          break;

        }
        gkg::ungetCharacter( is );
        typedObject->getTypedValue().push_back(
                       baseObjectReader.read( is, typedObject, "" ) );

      }
      while ( !!is );
      return typedObject;

    }
    catch( std::exception & e )
    {

      delete typedObject;
      throw;

    }

  }
  GKG_CATCH( "gkg::BaseObject* gkg::genericGenericObjectListHelper( "
             "std::istream& is, gkg::BaseObject*, const gkg::BaseObjectReader& "
             "baseObjectReader, const std::string& )" );

}


gkg::BaseObject* gkg::genericDictionaryHelper(
                                  std::istream& is,
                                  gkg::BaseObject*,
                                  const gkg::BaseObjectReader& baseObjectReader,
                                  const std::string& ) 
{

  try
  {


    try
    {

      gkg::skipWhile( is, " \t\n\\\r" );
      char c = static_cast< char >( is.get() );

      if ( c != '{' )
      {

        throw std::runtime_error(
                         std::string( "not a dictionary, '{' missing, line " ) +
                         gkg::getLineString( is ) );

      }

    }
    catch( std::exception & e )
    {

      throw std::runtime_error( e.what() );

    }

    gkg::TypedObject< gkg::Dictionary >* dictionary = 0;
    std::string syntax;

    if ( baseObjectReader.readSyntax( is, syntax ) )
    {

      dictionary = new gkg::SyntaxedTypedObject< gkg::Dictionary >( syntax );

    }
    else
    {

      dictionary = new gkg::TypedObject< gkg::Dictionary >;

    }

    try
    {

      bool end = false, separator;
      std::string id, type;
      size_t count = 0;
      gkg::BaseObject* baseObject;
      char c;

      do
      {

        gkg::skipWhile( is, " \t\n\\\r" );
        c = static_cast< char >( is.get() );

        // separator
        if ( c == ',' )
        {

          gkg::skipWhile( is, " \t\n\\\r" );
          c = static_cast< char >( is.get() );

        }

        separator = false;
        // expect attribute name
        switch ( c )
        {

          // string identifier
          case '\'':
          case '"':

            id = gkg::readString( is, c, count );
            if ( id.empty() )
            {

              throw std::runtime_error(
                               std::string( "empty identifier, line " ) +
                               gkg::getLineString( is ) );
              break;

            }
            break;

          case '}':

            end = true;
            break;

          default:

            throw std::runtime_error(
                               std::string( "unrecognized tag start '" ) +
                               c + "', line " +
                               gkg::getLineString( is ) );
            end = true;
            break;

        }

        if ( end || !is )
        {

          break;

        }

        // expect ':'
        if ( !separator )
        {

          gkg::skipWhile( is, " \t\n\\\r" );
          c = static_cast< char >( is.get() );
          if ( c != ':' )
          {

            throw std::runtime_error(
                               std::string( "expecting ':', got '" ) +
                               c + "', line " +
                               gkg::getLineString( is ) + ", id: " + id );
            break;

          }

        }

        baseObject = baseObjectReader.read( is, dictionary, id );
        if ( baseObject )
        {

          if ( dictionary->hasSyntax() && ( id == "__syntax__" ) )
          {

            dictionary->setSyntax( baseObject->getValue< std::string >() );

          }
          else
          {

            dictionary->getTypedValue()[ id ] = baseObject;

          }

        }

      }
      while ( !end && !!is );

    }
    catch( std::exception & e )
    {

      delete dictionary;
      throw;

    }
    return dictionary;

  }
  GKG_CATCH( "gkg::BaseObject* gkg::genericDictionaryHelper( "
             "std::istream& is, gkg::BaseObject*, "
             "const gkg::BaseObjectReader& baseObjectReader, "
             "const std::string& )" );

}


gkg::BaseObject* gkg::genericNoneHelper( std::istream&,
                                         gkg::BaseObject*,
                                         const gkg::BaseObjectReader&,
                                         const std::string& )
{

  return 0;

}


//
// BaseObjectReader methods
//

gkg::BaseObjectReader::BaseObjectReader(
                               const gkg::SyntaxSet& rules,
                               const gkg::BaseObjectReader::HelperSet& helpers )
                      : _rules( rules ),
                        _helpers( helpers )
{

  // char type
  if ( _helpers.find( gkg::TypeOf< int8_t >::getName() ) ==
       _helpers.end() )
  {

    _helpers[ gkg::TypeOf< int8_t >::getName() ] =
    &gkg::genericHelper< int8_t >;

  }
  {

    gkg::TypedObject< int8_t> t;
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
    &gkg::genericDictionaryHelper/*< std::string >*/;

  }
  {

    gkg::TypedObject< gkg::Dictionary > t;
    if ( _helpers.find( t.getTypeName() ) == _helpers.end() )
    {

      _helpers[ t.getTypeName() ] =
      &gkg::genericDictionaryHelper/*< std::string >*/;

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

}


gkg::BaseObjectReader::~BaseObjectReader()
{
}


void gkg::BaseObjectReader::read( std::istream& is,
                                  gkg::BaseObject& baseObject ) const
{

  try
  {

    gkg::TypedObject< gkg::Dictionary >* dictionaryOut = 
      dynamic_cast< gkg::TypedObject< gkg::Dictionary >* >( &baseObject );
    if ( !dictionaryOut )
    {

      throw std::runtime_error( 
                 std::string( "cannot read allocated object of type " ) +
                 baseObject.getTypeName() + ", line " +
                 gkg::getLineString( is ) );

    }

    try
    {

      // first we remove 'attributes = '
      readVariableName( is );

      // then we read dictionary
      gkg::BaseObject*
        baseObjectIn = gkg::genericDictionaryHelper( is, 0, *this, "" );

      gkg::TypedObject< gkg::Dictionary >* dictionaryIn = 
        dynamic_cast< gkg::TypedObject< gkg::Dictionary >* >( baseObjectIn );

      if ( !dictionaryIn )
      {

        throw std::runtime_error(
                std::string( "cannot convert to gkg::Dictionary, line " ) +
                gkg::getLineString( is ) );

      }
      *dictionaryOut = *dictionaryIn;
      delete dictionaryIn;

    }
    catch (std::exception& e )
    {

      throw std::runtime_error(
                std::string( "cannot read a gkg::Dictionary, line " ) +
                gkg::getLineString( is ) );

    }

  }
  GKG_CATCH( "void gkg::BaseObjectReader::read( std::istream& is, "
             "gkg::BaseObject& baseObject ) const" );

}


gkg::BaseObject* gkg::BaseObjectReader::read( std::istream& is ) const
{

 try
 {

    readVariableName( is );
    gkg::BaseObject* baseObject = 0;

    try
    {

      baseObject = read( is, 0, "" );

    }
    catch( std::exception& e )
    {

      baseObject = 0;
      throw std::runtime_error( e.what() );

    }
    return baseObject;

  }
  GKG_CATCH(
    "gkg::BaseObject* gkg::BaseObjectReader::read( std::istream& is ) const" );

}


gkg::BaseObject* gkg::BaseObjectReader::read(
                                            std::istream& is,
                                            gkg::BaseObject* baseObject,
                                            const std::string& semantic ) const
{

  try
  {

    if ( is.eof() )
    {

      throw gkg::EofException();

    }

    std::string syntax;
    bool genericFallback = false;

    if ( baseObject && baseObject->hasSyntax() )
    {

      syntax = baseObject->getSyntax();

    }
    if( syntax.empty() )
    {

      syntax = "__generic__";
      genericFallback = true;

    }

    // try to use syntax
    if ( !semantic.empty() )
    {

      // if this semantic attribute does have associated I/O properties
      gkg::SyntaxSet::const_iterator ss = _rules.find( syntax );
      if ( ( ss == _rules.end() ) && !genericFallback )
      {

        ss = _rules.find( "__generic__" );
        genericFallback = true;

      }
      if ( ss != _rules.end() )
      {

        gkg::Syntax::const_iterator property = ss->second.find( semantic );
        if ( ( property == ss->second.end() ) && !genericFallback )
        {

          genericFallback = true;
          gkg::SyntaxSet::const_iterator ss2 = _rules.find( "__generic__" );
          if ( ss2 != _rules.end() )
          {

            ss = ss2;
            property = ss->second.find( semantic );

          }

        }
        if ( property != ss->second.end() )
        {

          // find the associated I/O function
          gkg::BaseObjectReader::HelperSet::const_iterator
            h = _helpers.find( property->second.type );
          if ( h != _helpers.end() )
          {

            // call the associated I/O function
            return ( h->second )( is, baseObject, *this, semantic );

          }

        }

      }

    }

    // not in syntax: try to determine type from contents
    gkg::skipWhile( is, " \t\n\\\r" );
    int32_t c = is.get();
    gkg::ungetCharacter( is );

    std::string type;

    switch ( c )
    {

      // dictionary
      case '{':

        type = "dictionary";
        break;

      // list
      case '[':
      // tuple
      case '(':

        type = "list";
        break;

      // string
      case '\"':	
      case '\'':

        type = gkg::TypeOf< std::string >::getName();
        break;

      // number
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '.':
      case '-':
      case '+':

        type = gkg::TypeOf< double >::getName();
        break;

      case 'N':

        {

          const std::string none = "None";
          size_t count = 0;
          while ( !is.eof() && ( c = is.get() ) == none[ count ] )
          {

            ++ count;

          }
	  if ( count != none.length() )
          {

            gkg::ungetCharacter( is );
            type = "";
            break;

          }
	  type = "None";

        }
        break;

      default:

        type = "";
        break;

    }

    if ( type.empty() )
    {

      throw std::runtime_error( std::string( "unrecognized syntax, " ) +
                                "line " + gkg::getLineString( is ) );

    }

    // now try to find an appropriate helper function
    gkg::BaseObjectReader::HelperSet::const_iterator h = _helpers.find( type );
    if ( h != _helpers.end() )
    {

      // make new rule (should we really do that ?)
      if ( !semantic.empty() && ( syntax != "__generic__" ) )
      {

        gkg::Semantic& rule = _rules[ syntax ][ semantic ];
        rule.type = type;
        rule.needed = false;

      }
      // call the associated I/O function
      return ( h->second )( is, baseObject, *this, semantic );

    }

    // failed
    throw std::runtime_error( std::string( "cannot read attribute " ) +
                              semantic + " of type " + type + ", line " + 
                              gkg::getLineString( is ) );

  }
  GKG_CATCH( "gkg::BaseObject* gkg::BaseObjectReader::read( "
             "std::istream& is, gkg::BaseObject* baseObject, "
             "const std::string& semantic ) const" );


}


std::string gkg::BaseObjectReader::readVariableName( std::istream& is ) const
{

  try
  {

    gkg::skipWhile( is, " \t\n\\\r" );

    // reading variable name
    std::string variableName = gkg::readUntil( is, "= \t\n\\\r" );

    if ( !is )
    {

      throw gkg::EofException();

    }
    gkg::skipWhile( is, " \t\n\\\r" );
    char c = static_cast< char >( is.get() );
    if ( c != '=' )
    {

      throw std::runtime_error( std::string( "unrecognized syntax, line " )+
                                gkg::getLineString( is ) +
                                " (expecting 'variable = ...')" );

    }
    return variableName;

  }
  GKG_CATCH( "std::string gkg::BaseObjectReader::readVariableName( "
             "std::istream& is ) const" );

}


bool gkg::BaseObjectReader::readSyntax( std::istream& is,
                                        std::string& syntax ) const
{
  char c;
  size_t count = 0;
  bool ok = true;

  gkg::skipWhile( is, " \t\n\\\r" );

  c = static_cast< char >( is.get() );
  ++ count;

  if ( ( c != '\'' ) && ( c != '"' ) )
  {

    ok = false;

  }
  else
  {

    std::string attribute = gkg::readString( is, c, count );
    if ( attribute == "__syntax__" )
    {

      count += gkg::skipWhile( is, " \t\n\\\r" );
      c = static_cast< char >( is.get() );
      ++ count;
      if ( c == ':' )
      {

        count += gkg::skipWhile( is, " \t\n\\\r" );
        c = static_cast< char >( is.get() );
        ++ count;
        if ( ( c != '\'' ) && ( c != '"' ) )
        {

          ok = false;

        }
        else
        {

          syntax = gkg::readString( is, c, count );

        }

      }
      else
      {

        ok = false;

      }

    }
    else
    {

      ok = false;

    }

  }

  if ( ok )
  {

    return true;

  }
  gkg::seek( is, - ( int32_t )count, std::ios::cur );

  return false;

}


