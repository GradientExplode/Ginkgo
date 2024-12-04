#include <gkg-core-io/StringConverter.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <sstream>
#include <climits>
#include <cfloat>


std::string gkg::StringConverter::toPureAscii( const std::string& msg )
{

  std::string asciiMsg = msg;

  size_t i;
  for ( i = 0; i < asciiMsg.length(); i++ )
  {

    // e accent aigu
    if ( ( int32_t )asciiMsg[ i ] == -23 )
    {

      asciiMsg[ i ] = 'e';

    }
    // e accent grave
    else if ( ( int32_t )asciiMsg[ i ] == -24 )
    {

      asciiMsg[ i ] = 'e';

    }
    // c cedille
    else if ( ( int32_t )asciiMsg[ i ] == -25 )
    {

      asciiMsg[ i ] = 'c';

    }
    // a accent grave
    else if ( ( int32_t )asciiMsg[ i ] == -32 )
    {

      asciiMsg[ i ] = 'a';

    }
    // u accent aigu
    else if ( ( int32_t )asciiMsg[ i ] == -7 )
    {

      asciiMsg[ i ] = 'u';

    }
    // a accent circonflexe
    else if ( ( int32_t )asciiMsg[ i ] == -30 )
    {

      asciiMsg[ i ] = 'a';

    }
    // e accent circonflexe
    else if ( ( int32_t )asciiMsg[ i ] == -22 )
    {

      asciiMsg[ i ] = 'e';

    }
    // i accent circonflexe
    else if ( ( int32_t )asciiMsg[ i ] == -18 )
    {

      asciiMsg[ i ] = 'i';

    }
    // o accent circonflexe
    else if ( ( int32_t )asciiMsg[ i ] == -12 )
    {

      asciiMsg[ i ] = 'o';

    }
    // u accent circonflexe
    else if ( ( int32_t )asciiMsg[ i ] == -5 )
    {

      asciiMsg[ i ] = 'u';

    }
    // a trema
    else if ( ( int32_t )asciiMsg[ i ] == -28 )
    {

      asciiMsg[ i ] = 'a';

    }
    // e trema
    else if ( ( int32_t )asciiMsg[ i ] == -21 )
    {

      asciiMsg[ i ] = 'e';

    }
    // i trema
    else if ( ( int32_t )asciiMsg[ i ] == -17 )
    {

      asciiMsg[ i ] = 'i';

    }
    // o trema
    else if ( ( int32_t )asciiMsg[ i ] == -10 )
    {

      asciiMsg[ i ] = 'o';

    }
    // u trema
    else if ( ( int32_t )asciiMsg[ i ] == -36 )
    {

      asciiMsg[ i ] = 'u';

    }
    // y trema
    else if ( ( int32_t )asciiMsg[ i ] == -1 )
    {

      asciiMsg[ i ] = 'y';

    }

  }

  return asciiMsg;

}


