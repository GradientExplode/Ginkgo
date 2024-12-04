#ifndef _gkg_communication_sysinfo_SystemInfo_h_
#define _gkg_communication_sysinfo_SystemInfo_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <string>


namespace gkg
{


class SystemInfoImplementation;


class SystemInfo : public Singleton< SystemInfo >
{

  public:

    virtual ~SystemInfo();


    virtual int32_t getCpuCount() const;
    virtual uint32_t getCurrentThreadId() const;

    virtual void getMemory( uint64_t& ram,
                            uint64_t& freeRam,
                            uint64_t& swap ) const;
    virtual uint64_t getMaximumMemoryPerProcess() const;
    virtual uint64_t getProcessMemoryUsage() const;

    virtual std::map< std::string, uint64_t > getMacAddresses() const;
    virtual uint64_t getMacAddress( const std::string& ip ) const;
    virtual std::set< std::string > getIpAddresses() const;

    virtual std::string getUserLogin() const;

    int32_t getTerminalLineCount() const;
    int32_t getTerminalColumnCount() const;

    virtual bool hasCudaDevice() const;

    virtual std::vector< std::pair< uint64_t, std::string > >
              getCurrentProcessAndParentInformation() const;

    SystemInfoImplementation* getImplementation() const;

  protected:

    friend class Singleton< SystemInfo >;

    SystemInfo();

    SystemInfoImplementation* _systemInfoImplementation;

};


}


#endif
