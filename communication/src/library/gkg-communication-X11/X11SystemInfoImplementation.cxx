#include <gkg-communication-X11/X11SystemInfoImplementation.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-exception/Exception.h>

#if defined(__FreeBSD__)

// for sysctl() on FreeBSD
#include <sys/types.h>
#include <sys/sysctl.h>

#elif defined(__OpenBSD__) || defined(__NetBSD__)

// for sysctl() on OpenBSD and netBSD
#include <sys/param.h>
#include <sys/sysctl.h>

#elif defined(__hpux)
// for pstat_getdynamic() on HP-UX
#include <sys/param.h>
#include <sys/pstat.h>

#else

// for sysconf() on Linux, Solaris, IRIX, and Tru64
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#endif

#include <pthread.h>
#include <fstream>
#include <cstring>

#if ( defined(__APPLE__) && defined(__MACH__) )

#include <mach/mach.h>
#include <sys/mount.h>

#endif

#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <list>


////////////////////////////////////////////////////////////////////////////////
// static method(s)
////////////////////////////////////////////////////////////////////////////////

static void getProcessName( const pid_t pid, char* name )
{

  try
  {

    char procfile[ BUFSIZ ];
    sprintf( procfile, "/proc/%d/cmdline", pid );
    FILE* f = fopen( procfile, "r" );
    if ( f )
    {

      size_t size;
      size = fread(name, sizeof( char ), sizeof( procfile ), f );
      if ( size > 0 )
      {

        if ( name[size - 1] == '\n' )
        {

          name[ size - 1 ] = '\0';

        }

      }
      fclose(f);

    }

  }
  GKG_CATCH( "static void getProcessName( const pid_t pid, char* name )" );

}


static void getProcessParentId( const pid_t pid, pid_t* ppid )
{

  try
  {
    char buffer[ BUFSIZ ];
    sprintf( buffer, "/proc/%d/stat", pid );
    FILE* fp = fopen( buffer, "r" );
    if ( fp )
    {

      size_t size = fread( buffer, sizeof( char ), sizeof( buffer ), fp );
      if ( size > 0 )
      {

        strtok( buffer, " " );              // (1) pid  %d
        strtok( NULL, " " );                // (2) comm  %s
        strtok( NULL, " " );                // (3) state  %c
        char* s_ppid = strtok( NULL, " " ); // (4) ppid  %d
        *ppid = atoi( s_ppid );

      }
      fclose(fp);

    }

  }
  GKG_CATCH( "static void getProcessParentId( const pid_t pid, pid_t* ppid )" );

}



////////////////////////////////////////////////////////////////////////////////
// class X11SystemInfoImplementation
////////////////////////////////////////////////////////////////////////////////

gkg::X11SystemInfoImplementation::X11SystemInfoImplementation(
                                                   gkg::SystemInfo* systemInfo )
                              : gkg::SystemInfoImplementation( systemInfo )
{
}


gkg::X11SystemInfoImplementation::~X11SystemInfoImplementation()
{
}


int32_t gkg::X11SystemInfoImplementation::getCpuCount() const
{

  try
  {

    // get the number of CPU
    int32_t nCpu = 1;

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)

    // total number of processors, FreeBSD, OpenBSD, and NetBSD
    int32_t mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    size_t len = sizeof( nCpu );
    sysctl( mib, 2, &nCpu, &len, NULL, 0 );

#elif defined(__hpux) 

    // number of processors online, HP-UX
    struct pst_dynamic psd; 
    pstat_getdynamic(&psd, sizeof(psd), 1, 0);
    nCpu = (int32_t)psd.psd_proc_cnt;

#elif defined(__linux) || defined(__sun) || defined(__osf)

    // number of processors online, Linux, Solaris, and Tru64
    nCpu = (int32_t)sysconf( _SC_NPROCESSORS_ONLN );

#elif ( defined(__APPLE__) && defined(__MACH__) )

    // number of processors online, MacOS (should be the same as Linux)
    nCpu = (int32_t)sysconf( _SC_NPROCESSORS_ONLN );

#elif defined(__sgi)

    // number of processors online, IRIX
    nCpu = (int32_t)sysconf( _SC_NPROC_ONLN );

#endif

    return nCpu;

  }
  GKG_CATCH( "int32_t "
             "gkg::X11SystemInfoImplementation::getCpuCount() const" );

}


