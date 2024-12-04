#include <gkg-communication-socket/OStream.h>
#include <gkg-core-cppext/OStream.h>


// provide a constructor for derived classes to use and a constructor
// for public use.

gkg::OStream::OStream()
             : std::ostream( 0 )
{
}


gkg::OStream::OStream( std::streambuf* b )
             : std::ostream( b )
{

  init( b );

}


gkg::OStream::~OStream()
{
}


// redefine these functions to insert binary data where possible for
// faster I/O throughput and to delimit formatted data automatically.
// Binary integral values will occupy the length implied by their type
// and fit the peer's endian architecture.  Formatted integral and
// floating point values will end in a blank character.  Strings will
// end in a null character.  You need not, and should not, insert
// explicit delimiters.

gkg::OStream& gkg::OStream::operator<<( int8_t c )
{

  put( c );           // assume c is 8 bits long on all machines
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( uint8_t uc )
{

  put( uc );          // assume uc is 8 bits long on all machines
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( int16_t s )
{

  if ( _binary )
  {

    gkg::IOStreamBase::INT16 value = ( gkg::IOStreamBase::INT16 )s;
    if ( value != s )
    {

      setstate( std::ios::failbit ); // overflow: number won't fit in 16 bits

    }
    else if ( _swapped )
    {

      gkg::IOStreamBase::INT16 copy = value;
      ( ( char* )&value )[ 0 ] = ( ( char* )&copy )[ 1 ];
      ( ( char* )&value )[ 1 ] = ( ( char* )&copy )[ 0 ];

    }
    width( 0 );
    write( ( char* )&value, sizeof( value ) );

  }
  else
  {

    fixWidth();
    std::ostream::operator<<( s );
    put( ' ' );

  }
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( uint16_t us )
{

  if ( _binary )
  {

    gkg::IOStreamBase::UINT16 value = ( gkg::IOStreamBase::UINT16 )us;
    if ( value != us )
    {

      setstate( std::ios::failbit ); // number won't fit

    }
    else if ( _swapped )
    {

      gkg::IOStreamBase::UINT16 copy = value;
      ( ( char* )&value )[ 0 ] = ( ( char* )&copy )[ 1 ];
      ( ( char* )&value )[ 1 ] = ( ( char* )&copy )[ 0 ];

    }
    width( 0 );
    write( ( char* )&value, sizeof( value ) );

  }
  else
  {

    fixWidth();
    std::ostream::operator<<( us );
    put( ' ' );

  }
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( int32_t i )
{

  if ( _binary )
  {

    gkg::IOStreamBase::INT32 value = ( gkg::IOStreamBase::INT32 )i;
    if ( value != i )
    {

      setstate( std::ios::failbit ); // overflow: number won't fit in 32 bits

    }
    else if ( _swapped )
    {

      gkg::IOStreamBase::INT32 copy = value;
      ( ( char* )&value )[ 0 ] = ( ( char* )&copy )[ 3 ];
      ( ( char* )&value )[ 1 ] = ( ( char* )&copy )[ 2 ];
      ( ( char* )&value )[ 2 ] = ( ( char* )&copy )[ 1 ];
      ( ( char* )&value )[ 3 ] = ( ( char* )&copy )[ 0 ];

    }
    width( 0 );
    write( ( char* )&value, sizeof( value ) );

  }
  else
  {

    fixWidth();
    std::ostream::operator<<( i );
    put( ' ' );

  }
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( uint32_t ui )
{

  if ( _binary )
  {

    gkg::IOStreamBase::UINT32 value = ( gkg::IOStreamBase::UINT32 )ui;
    if ( value != ui )
    {

      setstate( std::ios::failbit ); // overflow: number won't fit in 32 bits

    }
    else if ( _swapped )
    {

      gkg::IOStreamBase::UINT32 copy = value;
      ( ( char* )&value )[ 0 ] = ( ( char* )&copy )[ 3 ];
      ( ( char* )&value )[ 1 ] = ( ( char* )&copy )[ 2 ];
      ( ( char* )&value )[ 2 ] = ( ( char* )&copy )[ 1 ];
      ( ( char* )&value )[ 3 ] = ( ( char* )&copy )[ 0 ];

    }
    width( 0 );
    write( ( char* )&value, sizeof( value ) );

  }
  else
  {

    fixWidth();
    std::ostream::operator<<( ui );
    put( ' ' );

  }
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( int64_t l )
{

  if ( _binary )
  {

    gkg::IOStreamBase::INT64 value = ( gkg::IOStreamBase::INT64 )l;
    if ( value != l )
    {

      setstate( std::ios::failbit ); // overflow: number won't fit in 32 bits

    }
    else if ( _swapped )
    {

      gkg::IOStreamBase::INT64 copy = value;
      ( ( char* )&value )[ 0 ] = ( ( char* )&copy )[ 7 ];
      ( ( char* )&value )[ 1 ] = ( ( char* )&copy )[ 6 ];
      ( ( char* )&value )[ 2 ] = ( ( char* )&copy )[ 5 ];
      ( ( char* )&value )[ 3 ] = ( ( char* )&copy )[ 4 ];
      ( ( char* )&value )[ 4 ] = ( ( char* )&copy )[ 3 ];
      ( ( char* )&value )[ 5 ] = ( ( char* )&copy )[ 2 ];
      ( ( char* )&value )[ 6 ] = ( ( char* )&copy )[ 1 ];
      ( ( char* )&value )[ 7 ] = ( ( char* )&copy )[ 0 ];

    }
    width( 0 );
    write( ( char* )&value, sizeof( value ) );

  }
  else
  {

    fixWidth();
    std::ostream::operator<<( l );
    put( ' ' );

  }
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( uint64_t ul )
{

  if ( _binary )
  {

    gkg::IOStreamBase::UINT64 value = ( gkg::IOStreamBase::UINT64 )ul;
    if ( value != ul )
    {

      setstate( std::ios::failbit ); // overflow: number won't fit in 32 bits

    }
    else if ( _swapped )
    {

      gkg::IOStreamBase::UINT64 copy = value;
      ( ( char* )&value )[ 0 ] = ( ( char* )&copy )[ 7 ];
      ( ( char* )&value )[ 1 ] = ( ( char* )&copy )[ 6 ];
      ( ( char* )&value )[ 2 ] = ( ( char* )&copy )[ 5 ];
      ( ( char* )&value )[ 3 ] = ( ( char* )&copy )[ 4 ];
      ( ( char* )&value )[ 4 ] = ( ( char* )&copy )[ 3 ];
      ( ( char* )&value )[ 5 ] = ( ( char* )&copy )[ 2 ];
      ( ( char* )&value )[ 6 ] = ( ( char* )&copy )[ 1 ];
      ( ( char* )&value )[ 7 ] = ( ( char* )&copy )[ 0 ];

    }
    width( 0 );
    write( ( char* )&value, sizeof( value ) );

  }
  else
  {

    fixWidth();
    std::ostream::operator<<( ul );
    put( ' ' );

  }
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( float f )
{

  fixWidth();
  std::ostream::operator<<( f );
  put( ' ' );
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( double d )
{

  fixWidth();
  std::ostream::operator<<( d );
  put( ' ' );
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( const char* p )
{

  fixWidth();
  gkg::ostream_write_const_char_pointer( *this, p );
  put( '\0' );
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( const unsigned char* up )
{

  fixWidth();
  std::ostream::operator<<( ( const char* )up );
                           // 2.0 ostream omitted unsigned char*
  put( '\0' );
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( const std::string& s )
{

  fixWidth();
  gkg::ostream_write_const_char_pointer( *this, s.c_str() );
  put( '\0' );
  return *this;

}


// Redefine the rest of the overloaded operator<< functions that we
// want to keep in the derived class.

gkg::OStream& 
gkg::OStream::operator<<( std::ostream& ( *f )( std::ostream& ) )
{

  std::ostream::operator<<( f );
  return *this;

}


gkg::OStream& gkg::OStream::operator<<( std::ios& ( *f )( std::ios& ) )
{

  std::ostream::operator<<( f );
  return *this;

}


// decrement the width by one to take into account the delimiter
// character that all the inserters insert after a formatted value.
inline
void gkg::OStream::fixWidth()
{

  int32_t w = ( int32_t )width();
  if ( w )
  {

    width( w - 1 );

  }

}


