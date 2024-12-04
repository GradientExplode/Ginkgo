//
//   g++ -W -Wall -o getMac getMac.cxx
//


#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// method 1
#include <sys/ioctl.h>

// method 2
#include <cstring>
#include <ifaddrs.h>

// HDD
#include <linux/hdreg.h>
#include <fcntl.h>


// ----- Method 1 -----

void printIPv4Information()
{

  int s = socket( AF_INET, SOCK_STREAM, 0 );

  if ( s >= 0 )
  {

    std::cout << "IPv4 domain:" << std::endl;

    struct ifconf ifc;
    struct ifreq ifr[ 50 ];

    ifc.ifc_len = sizeof( ifr );
    ifc.ifc_req = ifr;

    if ( ioctl( s, SIOCGIFCONF, &ifc ) >= 0 )
    {

      int i, nif = ifc.ifc_len / sizeof( struct ifreq );
      char ip[ INET_ADDRSTRLEN ];

      for ( i = 0; i < nif; i++ )
      {

        struct ifreq *item = &ifr[ i ];
        struct sockaddr_in* s_in = (struct sockaddr_in*)&item->ifr_addr;

        if ( inet_ntop( AF_INET, 
                        &s_in->sin_addr, 
                        ip, 
                        sizeof( ip ) ) )
        {

          std::cout << "\t" << item->ifr_name
                    << " :\tIP  " << ip << std::endl;

        }

        if ( ioctl( s, SIOCGIFHWADDR, item ) >= 0 )
        {

          std::cout << "\t\tMAC " 
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 0 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 1 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 2 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 3 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 4 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 5 ]
                    << std::dec << std::endl;

        }

      }

    }

    close( s );

  }
  else
  {

    std::cerr << "No IPv4 domain supported" << std::endl;

  }

}


void printIPv6Information()
{

  int s = socket( AF_INET6, SOCK_STREAM, 0 );

  if ( s >= 0 )
  {

    std::cout << "IPv6 domain:" << std::endl;

    struct ifconf ifc;
    struct ifreq ifr[ 50 ];

    ifc.ifc_len = sizeof( ifr );
    ifc.ifc_req = ifr;

    if ( ioctl( s, SIOCGIFCONF, &ifc ) >= 0 )
    {

      int i, nif = ifc.ifc_len / sizeof( struct ifreq );
      char ip[ INET6_ADDRSTRLEN ];

      for ( i = 0; i < nif; i++ )
      {

        struct ifreq *item = &ifr[ i ];
        struct sockaddr_in6* s_in = (struct sockaddr_in6*)&item->ifr_addr;

        if ( inet_ntop( AF_INET6, 
                        &s_in->sin6_addr, 
                        ip, 
                        sizeof( ip ) ) )
        {

          std::cout << "\t" << item->ifr_name
                    << " :\tIP  " << ip << std::endl;

        }

        if ( ioctl( s, SIOCGIFHWADDR, item ) >= 0 )
        {

          std::cout << "\t\tMAC " 
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 0 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 1 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 2 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 3 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 4 ]
                    << ":"
                    << std::hex << std::setfill( '0' ) << std::setw( 2 )
                    << (int)( (unsigned char*)item->ifr_hwaddr.sa_data )[ 5 ]
                    << std::dec << std::endl;

        }

      }

    }

    close( s );

  }
  else
  {

    std::cerr << "No IPv6 domain supported" << std::endl;

  }

}


// ----- Method 2 -----