uint32_t gkg::X11SystemInfoImplementation::getCurrentThreadId() const
{

  try
  {

    // getting the current thread id
#if ( defined(__APPLE__) && defined(__MACH__) )

    // note: directly convert to int32_t causes an error -> lose precision
    return ( uint32_t )( uint64_t )pthread_self();

#else

    return ( uint32_t )pthread_self();

#endif

  }
  GKG_CATCH( "uint32_t "
             "gkg::X11SystemInfoImplementation::getCurrentThreadId() const" );

}


void gkg::X11SystemInfoImplementation::getMemory(
                        uint64_t& ram, uint64_t& freeRam, uint64_t& swap ) const
{

  try
  {

#if ( defined(__APPLE__) && defined(__MACH__) )

    uint64_t totalMemory = 0;
    uint64_t freeMemory = 0;
    uint64_t usedMemory = 0;
    uint64_t swapFree = 0;

    // processing free memory
    vm_size_t pageSize;
    mach_port_t machPort;
    mach_msg_type_number_t count;
    vm_statistics_data_t vmStatistics; 

    machPort = mach_host_self();
    count = sizeof( vmStatistics ) / sizeof( natural_t );
    if ( ( KERN_SUCCESS == host_page_size( machPort, &pageSize ) ) &&
         ( KERN_SUCCESS == host_statistics( machPort, HOST_VM_INFO,
                                            ( host_info_t )&vmStatistics,
                                            &count ) ) )
    {

      freeMemory = ( uint64_t )vmStatistics.free_count * ( uint64_t )pageSize;

      usedMemory = ( ( uint64_t )vmStatistics.active_count +
                     ( uint64_t )vmStatistics.inactive_count +
                     ( uint64_t )vmStatistics.wire_count ) *
                   ( uint64_t )pageSize;

    }

    // processing total memory
    totalMemory = freeMemory + usedMemory;

    // processing swap free memory (note: MacOS does not use a preallocated
    // swap partition for virtual memory, instead it uses all of the available
    // space on the machine's boot partition)
    struct statfs fileSystemStatistics;
    if ( statfs( "/", &fileSystemStatistics ) == 0 )
    {

      swapFree = ( uint64_t )fileSystemStatistics.f_bsize *
                 ( uint64_t )fileSystemStatistics.f_bfree;

    }

    // processing RAM / free RAM / swap
    ram = totalMemory;
    freeRam = freeMemory;
    swap = swapFree;

#else

    // opening /proc/meminfo
    std::ifstream is( "/proc/meminfo" );
    if ( !is )
    {

      throw std::runtime_error( "unable to open /proc/meminfo" );

    }

    // looping over lines of /proc/meminfo
    char buffer[ 200 ];
    int32_t infoCount = 0;
    uint64_t totalMemory = 0;
    uint64_t freeMemory = 0;
    uint64_t buffers = 0;
    uint64_t cached = 0;
    uint64_t swapFree = 0;
    while ( ( infoCount < 5 ) && !is.eof() )
    {

      // reading lines
      is.getline( buffer, 200 );

      // total memory
      if ( !strncmp( buffer, "MemTotal:", 9 ) )
      {

        std::string MemTotal( buffer + 9 );
        if ( MemTotal.substr( MemTotal.length() - 3U, 3U ) == " kB" )
        {

          MemTotal = MemTotal.substr( 0U, MemTotal.length() - 3 );

        }
        while ( MemTotal[ 0 ] == ' ' )
        {

           MemTotal = MemTotal.substr( 1, MemTotal.length() - 1 );

        }
        gkg::StringConverter::toScalar( MemTotal, totalMemory );
        ++ infoCount;

      }

      // free memory
      if ( !strncmp( buffer, "MemFree:", 8 ) )
      {

        std::string MemFree( buffer + 8 );
        if ( MemFree.substr( MemFree.length() - 3U, 3U ) == " kB" )
        {

          MemFree = MemFree.substr( 0U, MemFree.length() - 3 );

        }
        while ( MemFree[ 0 ] == ' ' )
        {

           MemFree = MemFree.substr( 1, MemFree.length() - 1 );

        }
        gkg::StringConverter::toScalar( MemFree, freeMemory );
        ++ infoCount;

      }

      // buffers
      if ( !strncmp( buffer, "Buffers:", 8 ) )
      {

        std::string Buffers( buffer + 8 );
        if ( Buffers.substr( Buffers.length() - 3U, 3U ) == " kB" )
        {

          Buffers = Buffers.substr( 0U, Buffers.length() - 3 );

        }
        while ( Buffers[ 0 ] == ' ' )
        {

           Buffers = Buffers.substr( 1, Buffers.length() - 1 );

        }
        gkg::StringConverter::toScalar( Buffers, buffers );
        ++ infoCount;

      }

      // cached
      if ( !strncmp( buffer, "Cached:", 7 ) )
      {

        std::string Cached( buffer + 7 );
        if ( Cached.substr( Cached.length() - 3U, 3U ) == " kB" )
        {

          Cached = Cached.substr( 0U, Cached.length() - 3 );

        }
        while ( Cached[ 0 ] == ' ' )
        {

           Cached = Cached.substr( 1, Cached.length() - 1 );

        }
        gkg::StringConverter::toScalar( Cached, cached );
        ++ infoCount;

      }

      // swap free
      if ( !strncmp( buffer, "SwapFree:", 9 ) )
      {

        std::string SwapFree( buffer + 9 );
        if ( SwapFree.substr( SwapFree.length() - 3U, 3U ) == " kB" )
        {

          SwapFree = SwapFree.substr( 0U, SwapFree.length() - 3 );

        }
        while ( SwapFree[ 0 ] == ' ' )
        {

           SwapFree = SwapFree.substr( 1, SwapFree.length() - 1 );

        }
        gkg::StringConverter::toScalar( SwapFree, swapFree );
        ++ infoCount;

      }

    }

    // closing /proc/meminfo
    is.close();

    // sanity check
    if ( infoCount < 5 )
    {

      throw std::runtime_error( "unable to get all memory information" );

    }

    // processing RAM / free RAM / swap
    ram = totalMemory;
    freeRam = freeMemory + buffers + cached;
    swap = swapFree + freeRam;

    // converting from MBytes to Bytes
    ram <<= 10;
    freeRam <<= 10;
    swap <<= 10;

#endif

  }
  GKG_CATCH( "void gkg::X11SystemInfoImplementation::getMemory( "
             "uint64_t& ram, uint64_t& freeRam, uint64_t& swap ) const" );

}