// replacing other characters by -
std::string gkg::StringConverter::toazAZ09( const std::string& msg )
{

  std::string asciiMsg = msg;

  size_t i;
  for ( i = 0; i < asciiMsg.length(); i++ )
  {

    if ( ( asciiMsg[ i ] == ' ' ) ||
         ( asciiMsg[ i ] == '\t' ) ||
         ( asciiMsg[ i ] == '&' ) ||
         ( asciiMsg[ i ] == '~' ) ||
         ( asciiMsg[ i ] == '\"' ) ||
         ( asciiMsg[ i ] == '#' ) ||
         ( asciiMsg[ i ] == '\'' ) ||
         ( asciiMsg[ i ] == '{' ) ||
         ( asciiMsg[ i ] == '(' ) ||
         ( asciiMsg[ i ] == '[' ) ||
         ( asciiMsg[ i ] == '|' ) ||
         ( asciiMsg[ i ] == '`' ) ||
         ( asciiMsg[ i ] == '\\' ) ||
         ( asciiMsg[ i ] == '^' ) ||
         ( asciiMsg[ i ] == '@' ) ||
         ( asciiMsg[ i ] == ')' ) ||
         ( asciiMsg[ i ] == ']' ) ||
         ( asciiMsg[ i ] == '=' ) ||
         ( asciiMsg[ i ] == '}' ) ||

         ( asciiMsg[ i ] == '$' ) ||
         ( asciiMsg[ i ] == '%' ) ||
         ( asciiMsg[ i ] == '*' ) ||
         ( asciiMsg[ i ] == '<' ) ||
         ( asciiMsg[ i ] == '>' ) ||
         ( asciiMsg[ i ] == '?' ) ||
         ( asciiMsg[ i ] == ',' ) ||
         ( asciiMsg[ i ] == '.' ) ||
         ( asciiMsg[ i ] == ';' ) ||
         ( asciiMsg[ i ] == '/' ) ||
         ( asciiMsg[ i ] == ':' ) ||
         ( asciiMsg[ i ] == '!' )/* ||
         ( asciiMsg[ i ] == '§' ) ||
         ( asciiMsg[ i ] == '£' )*/ )
    {

      asciiMsg[ i ] = '-';

    }

  }

  return asciiMsg;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     std::string& value )
{

  value = msg;
  return true;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     int8_t& value, int32_t base )
{

  int64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v < INT8_MIN || v > INT8_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( int8_t )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     uint8_t& value, int32_t base )
{

  uint64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v > UINT8_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( uint8_t )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     int16_t& value, int32_t base )
{

  int64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v < INT16_MIN || v > INT16_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( int16_t )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     uint16_t& value, int32_t base )
{

  uint64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v > UINT16_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( uint16_t )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     int32_t& value, int32_t base )
{

  int64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v < INT32_MIN || v > INT32_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( int32_t )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     uint32_t& value, int32_t base )
{

  uint64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v > UINT32_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( uint32_t )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     int64_t& value, int32_t base )
{

  const char* p = msg.c_str();
  int64_t val = INT64_C( 0 );
  int32_t l = msg.length();
  const int64_t maxMult = INT64_MAX / base;
  bool isOk = false;
  int32_t negative = 0;

  if ( !p )
    goto bye;
  while ( l && isSpace( *p ) )   // skip leading space
  {

    l --;
    p ++;

  }
  if ( l && *p == '-' ) 
  {

    l --;
    p ++;
    negative = 1;

  }
  else if ( *p == '+' )
  {

    l --;
    p ++;

  }

  if ( !l || !isOkInBase( *p, base ) )
    goto bye;
  while ( l && isOkInBase( *p, base ) )
  {

    l--;
    int32_t dv;
    if ( isdigit( *p ) )
    {

      dv = ( int32_t )( *p - '0' );

    }
    else
    {

      if ( *p >= 'a' && *p <= 'z' )
        dv = *p - 'a' + 10;
      else
        dv = *p - 'A' + 10;

    }
    if ( val > maxMult ||
         ( val == maxMult && dv > ( INT64_MAX % base ) + negative ) )
      goto bye;
    val = base * val + dv;
    p ++;

  }
  if ( negative )
    val = -val;
  while ( l && isSpace( *p ) )  // skip trailing space
  {

    l --;
    p ++;

  }
  if ( !l )
    isOk = true;

bye:

  if ( isOk )
  {

    value = val;
    return true;

  }
  else
  {

    value = 0L;
    return false;

  }

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     uint64_t& value, int32_t base )
{

  const char* p = msg.c_str();
  uint64_t val = 0;
  int32_t l = msg.length();
  const uint64_t maxMult = UINT64_MAX / base;
  bool isOk = false;

  if ( !p )
    goto bye;
  while ( l && isSpace( *p ) )   // skip leading space
  {

    l --;
    p ++;

  }
  if ( *p == '+' )
  {

    l --;
    p ++;

  }

  if ( !l || !isOkInBase( *p, base ) )
    goto bye;
  while ( l && isOkInBase( *p, base ) )
  {

    l--;
    uint32_t dv;
    if ( isdigit( *p ) )
    {

      dv = ( uint32_t )( *p - '0' );

    }
    else
    {

      if ( *p >= 'a' && *p <= 'z' )
        dv = *p - 'a' + 10;
      else
        dv = *p - 'A' + 10;

    }
    if ( val > maxMult ||
         ( val == maxMult && dv > ( UINT64_MAX % base ) ) )
      goto bye;
    val = base * val + dv;
    p ++;

  }
  while ( l && isSpace( *p ) )  // skip trailing space
  {

    l --;
    p ++;

  }
  if ( !l )
    isOk = true;

bye:

  if ( isOk )
  {

    value = val;
    return true;

  }
  else
  {

    value = 0L;
    return false;

  }

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     float& value )
{

  double v = 0;
  bool ok = toScalar( msg, v );
  if ( ok && ( v < -FLT_MAX || v > FLT_MAX ) )
  {

    ok = false;
    v = 0.0;

  }
  value = ( float )v;
  return ok;

}


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     double& value )
{

  bool ok = true;

  if ( msg.empty() )
  {

    ok = false;
    value = 0;

  }
  else
  {

    char* end;
    value = strtod( msg.c_str(), &end );
    ok = msg[ 0 ] && ( end == 0 || *end == '\0' );

  }

  return ok;

}