void printIfa( struct ifaddrs* ifa )
{

  if ( ifa->ifa_addr )
  {

    if ( ifa->ifa_flags & IFF_UP )
    {

      if ( ifa->ifa_addr->sa_family == AF_INET )
      {

        char ip[ INET_ADDRSTRLEN ];
        struct sockaddr_in* s = (struct sockaddr_in *)ifa->ifa_addr;
   
        if ( inet_ntop( ifa->ifa_addr->sa_family, 
                        (void *)&s->sin_addr, 
                        ip, 
                        INET_ADDRSTRLEN ) )
        {

          std::cout << "IPv4 domain:" << std::endl;
          std::cout << "\t" << ifa->ifa_name
                    << " :\tIP  " << ip << std::endl;

        }

      }
      else if ( ifa->ifa_addr->sa_family == AF_INET6 )
      {

        char ip[ INET6_ADDRSTRLEN ];
        struct sockaddr_in6* s = (struct sockaddr_in6 *)ifa->ifa_addr;
   
        if ( inet_ntop( ifa->ifa_addr->sa_family, 
                        (void *)&s->sin6_addr, 
                        ip, 
                        INET6_ADDRSTRLEN ) )
        {

          std::cout << "IPv6 domain:" << std::endl;
          std::cout << "\t" << ifa->ifa_name
                    << " :\tIP  " << ip << std::endl;

        }

      }
      if ( ifa->ifa_addr->sa_family == AF_PACKET )
      {

        struct ifreq ifr;

        int s = socket( AF_INET, SOCK_DGRAM, 0 );

        if ( s >= 0 )
        {

          ifr.ifr_addr.sa_family = AF_INET;
          std::strcpy( ifr.ifr_name, ifa->ifa_name );

          if ( ioctl( s, SIOCGIFHWADDR, &ifr ) >= 0 )
          {

            close( s );

            std::cout << "Packet domain:" << std::endl;

            if ( ifa->ifa_flags & IFF_RUNNING )
            {

              std::cout << "#";

            }

            std::cout << "\t" << ifa->ifa_name
                      << " :\tMAC ";

            if ( ifa->ifa_flags & IFF_LOOPBACK )
            {

              std::cout << "-" << std::endl;

            }
            else
            {

              std::cout << std::hex << std::setfill( '0' ) << std::setw( 2 )
                        << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 0 ]
                        << ":"
                        << std::hex << std::setfill( '0' ) << std::setw( 2 )
                        << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 1 ]
                        << ":"
                        << std::hex << std::setfill( '0' ) << std::setw( 2 )
                        << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 2 ]
                        << ":"
                        << std::hex << std::setfill( '0' ) << std::setw( 2 )
                        << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 3 ]
                        << ":"
                        << std::hex << std::setfill( '0' ) << std::setw( 2 )
                        << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 4 ]
                        << ":"
                        << std::hex << std::setfill( '0' ) << std::setw( 2 )
                        << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 5 ]
                        << std::dec << std::endl;

            }

          }

        }

      }

    }
    else
    {

      std::cout << "\t" << ifa->ifa_name
                << " : " << "\tInterface is down" << std::endl;

    }

  }
  else
  {

    std::cout << "\t" << ifa->ifa_name
              << " : " << "\tNo address" << std::endl;


  }

}


void printMethod2()
{

  struct ifaddrs* addr;
  
  if ( !getifaddrs( &addr ) )
  {

    struct ifaddrs* ifa = addr;

    while ( ifa != NULL )
    {

      printIfa( ifa );
      ifa = ifa->ifa_next;

    }

    freeifaddrs( addr );

  }

}


// ----- Method 3 -----

class IfInfo
{

  public:

    IfInfo();
    IfInfo( const IfInfo& other );
    virtual ~IfInfo();

    std::string mac;
    std::string ipv4;
    std::string netmask;
    std::string broadcast;
    std::string ipv6;

};


IfInfo::IfInfo()
      : mac( "" ),
        ipv4( "" ),
        netmask( "" ),
        broadcast( "" ),
        ipv6( "" )
{
}


IfInfo::IfInfo( const IfInfo& other )
      : mac( other.mac ),
        ipv4( other.ipv4 ),
        netmask( other.netmask ),
        broadcast( other.broadcast ),
        ipv6( other.ipv6 )
{
}


IfInfo::~IfInfo()
{
}


class IfList
{

  public:

    IfList();
    virtual ~IfList();

    void addMac( std::string inter, std::string mac );
    void addIPv4( std::string inter, std::string ip );
    void addNetmask( std::string inter, std::string netmask );
    void addBroadcast( std::string inter, std::string broadcast );
    void addIPv6( std::string inter, std::string ip );