uint64_t gkg::X11SystemInfoImplementation::getMaximumMemoryPerProcess() const
{

  try
  {

    // probing the address space limits in bytes
    rlimit value;
    if ( getrlimit( RLIMIT_AS, &value ) )
    {

      throw std::runtime_error(
                               "problem when probing the address space limit" );

    }
    uint64_t maximumMemoryPerProcess = ( uint64_t )value.rlim_max;

    return maximumMemoryPerProcess;

    //return 1U << 31;

  }
  GKG_CATCH( "uint64_t "
             "gkg::X11SystemInfoImplementation::getMaximumMemoryPerProcess() "
             "const" );

}


uint64_t gkg::X11SystemInfoImplementation::getProcessMemoryUsage() const
{

  try
  {

#if ( defined(__APPLE__) && defined(__MACH__) )

    task_t task = MACH_PORT_NULL;

    struct task_basic_info taskInfo;
    mach_msg_type_number_t taskInfoCount = TASK_BASIC_INFO_COUNT;

    if ( KERN_SUCCESS != task_info( mach_task_self(),
                                    TASK_BASIC_INFO,
                                    ( task_info_t )&taskInfo,
                                    &taskInfoCount ) )
    {

      throw std::runtime_error( "unable to get memory usage" );

    }

    return ( uint64_t )taskInfo.resident_size;

#else

    std::string statFileName = std::string( "/proc/" ) +
                               gkg::StringConverter::toString( 
                                 ( uint64_t )getpid() ) + "/statm";

    // opening /proc/meminfo
    std::ifstream is( statFileName.c_str() );
    if ( !is.good() )
    {

      throw std::runtime_error( std::string( "unable to open " ) +
                                statFileName );

    }

    uint64_t processMemoryUsage = 0U;
    is >> processMemoryUsage;

    is.close();

    return processMemoryUsage * ( uint64_t )getpagesize();

#endif

  }
  GKG_CATCH( "uint64_t "
             "gkg::X11SystemInfoImplementation::getProcessMemoryUsage() "
             "const" );

}


