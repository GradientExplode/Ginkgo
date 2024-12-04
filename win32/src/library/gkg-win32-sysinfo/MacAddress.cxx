#ifdef WIN32
#pragma warning (disable : 4702)
#endif


#include <gkg-win32-sysinfo/MacAddress.h>
#include <gkg-core-exception/Exception.h>


#ifndef VXWORKS

#define _AFXDLL
#include <afx.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

#endif


gkg::MacAddress::MacAddress()
{

  scanInterfaces();

}


gkg::MacAddress::~MacAddress()
{
}


uint64_t gkg::MacAddress::getMacAddress( std::string ip )
{

  uint64_t macAddress = UINT64_C( 0x00FFFFFFFFFFFFFF );
  
  std::map< std::string, uint64_t >::iterator
    m = _macAddress.find( ip );
    
  if ( m != _macAddress.end() )
  {
  
    macAddress = m->second;
  
  }
  
  return macAddress;

}

std::set< std::string > gkg::MacAddress::getIpAddresses()
{

  try
  {

    std::set< std::string > setIpAddress;
    std::map< std::string, uint64_t >::iterator i  = _macAddress.begin(),
                                                ie = _macAddress.end();

    while ( i != ie )
    {

      setIpAddress.insert( i->first );
      ++ i;

    }

    return setIpAddress;

  }
  GKG_CATCH( "std::set< std::string > gkg::MacAddress::getIpAddresses()" );

}


void gkg::MacAddress::scanInterfaces()
{

#ifndef VXWORKS

  uint32_t ulOutBufLen = sizeof( IP_ADAPTER_INFO );
  PIP_ADAPTER_INFO aInfo = (IP_ADAPTER_INFO*)malloc( ulOutBufLen );

  if ( GetAdaptersInfo( aInfo, (PULONG)&ulOutBufLen ) != ERROR_SUCCESS )
  {

    free( aInfo );
    aInfo = (IP_ADAPTER_INFO*)malloc( ulOutBufLen );

  }

  if ( GetAdaptersInfo( aInfo, (PULONG)&ulOutBufLen ) == NO_ERROR )
  {

    uint32_t i;
    PIP_ADAPTER_INFO pAInfo = aInfo;

    while ( pAInfo )
    {
    
      std::string ipAddress = pAInfo->IpAddressList.IpAddress.String;

      if ( ( ipAddress != "0.0.0.0" ) && ( ipAddress != "127.0.0.1" ) )
      {

        uint64_t macAddress = 0;

        for ( i = 0; i < pAInfo->AddressLength; i++ )
        {

          macAddress |= (uint64_t)pAInfo->Address[ i ] << ( 8 * ( 5 - i ) );

        }

        _macAddress.insert( std::make_pair( ipAddress, macAddress ) );

      }

      pAInfo = pAInfo->Next;

    }

  }

  free( aInfo );

#endif
  
}
