#include <gkg-communication-socket/IStream.h>


// Provide a constructor for derived classes to use and a constructor
// for public use.

gkg::IStream::IStream()
             : std::istream( 0 )
{
}


gkg::IStream::IStream( std::streambuf* b )
             : std::istream( b )
{

  init( b );

}


gkg::IStream::~IStream()
{
}


// redefine these functions to extract binary data where possible for
// faster I/O throughput and to discard delimiters automatically.
// Binary gkg::IOStreamBase::INTegral values occupy the length implied by their type and
// already fit this machine's endian architecture.  Formatted gkg::IOStreamBase::INTegral
// and floating pogkg::IOStreamBase::INT values end in a blank character that is
// extracted.  Strings end in a null character that is extracted.

gkg::IStream& gkg::IStream::operator>>( int8_t& c )
{

  get( *( ( char* )&c ) );   // assume c is 8 bits long on all machines
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( uint8_t& uc )
{

  get( *( ( char* )&uc ) );  // assume uc is 8 bits long on all machines
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( int16_t& s )
{

  if ( _binary )
  {

    gkg::IOStreamBase::INT16 value;
    read( ( char* )&value, sizeof( value ) );
    if ( good() )
    {

      s = ( int16_t )value;
      if ( s != value )
      {

        setstate( std::ios::failbit ); // overflow: value won't fit in short

      }

    }

  }
  else
  {

    std::istream::operator>>( s );
    get();

  }
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( uint16_t& us )
{

  if ( _binary )
  {

    gkg::IOStreamBase::UINT16 value;
    read( ( char* )&value, sizeof( value ) );
    if ( good() )
    {

      us = ( uint16_t )value;
      if ( us != value )
      {

        setstate( std::ios::failbit ); // overflow: value won't fit in ushort

      }

    }

  }
  else
  {

    std::istream::operator>>( us );
    get();

  }
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( int32_t& i )
{

  if ( _binary )
  {

    gkg::IOStreamBase::INT32 value = 0;
    read( ( char* )&value, sizeof( value ) );
    if ( good() )
    {

      i = ( int32_t )value;
      if ( i != value )
      {

        setstate( std::ios::failbit ); // overflow: value won't fit in int

      }

    }

  }
  else
  {

    std::istream::operator>>( i );
    get();

  }
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( uint32_t& ui )
{

  if ( _binary )
  {

    gkg::IOStreamBase::UINT32 value;
    read( ( char* )&value, sizeof( value ) );
    if ( good() )
    {

      ui = ( uint32_t )value;
      if ( ui != value )
      {

        setstate( std::ios::failbit ); // overflow: value won't fit in uint

      }

    }

  }
  else
  {

    std::istream::operator>>( ui );
    get();

  }
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( int64_t& l )
{

  if ( _binary )
  {

    gkg::IOStreamBase::INT64 value;
    read( ( char* )&value, sizeof( value ) );
    if ( good() )
    {

      l = (int64_t)value;
      if ( l != value )
      {

        setstate( std::ios::failbit ); // overflow: value won't fit in long

      }

    }

  }
  else
  {

    std::istream::operator>>( l );
    get();

  }
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( uint64_t& ul )
{

  if ( _binary )
  {

    gkg::IOStreamBase::UINT64 value;
    read( ( char* )&value, sizeof( value ) );
    if ( good() )
    {

      ul = ( uint64_t )value;
      if ( ul != value )
      {

        setstate( std::ios::failbit ); // overflow: value won't fit in ulong

      }

    }

  }
  else
  {

    std::istream::operator>>( ul );
    get();

  }
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( float& f )
{

  std::istream::operator>>( f );
  get();
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( double& d )
{

  std::istream::operator>>( d );
  get();
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( char* p )
{

  const int32_t MAXINT = ( int32_t )( ( ( uint32_t )-1 ) >> 1 );
  const int32_t w = ( int32_t )width( 0 );
  getline( p, w ? w : MAXINT, '\0' );
  if ( w && ( ( int32_t )gcount() == w - 1 ) )
  {

    setstate( std::ios::failbit );

  }

  return *this;

}


gkg::IStream& gkg::IStream::operator>>( signed char* p )
{

  const int32_t MAXINT = ( int32_t )( ( ( uint32_t )-1 ) >> 1 );
  const int32_t w = ( int32_t )width( 0 );
  getline( ( char* )p, w ? w : MAXINT, '\0' );
  if ( w && ( ( int32_t )gcount() == w - 1 ) )
  {

    setstate( std::ios::failbit );

  }

  return *this;

}


gkg::IStream& gkg::IStream::operator>>( unsigned char* up )
{

  const int32_t MAXINT = ( int32_t )( ( ( uint32_t )-1 ) >> 1 );
  const int32_t w = ( int32_t )width( 0 );
  getline( ( char* )up, w ? w : MAXINT, '\0' );
  if ( w && ( ( int32_t )gcount() == w - 1 ) )
  {

    setstate( std::ios::failbit );

  }

  return *this;

}


// redefine the rest of the overloaded operator>> functions that we
// want to keep in the derived class.
gkg::IStream& 
gkg::IStream::operator>>( std::istream& ( *f )( std::istream& ) )
{

  std::istream::operator>>( f );
  return *this;

}


gkg::IStream& gkg::IStream::operator>>( std::ios& ( *f )( std::ios& ) )
{

  std::istream::operator>>( f );
  return *this;

}

