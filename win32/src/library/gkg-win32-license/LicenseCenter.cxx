#ifdef WIN32
#pragma warning (disable : 4505)
#pragma warning (disable : 4702)
#endif


#include <gkg-win32-license/LicenseCenter.h>
#include <gkg-win32-license/LicenseFormat.h>
#include <gkg-win32-license/KeyName.h>

#ifndef VXWORKS
#include <gkg-win32-register/Register.h>


// some CString <-> std::string conversion functions


static std::string toStdString( const wchar_t* lptstr )
{

  char txt[ MAX_REG_STRING_LEN ];
  wcstombs( txt, lptstr, MAX_REG_STRING_LEN );

  return std::string( txt );

}


static std::string toStdString( const char* lptstr )
{

  return std::string( lptstr );

}


static CString toCString( std::string str )
{

  CString txt;

#if defined( _UNICODE ) || defined( UNICODE )

  TCHAR buf[ MAX_REG_STRING_LEN ];

  mbstowcs( buf, str.c_str(), MAX_REG_STRING_LEN );
  txt = buf;

#else

  txt = str.c_str();

#endif

  return txt;

}

#endif


gkg::LicenseCenter::LicenseCenter()
{
}


gkg::LicenseCenter::~LicenseCenter()
{
}


bool gkg::LicenseCenter::connect( std::string file,
                                  uint64_t hardwareInformation )
{

  if ( file.empty() )
  {
  
    return false;

  }
  
  _applications.clear();
  _file = file;
  _hardwareInformation = hardwareInformation;

  return initialize();
  
}


bool gkg::LicenseCenter::initialize()
{

  bool status = false;

#ifndef VXWORKS

  CRegister reg( _T( "" ) );

  status = reg.SetSubKey( toCString( _file ) );

  if ( status )
  {

    DWORD i, n = reg.NumberOfValues();

    for ( i = 0; i < n; i++ )
    {

      CString keyName = reg.KeyValue( i );

      if ( !keyName.IsEmpty() )
      {

        CString strLic = reg.GetString( keyName );

        registerLicense( toStdString( strLic ) );
        status = true;

      }

    }

    reg.SetSubKey( _T( "" ) );

  }

#endif
 
  return status;

}


void gkg::LicenseCenter::registerLicense( std::string strLicense )
{

  gkg::LicenseFormat licenseFormat;
  uint64_t license = licenseFormat.fromString( strLicense );
  uint64_t applicationId = _licenseEncoder.getApplicationId( 
                                                         license, 
                                                         _hardwareInformation );

  _applications.insert( applicationId );

}


bool gkg::LicenseCenter::hasValidKeys()
{

  bool validKeys = false;
  std::list< std::string > applicationNames = 
                              gkg::KeyName::getInstance().getApplicationNames();

  std::list< std::string >::iterator
    a = applicationNames.begin(),
    ae = applicationNames.end();

  while ( a != ae )
  {

    validKeys |= hasApplication( gkg::KeyName::getInstance().getId( *a ) );
    ++ a;

  }

  return validKeys;

}


bool gkg::LicenseCenter::hasApplication( uint64_t applicationId )
{

  std::set< uint64_t >::iterator a = _applications.find( applicationId );

  return ( a != _applications.end() );

}