std::map< std::string, uint64_t > 
gkg::X11SystemInfoImplementation::getMacAddresses() const
{

  try
  {

    std::map< std::string, uint64_t > macAddresses;
    struct ifaddrs* addrs;

    if ( !getifaddrs( &addrs ) )
    {

      struct ifaddrs* ifa = addrs;

      while ( ifa )
      {

        std::string ifaName( ifa->ifa_name );

        if ( ifa->ifa_addr && 
             !( ifa->ifa_flags & IFF_LOOPBACK ) &&
             ( ifa->ifa_addr->sa_family == AF_PACKET ) &&
             ( ifaName.find( "docker" ) == std::string::npos ) )
        {

          struct ifreq ifr;
          int s = socket( AF_INET, SOCK_DGRAM, 0 );

          if ( s >= 0 )
          {

            ifr.ifr_addr.sa_family = AF_INET;
            std::strcpy( ifr.ifr_name, ifa->ifa_name );

            if ( ioctl( s, SIOCGIFHWADDR, &ifr ) >= 0 )
            {

              uint64_t macAddress = 0;
              int j;

              close( s );

              for ( j = 0; j < 6; j++ )
              {

                macAddress |=
                 ( uint64_t )( ( unsigned char* )ifr.ifr_hwaddr.sa_data )[ j ]
                 << ( 8 * ( 5 - j ) );

              }

              macAddresses.insert( std::make_pair( 
                                                 std::string( ifa->ifa_name ),
                                                 macAddress ) );

            }

          }

        }

        ifa = ifa->ifa_next;

      }

      freeifaddrs( addrs );

    }

    return macAddresses;

  }
  GKG_CATCH( "std::set< uint64_t > "
             "gkg::X11SystemInfoImplementation::getMacAddresses() const" );

}


uint64_t 
gkg::X11SystemInfoImplementation::getMacAddress( const std::string& ip ) const
{

  try
  {

    // getting a socket handler
    int32_t fd = socket( AF_INET, SOCK_DGRAM, 0 );
 
    if ( fd >= 0 )
    {

      /* Query available interfaces. */
      char buf[ 1024 ];
      struct ifconf ifc;
 
      ifc.ifc_len = sizeof( buf );
      ifc.ifc_buf = buf;
 
      if ( ioctl( fd, SIOCGIFCONF, &ifc ) >= 0 )
      {

        // iterating through the list of interfaces
        int32_t i;
        struct ifreq* ifr = ifc.ifc_req;
        int32_t interfaceCount = ifc.ifc_len / sizeof( struct ifreq );
 
        for ( i = 0; i < interfaceCount; i++ )
        {
 
          struct ifreq* item = &ifr[ i ];

          // getting the IP address
          std::string currentIp = inet_ntoa(
                         ( ( struct sockaddr_in* )&item->ifr_addr )->sin_addr );


          if ( currentIp == ip )
          {

            // getting the MAC address
            if ( ioctl( fd, SIOCGIFHWADDR, item ) >= 0 )
            {

              uint64_t macAddress = 0;
              int j;

              for ( j = 0; j < 6; j++ )
              {

                macAddress |=
                 ( uint64_t )( ( unsigned char* )item->ifr_hwaddr.sa_data )[ j ]
                 << ( 8 * ( 5 - j ) );

              }

              return macAddress;

            }

          }

        }

      }

    }
    return ( uint64_t )0;

  }
  GKG_CATCH( "uint64_t "
             "gkg::X11SystemInfoImplementation::getMacAddress( "
             "std::string ip ) const" );

}


