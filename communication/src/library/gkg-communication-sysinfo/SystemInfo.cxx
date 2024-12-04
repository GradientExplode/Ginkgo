#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-communication-sysinfo/SystemInfoImplementation.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-core-exception/Exception.h>

#ifdef GKG_CUDA
#include <gkg-communication-cuda/CudaDevices.h>
#endif

#include <iostream>


gkg::SystemInfo::SystemInfo()
{

  try
  {

    _systemInfoImplementation = gkg::getCommunicationImplementationFactory()
                                        .createSystemInfoImplementation( this );

  }
  GKG_CATCH( "gkg::SystemInfo::SystemInfo()" );

}


gkg::SystemInfo::~SystemInfo()
{

  delete _systemInfoImplementation;

}


int32_t gkg::SystemInfo::getCpuCount() const
{

  try
  {

    int32_t numberOfCpuToUse = 
                    gkg::ConfigurationInfo::getInstance().getNumberOfCpuToUse();
    int32_t cpuCount = _systemInfoImplementation->getCpuCount();

    if ( !numberOfCpuToUse ||
         ( numberOfCpuToUse > cpuCount ) )
    {

      numberOfCpuToUse = cpuCount;

    }

    return numberOfCpuToUse;

  }
  GKG_CATCH( "int32_t gkg::SystemInfo::getCpuCount() const" );

}


uint32_t gkg::SystemInfo::getCurrentThreadId() const
{

  try
  {

    return _systemInfoImplementation->getCurrentThreadId();

  }
  GKG_CATCH( "uint32_t gkg::SystemInfo::getCurrentThreadId() const" );

}


void gkg::SystemInfo::getMemory( uint64_t& ram,
                                 uint64_t& freeRam,
                                 uint64_t& swap ) const
{

  try
  {

    _systemInfoImplementation->getMemory( ram, freeRam, swap );

  }
  GKG_CATCH( "void gkg::SystemInfo::getMemory( "
             "uint64_t& ram, "
             "uint64_t& freeRam, "
             "uint64_t& swap ) const" );

}


uint64_t gkg::SystemInfo::getMaximumMemoryPerProcess() const
{

  try
  {

    return _systemInfoImplementation->getMaximumMemoryPerProcess();

  }
  GKG_CATCH( "uint64_t gkg::SystemInfo::getMaximumMemoryPerProcess() const" );

}


uint64_t gkg::SystemInfo::getProcessMemoryUsage() const
{

  try
  {

    return _systemInfoImplementation->getProcessMemoryUsage();

  }
  GKG_CATCH( "uint64_t gkg::SystemInfo::getProcessMemoryUsage() const" );

}


std::map< std::string, uint64_t > gkg::SystemInfo::getMacAddresses() const
{

  try
  {

    return _systemInfoImplementation->getMacAddresses();

  }
  GKG_CATCH( "std::map< std::string, uint64_t > "
             "gkg::SystemInfo::getMacAddresses() const" );

}


uint64_t gkg::SystemInfo::getMacAddress( const std::string& ip ) const
{

  try
  {

    return _systemInfoImplementation->getMacAddress( ip );

  }
  GKG_CATCH( "uint64_t gkg::SystemInfo::getMacAddress( "
             "const std::string& ip ) const" );

}


std::set< std::string > gkg::SystemInfo::getIpAddresses() const
{

  try
  {

    return _systemInfoImplementation->getIpAddresses();

  }
  GKG_CATCH( "std::set< std::string > "
             "gkg::SystemInfo::getIpAddresses() const" );

}


std::string gkg::SystemInfo::getUserLogin() const
{

  try
  {

    return _systemInfoImplementation->getUserLogin();

  }
  GKG_CATCH( "std::string gkg::SystemInfo::getUserLogin() const" );

}


int32_t gkg::SystemInfo::getTerminalLineCount() const
{

  try
  {

    return _systemInfoImplementation->getTerminalLineCount();

  }
  GKG_CATCH( "int32_t gkg::SystemInfo::getTerminalLineCount() const" );

}


int32_t gkg::SystemInfo::getTerminalColumnCount() const
{

  try
  {

    return _systemInfoImplementation->getTerminalColumnCount();

  }
  GKG_CATCH( "int32_t gkg::SystemInfo::getTerminalColumnCount() const" );

}


bool gkg::SystemInfo::hasCudaDevice() const
{

  try
  {

#ifdef GKG_CUDA

  return gkg::CudaDevices::getInstance().size() ? true : false;

#else

  return false;

#endif

  }
  GKG_CATCH( "bool gkg::SystemInfo::hasCudaDevice() const" );

}


std::vector< std::pair< uint64_t, std::string > >
gkg::SystemInfo::getCurrentProcessAndParentInformation() const
{

  try
  {

    return _systemInfoImplementation->getCurrentProcessAndParentInformation();

  }
  GKG_CATCH( "std::vector< std::pair< uint64_t, std::string > > "
             "gkg::SystemInfo::getCurrentProcessAndParentInformation() const" );

}


gkg::SystemInfoImplementation* gkg::SystemInfo::getImplementation() const
{

  try
  {

    return _systemInfoImplementation;

  }
  GKG_CATCH( "gkg::SystemInfo::SystemInfo()" );

}
