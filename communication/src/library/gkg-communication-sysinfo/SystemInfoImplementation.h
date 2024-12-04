#ifndef _gkg_communication_sysinfo_SystemInfoImplementation_h_
#define _gkg_communication_sysinfo_SystemInfoImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <string>


namespace gkg
{


class SystemInfo;


class SystemInfoImplementation
{

  public:

    virtual ~SystemInfoImplementation();

    SystemInfo* getTarget() const;

    virtual int32_t getCpuCount() const = 0;
    virtual uint32_t getCurrentThreadId() const = 0;

    virtual void getMemory( uint64_t& ram,
                            uint64_t& freeRam,
                            uint64_t& swap ) const = 0;
    virtual uint64_t getMaximumMemoryPerProcess() const = 0;
    virtual uint64_t getProcessMemoryUsage() const = 0;

    virtual std::map< std::string, uint64_t > getMacAddresses() const = 0;
    virtual uint64_t getMacAddress( const std::string& ip ) const = 0;
    virtual std::set< std::string > getIpAddresses() const = 0;

    virtual std::string getUserLogin() const = 0;

    virtual int32_t getTerminalLineCount() const = 0;
    virtual int32_t getTerminalColumnCount() const = 0;

    virtual std::vector< std::pair< uint64_t, std::string > >
              getCurrentProcessAndParentInformation() const = 0;

  protected:

    SystemInfoImplementation( SystemInfo* systemInfo );

    SystemInfo* _target;

};


}


#endif

