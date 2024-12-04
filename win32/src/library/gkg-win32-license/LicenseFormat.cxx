#ifdef WIN32
#pragma warning (disable : 4702)
#endif


#include <gkg-win32-license/LicenseFormat.h>

#include <cstdio>


gkg::LicenseFormat::LicenseFormat()
{
}


gkg::LicenseFormat::~LicenseFormat()
{
}


std::string gkg::LicenseFormat::toString( uint64_t value )
{

  int32_t i = 4;
  std::string str;
  char tmp[ 11 ];

  sprintf( tmp, "%010"PRIu64, value / UINT64_C( 10000000000 ) );
  str = str + tmp;
  sprintf( tmp, "%010"PRIu64, value % UINT64_C( 10000000000 ) );
  str = str + tmp;

  while ( i )
  {
  
    str.insert( 4 * i--, "-" );
 
  }

  return str;

}


uint64_t gkg::LicenseFormat::fromString( std::string text )
{

  size_t pos = text.find( '-' );

  while ( pos != std::string::npos )
  {
  
    text.erase( pos, 1 );
    pos = text.find( '-' );
    
  }

  // convert string to a uint64_t (a kind of atoui64() function)
  uint64_t value = 0;
  std::string::iterator
    c = text.begin(),
    ce = text.end();
    
  while ( c != ce )
  {
  
    value = value * 10 + ( *c - 0x30 );
    ++c;
  
  }

  return value;

}