#if ( defined(__APPLE__) && defined(__MACH__) )

bool gkg::StringConverter::toScalar( const std::string& msg,
                                     size_t& value, int32_t base )
{

  uint64_t v = 0;
  bool ok = toScalar( msg, v, base );
  if ( ok && ( v > UINT64_MAX ) )
  {

    ok = false;
    v = 0;

  }
  value = ( size_t )v;
  return ok;

}

#endif


bool gkg::StringConverter::toScalar( const std::string& msg,
                                     bool& value )
{

  bool ok = true;
  if ( msg == "true" || msg == "TRUE" || msg == "True" )
  {

    value = true;
    return true;

  }
  else if ( msg == "false" || msg == "FALSE" || msg == "False" )
  {

    value = false;
    return true;

  }
  value = false;
  ok = false;
  return ok;
//  throw invalid_boolean( value );

}


std::string gkg::StringConverter::toString( const std::string& value )
{

  return value;


}


std::string gkg::StringConverter::toString( const int8_t& value,
                                            int32_t base )
{

  return toString( ( int64_t )value, base );


}


std::string gkg::StringConverter::toString( const uint8_t& value,
                                            int32_t base )
{

  return toString( ( uint64_t )value, base );


}


std::string gkg::StringConverter::toString( const int16_t& value,
                                            int32_t base )
{

  return toString( ( int64_t )value, base );


}


std::string gkg::StringConverter::toString( const uint16_t& value,
                                            int32_t base )
{

  return toString( ( uint64_t )value, base );


}


std::string gkg::StringConverter::toString( const int32_t& value,
                                            int32_t base )
{

  return toString( ( int64_t )value, base );


}


std::string gkg::StringConverter::toString( const uint32_t& value,
                                            int32_t base )
{

  return toString( ( uint32_t )value, base );


}


std::string gkg::StringConverter::toString( const int64_t& value, int32_t base )
{

  int64_t val = value;
  char charBuffer[ 65 ];
  char* buffer = ( char *)charBuffer;
  char* p = &buffer[ 64 ];
  int32_t  length = 0;
  bool negative;
  if ( val < 0 )
  {

    negative = true;
    if ( val == INT32_MIN )
    {

      // Cannot always negate this special case
      std::string s1, s2;
      s1 = toString( val / base );
      s2 = toString( - ( val + base ) % base );
      return s1 + s2;

    }
    val = -val;

  }
  else
  {

    negative = false;

  }
  do
  {

    *--p = "0123456789abcdefghijklmnopqrstuvwxyz"[ 
                                                ( ( int32_t )( val % base ) ) ];
    val /= base;
    length ++;

  }
  while ( val );
  if ( negative )
  {

    *--p = '-';
    length ++;

  }
  return std::string( p, length );

}