std::set< std::string > gkg::X11SystemInfoImplementation::getIpAddresses() const
{

  try
  {

    // allocating set
    std::set< std::string > ipAddresses;

    // getting a socket handler
    int32_t fd = socket( AF_INET, SOCK_DGRAM, 0 );
 
    if ( fd >= 0 )
    {

      /* Query available interfaces. */
      char buf[ 1024 ];
      struct ifconf ifc;
 
      ifc.ifc_len = sizeof( buf );
      ifc.ifc_buf = buf;
 
      if ( ioctl( fd, SIOCGIFCONF, &ifc ) >= 0 )
      {

        // iterating through the list of interfaces
        int32_t i;
        struct ifreq* ifr = ifc.ifc_req;
        int32_t interfaceCount = ifc.ifc_len / sizeof( struct ifreq );
 
        for ( i = 0; i < interfaceCount; i++ )
        {
 
          struct ifreq* item = &ifr[ i ];
          std::string ifName( item->ifr_name );

          // getting the IP address
          std::string ip = inet_ntoa(
                         ( ( struct sockaddr_in* )&item->ifr_addr )->sin_addr );


          if ( ( ip != "0.0.0.0" ) && ( ip != "127.0.0.1" ) &&
               ( ifName.find( "docker" ) == std::string::npos ) )
          {

            ipAddresses.insert( ip );

          }

        }

      }

    }

    return ipAddresses;

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::X11SystemInfoImplementation::getIpAddresses() const" );

}


std::string gkg::X11SystemInfoImplementation::getUserLogin() const
{

  try
  {

    char buffer[ 256 ];

    int32_t value = getlogin_r( buffer, 256 );

    if ( value )
    {

      return "Unknown";

    }

    return std::string( buffer );

  }
  GKG_CATCH( "std::string "
             "gkg::X11SystemInfoImplementation::getUserLogin() const" );

}


int32_t gkg::X11SystemInfoImplementation::getTerminalLineCount() const
{

  try
  {

    struct winsize w;

    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );

    return (int32_t)w.ws_row;

  }
  GKG_CATCH( 
     "int32_t gkg::X11SystemInfoImplementation::getTerminalLineCount() const" );

}


int32_t gkg::X11SystemInfoImplementation::getTerminalColumnCount() const
{

  try
  {

    struct winsize w;

    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );

    return (int32_t)w.ws_col;

  }
  GKG_CATCH( 
   "int32_t gkg::X11SystemInfoImplementation::getTerminalColumnCount() const" );

}


std::vector< std::pair< uint64_t, std::string > >
gkg::X11SystemInfoImplementation::getCurrentProcessAndParentInformation() const
{

  try
  {

    std::list< std::pair< uint64_t, std::string > > processAndParentInformation;

    pid_t pid = getpid();
    while ( pid != 0 )
    {

      char name[ BUFSIZ ];
      getProcessName( pid, name );
      processAndParentInformation.push_back(
        std::pair< uint64_t, std::string >( ( uint64_t )pid,
                                            std::string( name ) ) );
      getProcessParentId( pid, &pid );

    }

    return std::vector< std::pair< uint64_t, std::string > >(
                                            processAndParentInformation.begin(),
                                            processAndParentInformation.end() );

  }
  GKG_CATCH( "std::vector< std::pair< uint64_t, std::string > > "
             "gkg::SystemInfo::getCurrentProcessAndParentInformation() const" );

}




gkg::X11SystemInfoImplementation*
gkg::X11SystemInfoImplementation::getImplementation(
                                                   gkg::SystemInfo* systemInfo )
{

  return static_cast< gkg::X11SystemInfoImplementation* >(
                                              systemInfo->getImplementation() );

}
