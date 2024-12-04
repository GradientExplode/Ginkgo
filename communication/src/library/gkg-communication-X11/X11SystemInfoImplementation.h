#ifndef _gkg_communication_X11_X11SystemInfoImplementation_h_
#define _gkg_communication_X11_X11SystemInfoImplementation_h_


#include <gkg-communication-sysinfo/SystemInfoImplementation.h>


namespace gkg
{


class X11SystemInfoImplementation : public SystemInfoImplementation
{

  public:

    X11SystemInfoImplementation( SystemInfo* systemInfo );
    ~X11SystemInfoImplementation();

    int32_t getCpuCount() const;
    uint32_t getCurrentThreadId() const;

    void getMemory( uint64_t& ram, uint64_t& freeRam, uint64_t& swap ) const;
    uint64_t getMaximumMemoryPerProcess() const;
    uint64_t getProcessMemoryUsage() const;

    std::map< std::string, uint64_t > getMacAddresses() const;
    uint64_t getMacAddress( const std::string& ip ) const;
    std::set< std::string > getIpAddresses() const;

    std::string getUserLogin() const;

    int32_t getTerminalLineCount() const;
    int32_t getTerminalColumnCount() const;

    std::vector< std::pair< uint64_t, std::string > >
      getCurrentProcessAndParentInformation() const;

    static X11SystemInfoImplementation*
             getImplementation( SystemInfo* systemInfo );

};


}


#endif