std::string gkg::StringConverter::toString( const uint64_t& value,
                                            int32_t base )
{

  uint64_t val = value;
  char charBuffer[ 65 ];
  char* buffer = ( char* )charBuffer;
  char* p = &buffer[ 64 ];
  int32_t length = 0;
  do
  {

    *--p = "0123456789abcdefghijklmnopqrstuvwxyz"[ 
                                                ( ( int32_t )( val % base ) ) ];
    val /= base;
    length ++;

  }
  while ( val );
  return std::string( p, length );

}


std::string gkg::StringConverter::toString( const float& value,
                                            char format,
                                            int32_t width,
                                            int32_t precision )
{

  return toString( ( double )value, format, width, precision );


}


std::string gkg::StringConverter::toString( const double& value,
                                            char format,
                                            int32_t width,
                                            int32_t precision )
{

  if ( !( format == 'f' ||
          format == 'F' ||
          format == 'e' ||
          format == 'E' ||
          format == 'g' ||
          format == 'G' ) )
  {

    std::cerr << "StringConverter::toString( double...): invalid format "
              << format << std::endl;
    format = 'f';

  }

  char fmt[ 20 ];
  char *fs = fmt; // generate format string: %.<prec>l<f>
  *fs++ = '%';
  if ( width >= 0 )
  {

    if ( width > 99 ) // rather than crash in sprintf()
      width = 99;
    if ( width >= 10 )
    {

      *fs++ = width / 10 + '0';
      *fs++ = width % 10 + '0';

    }
    else
    {

      *fs++ = width + '0';

    }

  }
  if ( precision >= 0 )
  {

    if ( precision > 99 ) // rather than crash in sprintf()
      precision = 99;
    *fs++ = '.';
    if ( precision >= 10 )
    {

      *fs++ = precision / 10 + '0';
      *fs++ = precision % 10 + '0';

    }
    else
    {

      *fs++ = precision + '0';

    }

  }
  *fs++ = 'l';
  *fs++ = format;
  *fs = '\0';

  char buffer[ 512 ];
  std::sprintf( buffer, fmt, value );
  return std::string( buffer );

}


std::string gkg::StringConverter::toString( const float& value,
                                            int32_t width,
                                            int32_t precision )
{

  return toString( ( double )value, width, precision );

}

std::string gkg::StringConverter::toString( const double& value,
                                            int32_t width,
                                            int32_t precision )
{

  std::ostringstream buffer;
  buffer.precision( precision );
  buffer.width( width );
  buffer << value;

  return buffer.str();

}


#if ( defined(__APPLE__) && defined(__MACH__) )

std::string gkg::StringConverter::toString( const size_t& value,
                                            int32_t base )
{

  return toString( ( uint64_t )value, base );


}

#endif


std::string gkg::StringConverter::toString( const bool& value )
{

  return ( value == true ? "true" : "false" );


}


std::string gkg::StringConverter::toLower( const std::string& msg )
{


  std::string result = msg;
  for ( size_t c = 0; c < result.length(); c++ )
    result[ c ] = ( char )tolower( result[ c ] );

  return result;

}


std::string gkg::StringConverter::toUpper( const std::string& msg )
{

  std::string result = msg;
  for ( size_t c = 0; c < result.length(); c++ )
    result[ c ] = ( char )toupper( result[ c ] );

  return result;

}


bool gkg::StringConverter::isSpace( char c )
{

  return c >= 9 && c <= 13;

}


bool gkg::StringConverter::isOkInBase( char c, int32_t base )
{

  if ( base <= 10 )
    return isdigit( c ) && ( int32_t )( c - '0' ) < base;
  else
    return isdigit( c ) || ( c >= 'a' && c < ( char )( 'a' + base - 10 ) )
                        || ( c >= 'A' && c < ( char )( 'A' + base - 10 ) );

}