    void print();

  private:

    std::map< std::string, IfInfo > interfaces;

};


IfList::IfList()
{
}


IfList::~IfList()
{
}


void IfList::addMac( std::string inter, std::string mac )
{

  std::map< std::string, IfInfo >::iterator
    i = interfaces.find( inter );

  if ( i == interfaces.end() )
  {

    IfInfo ifi;
    ifi.mac = mac;
    interfaces.insert( std::make_pair( inter, ifi ) );

  }
  else
  {

    i->second.mac = mac;

  }

}


void IfList::addIPv4( std::string inter, std::string ip )
{

  std::map< std::string, IfInfo >::iterator
    i = interfaces.find( inter );

  if ( i == interfaces.end() )
  {

    IfInfo ifi;
    ifi.ipv4 = ip;
    interfaces.insert( std::make_pair( inter, ifi ) );

  }
  else
  {

    i->second.ipv4 = ip;

  }

}


void IfList::addNetmask( std::string inter, std::string netmask )
{

  std::map< std::string, IfInfo >::iterator
    i = interfaces.find( inter );

  if ( i == interfaces.end() )
  {

    IfInfo ifi;
    ifi.netmask = netmask;
    interfaces.insert( std::make_pair( inter, ifi ) );

  }
  else
  {

    i->second.netmask = netmask;

  }

}


void IfList::addBroadcast( std::string inter, std::string broadcast )
{

  std::map< std::string, IfInfo >::iterator
    i = interfaces.find( inter );

  if ( i == interfaces.end() )
  {

    IfInfo ifi;
    ifi.broadcast = broadcast;
    interfaces.insert( std::make_pair( inter, ifi ) );

  }
  else
  {

    i->second.broadcast = broadcast;

  }

}


void IfList::addIPv6( std::string inter, std::string ip )
{

  std::map< std::string, IfInfo >::iterator
    i = interfaces.find( inter );

  if ( i == interfaces.end() )
  {

    IfInfo ifi;
    ifi.ipv6 = ip;
    interfaces.insert( std::make_pair( inter, ifi ) );

  }
  else
  {

    i->second.ipv6 = ip;

  }

}


void IfList::print()
{

  std::map< std::string, IfInfo >::iterator
    i = interfaces.begin(),
    ie = interfaces.end();

  while ( i != ie )
  {

    std::cout << i->first << " :\tMAC " << i->second.mac << std::endl;

    if ( i->second.ipv4.length() )
    {

      std::cout << "\tIPv4 " << i->second.ipv4 << std::endl;

    }

    if ( i->second.netmask.length() )
    {

      std::cout << "\tNetmask " << i->second.netmask << std::endl;

    }

    if ( i->second.broadcast.length() )
    {

      std::cout << "\tBroadcast " << i->second.broadcast << std::endl;

    }

    if ( i->second.ipv6.length() )
    {

      std::cout << "\tIPv6 " << i->second.ipv6 << std::endl;

    }

    ++i;

  }

}


