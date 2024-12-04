#ifndef _gkg_processing_plugin_functors_DicomProcessingUnit_DicomProcessingUnit_h_
#define _gkg_processing_plugin_functors_DicomProcessingUnit_DicomProcessingUnit_h_


#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuCommandId.h>
#include <gkg-communication-tcpip/HostPort.h>
#include <gkg-core-pattern/Singleton.h>

#include <map>


namespace gkg
{


class RpcCommandWriter;


class DicomProcessingUnit : public Singleton< DicomProcessingUnit >
{

  public:
  
    ~DicomProcessingUnit();
    
    void addWriter( const std::string& host, int32_t port );
    void removeWriter( const std::string& host, int32_t port );
    void send( const std::string& folderName );
    void send( gkg::DpuCommandId commandId );
    
  private:
  
    friend class Singleton< DicomProcessingUnit >;

    std::map< HostPort, RpcCommandWriter*, HostPortCompare > _writers;

};


}


#endif
