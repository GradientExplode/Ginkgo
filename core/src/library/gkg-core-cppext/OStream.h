#ifndef _gkg_core_cppext_OStream_h_
#define _gkg_core_cppext_OStream_h_


#include <iostream>
#include <string>
#include <cstdio>


namespace gkg
{


template < class T >
inline
void ostream_out_form( std::ostream& os,
                       const std::string& format,
                       T value )
{

#if __GNUC__

  char buffer[ 10000 ];
  ::snprintf( buffer, 10000, format.c_str(), value );
  os << buffer;

#else

  os.form( format, value );

#endif

}


inline
void ostream_write_hex_byte( std::ostream& os, int theByte )
{

#ifdef __GNUC__

  ostream_out_form( os, "%02x", theByte );

#else

  os << ( ( theByte >> 4 ) & 0x0f ) << ( theByte & 0x0f );

#endif

}


inline
void ostream_write_oct_byte( std::ostream& os, int theByte )
{

#ifdef __GNUC__

  ostream_out_form( os, "%03o", ( unsigned int )theByte );

#else

  os << theByte;

#endif

}


#ifdef __GNUC__


inline
int ostream_set_hex_mode( std::ostream&, int, char = 0, char* = 0 )
{

  return 0;

}


inline
int ostream_set_dec_mode( std::ostream&, int, char = 0, char* = 0 )
{

  return 0;

}


inline
int ostream_set_oct_mode( std::ostream&, int, char = 0, char* = 0 )
{

  return 0;

}


#else 

inline
int ostream_set_hex_mode( std::ostream& os, int width, char fill = 0,
                          char* oldFill = 0 )
{


  int oldWidth = os.width( width );
  if ( oldFill )
    *oldFill = os.fill( fill );
  os << hex;
  return oldWidth;


}


inline
int ostream_set_dec_mode( std::ostream& os, int width, char fill = 0,
                          char* oldFill = 0 )
{

  int oldWidth = os.width( width );
  if ( oldFill )
    *oldFill = os.fill( fill );
  os << dec;
  return oldWidth;

}


inline
int ostream_set_oct_mode( std::ostream& os, int width, char fill = 0,
                          char* oldFill = 0 )
{

  int oldWidth = os.width( width );
  if ( oldFill )
    *oldFill = os.fill( fill );
  os << oct;
  return oldWidth;

}


#endif


inline
void ostream_write_const_char_pointer( std::ostream& os, const char* s )
{

#if __GNUC__

#if __GNUC__ >= 3

  std::operator<<( os, s );

#else

  os.operator<<( s );

#endif

#else     // non GNU compiler(s)

  os.operator<<( s );

#endif

}


}


#endif
