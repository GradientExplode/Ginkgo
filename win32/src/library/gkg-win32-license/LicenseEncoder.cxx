#ifdef WIN32
#pragma warning (disable : 4702)
#endif


#include <gkg-win32-license/LicenseEncoder.h>
#include <gkg-win32-license/LicenseFormat.h>


gkg::LicenseEncoder::LicenseEncoder()
                   : _mask0( UINT64_C( 0x0056A9E7832C410B ) ),
                     _mask1( UINT64_C( 0x0C75AE32D81F94B6 ) ),
                     _mask2( UINT64_C( 0x001D58F39E2BC107 ) ),
                     _mask3( UINT64_C( 0x00000045EF78CD44 ) )
{
}


gkg::LicenseEncoder::~LicenseEncoder()
{
}


uint64_t gkg::LicenseEncoder::keyEncode( uint64_t key )
{

  uint64_t res = ( key & UINT64_C( 0x00000000000000FF ) ) << 44;
  res |= ( key & UINT64_C( 0x000000000000FF00 ) ) << 28;
  res |= ( key & UINT64_C( 0x0000000000FF0000 ) ) << 12;
  res |= ( key & UINT64_C( 0x0000FFFFFF000000 ) ) >> 20;
  res ^= _mask0;

  return res;

}


uint64_t gkg::LicenseEncoder::keyDecode( uint64_t key )
{

  key ^= _mask0;
  
  uint64_t res = ( key & UINT64_C( 0x000FF00000000000 ) ) >> 44;
  res |= ( key & UINT64_C( 0x00000FF000000000 ) ) >> 28;
  res |= ( key & UINT64_C( 0x0000000FF0000000 ) ) >> 12;
  res |= ( key & UINT64_C( 0x000000000FFFFFF0 ) ) << 20;

  return res;

}


std::string gkg::LicenseEncoder::getProtectionCode( uint64_t key,
                                                    uint64_t applicationId )
{

  gkg::LicenseFormat licenseFormat;
  
  uint64_t hardwareInformation = keyDecode( key );
  uint64_t applicationCode = encode( 
                              hardwareInformation ^ ( applicationId << 8 ), 0 );

  return licenseFormat.toString( applicationCode );

}


bool gkg::LicenseEncoder::checkLicense( uint64_t applicationId, 
                                        std::string strLicense,
                                        uint64_t hardwareInformation )
{

  uint64_t hardware = decode( strLicense ) ^ ( applicationId << 8 );

  return ( hardware == hardwareInformation );

}


uint64_t gkg::LicenseEncoder::getApplicationId( uint64_t license,
                                                uint64_t hardwareInformation )
{
  
  uint64_t id = ( decode( license ) ^ hardwareInformation ) >> 8;

  return id;
  
}


uint64_t gkg::LicenseEncoder::encode( uint64_t hid, uint64_t license )
{

  uint64_t c = hid;

  c ^= _mask1;
  c += _mask2;
  uint64_t res = ( c & UINT64_C( 0xFF00000000000000 ) ) >> 16;
  res |= ( c & UINT64_C( 0x00FF000000000000 ) ) >> 24;
  res |= ( c & UINT64_C( 0x0000FF0000000000 ) ) >> 24;
  res |= ( c & UINT64_C( 0x000000FF00000000 ) ) << 24;
  res |= ( c & UINT64_C( 0x00000000FF000000 ) ) >> 16;
  res |= ( c & UINT64_C( 0x0000000000FF0000 ) ) << 16;
  res |= ( c & UINT64_C( 0x000000000000FF00 ) ) >> 8;
  res |= ( c & UINT64_C( 0x00000000000000FF ) ) << 48;
  res -= _mask3;

  return ( res - license );

}


uint64_t gkg::LicenseEncoder::decode( std::string strLicense )
{

  gkg::LicenseFormat licenseFormat;
  
  return decode( licenseFormat.fromString( strLicense ) );

}


uint64_t gkg::LicenseEncoder::decode( uint64_t license )
{

  uint64_t c = license + _mask3;
  uint64_t r = ( c & UINT64_C( 0x00000000000000FF ) ) << 8;
  r |= ( c & UINT64_C( 0x000000000000FF00 ) ) << 16;
  r |= ( c & UINT64_C( 0x0000000000FF0000 ) ) << 24;
  r |= ( c & UINT64_C( 0x00000000FF000000 ) ) << 24;
  r |= ( c & UINT64_C( 0x000000FF00000000 ) ) >> 16;
  r |= ( c & UINT64_C( 0x0000FF0000000000 ) ) << 16;
  r |= ( c & UINT64_C( 0x00FF000000000000 ) ) >> 48;
  r |= ( c & UINT64_C( 0xFF00000000000000 ) ) >> 24;
  r -= _mask2;
  r ^= _mask1;

  return r;

}
