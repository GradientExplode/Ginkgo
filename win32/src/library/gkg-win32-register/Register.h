#ifndef _gkg_win32_register_Register_h_
#define _gkg_win32_register_Register_h_


#ifdef WIN32
#pragma warning (disable : 4786)
#endif


#ifndef VXWORKS


#define _AFXDLL
#include <afx.h>


#define MAX_REG_STRING_LEN 256


namespace gkg
{


class CRegister
{

  struct Key
  {

    TCHAR m_szName[ MAX_PATH ];
    DWORD m_dwNameSize;
    DWORD m_dwSubKeys;
    DWORD m_dwMaxSubKey;
    DWORD m_dwMaxClass;
    DWORD m_dwValues;
    DWORD m_dwMaxName;
    DWORD m_dwMaxValue;
    DWORD m_dwSecurity;
    FILETIME m_ftLastWrite;

  };

  public:

    CRegister( LPCTSTR pszSubKeyName = NULL );
    ~CRegister();

    bool SetSubKey( LPCTSTR pszSubKeyName = NULL );
    DWORD NumberOfKeys();
    DWORD NumberOfValues();
    CString KeyName( DWORD dwIndex );
    CString KeyValue( DWORD dwIndex );
    CString GetString( LPCTSTR pszKeyName );

  protected:

    bool m_fBaseOpen;
    bool m_fLocalOpen;
    HKEY m_hBaseKey;
    HKEY m_hLocalKey;
    Key  key;

    bool QueryLocalKeyInfo();

};


}


#endif


#endif
