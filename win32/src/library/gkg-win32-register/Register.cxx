#ifdef WIN32
#pragma warning (disable : 4505)
#pragma warning (disable : 4702)
#endif


#include <gkg-win32-register/Register.h>


#ifndef VXWORKS


gkg::CRegister::CRegister( LPCTSTR pszSubKeyName )
{

  m_fBaseOpen  = false;
  m_fLocalOpen = false;

  if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                     _T( "Software\\NeuroSpin" ),
                     0,
                     KEY_READ | KEY_WOW64_32KEY,
                     &m_hBaseKey ) == ERROR_SUCCESS )
  {

    m_fBaseOpen = true;
    SetSubKey( pszSubKeyName );

  }

}


gkg::CRegister::~CRegister()
{

  if ( m_fBaseOpen )
  {

    RegCloseKey( m_hBaseKey );
    m_fBaseOpen = false;

  }

  if ( m_fLocalOpen )
  {
    RegCloseKey( m_hLocalKey );
    m_fLocalOpen = false;
  }

}


bool gkg::CRegister::SetSubKey( LPCTSTR pszSubKeyName )
{

  if ( m_fLocalOpen )
  {

    RegCloseKey( m_hLocalKey );
    m_fLocalOpen = false;

  }

  if ( m_fBaseOpen && pszSubKeyName && pszSubKeyName[ 0 ] )
  {

    m_fLocalOpen = ( RegOpenKeyEx( m_hBaseKey,
                                   pszSubKeyName,
                                   0,
                                   KEY_READ | KEY_WOW64_32KEY,
                                   &m_hLocalKey ) == ERROR_SUCCESS );

    if ( !m_fLocalOpen )
    {

      m_hLocalKey  = m_hBaseKey;

    }

    return m_fLocalOpen;

  }
  else
  {

    m_fLocalOpen = false;
    m_hLocalKey  = m_hBaseKey;

    return true;

  }

}


DWORD gkg::CRegister::NumberOfKeys()
{

  return ( ( m_fBaseOpen && QueryLocalKeyInfo() ) ? key.m_dwSubKeys : 0 );

} 


DWORD gkg::CRegister::NumberOfValues()
{

  return ( ( m_fBaseOpen && QueryLocalKeyInfo() ) ? key.m_dwValues : 0 );

} 

CString gkg::CRegister::KeyName( DWORD dwIndex )
{

  CString str( _T( "" ) );

  if ( m_fBaseOpen )
  {

    TCHAR szKeyName[ 100 ];
    TCHAR szKeyClass[ 100 ];
    DWORD dwNameSize = sizeof( szKeyName );
    DWORD dwClassSize = sizeof( szKeyClass );
    FILETIME ftLastWrite;

    if ( RegEnumKeyEx( m_hLocalKey,
                       dwIndex,
                       szKeyName,
                       &dwNameSize,
                       NULL,
                       szKeyClass,
                       &dwClassSize,
                       &ftLastWrite ) == ERROR_SUCCESS )
    {

      str = szKeyName;

    }

  }

  return str;

}


CString gkg::CRegister::KeyValue( DWORD dwIndex )
{

  CString str( _T( "" ) );

  if ( m_fBaseOpen )
  {

    TCHAR szKeyName[ 100 ];
    DWORD dwNameSize = sizeof( szKeyName );

    if ( RegEnumValue( m_hLocalKey,
                       dwIndex,
                       szKeyName,
                       &dwNameSize,
                       NULL,
                       NULL,
                       NULL,
                       NULL ) == ERROR_SUCCESS )
    {

      if ( dwNameSize )
      {

        str = szKeyName;

      }

    }

  }

  return str;

}


CString gkg::CRegister::GetString( LPCTSTR pszKeyName )
{

  CString str( _T( "" ) );

  if ( m_fBaseOpen )
  {

    TCHAR szValue[ MAX_REG_STRING_LEN ];
    DWORD dwSize = sizeof( szValue );
    DWORD dwType;

    if ( RegQueryValueEx( m_hLocalKey,
                          pszKeyName,
                          NULL,
                          &dwType,
                          LPBYTE( szValue ),
                          &dwSize ) == ERROR_SUCCESS )
    {

      if ( dwType == REG_SZ )
      {

        str = szValue;

      }

    }

  }

  return str;

}


bool gkg::CRegister::QueryLocalKeyInfo()
{

  if ( m_fBaseOpen )
  {

    key.m_dwNameSize = sizeof( key.m_szName );

    return ( RegQueryInfoKey( m_hLocalKey,
                              key.m_szName,
                              &key.m_dwNameSize,
                              NULL,
                              &key.m_dwSubKeys,
                              &key.m_dwMaxSubKey,
                              &key.m_dwMaxClass,
                              &key.m_dwValues,
                              &key.m_dwMaxName,
                              &key.m_dwMaxValue,
                              &key.m_dwSecurity,
                              &key.m_ftLastWrite ) == ERROR_SUCCESS );

  }

  return false;

}


#endif