void fill( struct ifaddrs* ifa, IfList& inter )
{

  if ( ifa->ifa_addr )
  {

    if ( ifa->ifa_flags & IFF_UP )
    {

      if ( ifa->ifa_addr->sa_family == AF_INET )
      {

        char ip[ INET_ADDRSTRLEN ];
        struct sockaddr_in* s = (struct sockaddr_in*)ifa->ifa_addr;
   
        if ( inet_ntop( ifa->ifa_addr->sa_family, 
                        (void *)&s->sin_addr, 
                        ip, 
                        INET_ADDRSTRLEN ) )
        {

          inter.addIPv4( ifa->ifa_name, ip );

        }

        s = (struct sockaddr_in*)ifa->ifa_netmask;
   
        if ( inet_ntop( ifa->ifa_addr->sa_family, 
                        (void *)&s->sin_addr, 
                        ip, 
                        INET_ADDRSTRLEN ) )
        {

          inter.addNetmask( ifa->ifa_name, ip );

        }

        s = (struct sockaddr_in*)ifa->ifa_broadaddr;
   
        if ( inet_ntop( ifa->ifa_addr->sa_family, 
                        (void *)&s->sin_addr, 
                        ip, 
                        INET_ADDRSTRLEN ) )
        {

          inter.addBroadcast( ifa->ifa_name, ip );

        }

      }
      else if ( ifa->ifa_addr->sa_family == AF_INET6 )
      {

        char ip[ INET6_ADDRSTRLEN ];
        struct sockaddr_in6* s = (struct sockaddr_in6*)ifa->ifa_addr;
   
        if ( inet_ntop( ifa->ifa_addr->sa_family, 
                        (void *)&s->sin6_addr, 
                        ip, 
                        INET6_ADDRSTRLEN ) )
        {

          inter.addIPv6( ifa->ifa_name, ip );

        }

      }
      if ( ifa->ifa_addr->sa_family == AF_PACKET )
      {

        struct ifreq ifr;
        int sock = socket( AF_INET, SOCK_DGRAM, 0 );

        if ( sock >= 0 )
        {

          ifr.ifr_addr.sa_family = AF_INET;
          std::strcpy( ifr.ifr_name, ifa->ifa_name );

          if ( ifa->ifa_flags & IFF_LOOPBACK )
          {

            inter.addMac( ifa->ifa_name, "-" );

          }
          else
          {

            if ( ioctl( sock, SIOCGIFHWADDR, &ifr ) >= 0 )
            {

              std::ostringstream oss;
              oss << std::hex << std::setfill( '0' ) << std::setw( 2 )
                  << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 0 ]
                  << ":"
                  << std::hex << std::setfill( '0' ) << std::setw( 2 )
                  << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 1 ]
                  << ":"
                  << std::hex << std::setfill( '0' ) << std::setw( 2 )
                  << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 2 ]
                  << ":"
                  << std::hex << std::setfill( '0' ) << std::setw( 2 )
                  << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 3 ]
                  << ":"
                  << std::hex << std::setfill( '0' ) << std::setw( 2 )
                  << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 4 ]
                  << ":"
                  << std::hex << std::setfill( '0' ) << std::setw( 2 )
                  << (int)( (unsigned char*)ifr.ifr_hwaddr.sa_data )[ 5 ]
                  << std::dec;
              inter.addMac( ifa->ifa_name, oss.str() );

            }

          }

          close( sock );

        }

      }

    }

  }

}


void printMethod3()
{

  struct ifaddrs* addr;
  
  if ( !getifaddrs( &addr ) )
  {

    IfList interfaces;
    struct ifaddrs* ifa = addr;

    while ( ifa != NULL )
    {

      fill( ifa, interfaces );
      ifa = ifa->ifa_next;

    }

    freeifaddrs( addr );
    interfaces.print();

  }

}


// ----- HDD serial number -----

void printHDDserial()
{

  if ( geteuid() > 0 )
  {

    std::cerr << "You must be root to retrieve HDD serial number" << std::endl;

  }
  else
  {

    int fd = open( "/dev/sda", O_RDONLY | O_NONBLOCK );

    if ( fd >= 0 )
    {

      struct hd_driveid id;

      if ( ioctl( fd, HDIO_GET_IDENTITY, &id ) >= 0 )
      {
 
        std::cout << "HDD :\tModel " << id.model << std::endl;
        std::cout << "\tSerial Number " << id.serial_no << std::endl;
 
      }

    }

  }

}


int main()
{

  std::cout << "----- METHOD 1 -----" << std::endl;

  printIPv4Information();
  printIPv6Information();

  std::cout << std::endl << "----- METHOD 2 -----" << std::endl;
  printMethod2();

  std::cout << std::endl << "----- METHOD 3 -----" << std::endl;
  printMethod3();

  std::cout << std::endl << "----- HDD Serial Nb -----" << std::endl;
  printHDDserial();

  return EXIT_SUCCESS;

}
