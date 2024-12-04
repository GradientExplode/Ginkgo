#ifndef _gkg_processing_plugin_functors_DicomProcessingUnit_DpuUnregisterCallback_h_
#define _gkg_processing_plugin_functors_DicomProcessingUnit_DpuUnregisterCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class DpuUnregisterCallback : public RpcCallback
{

  public:
  
    DpuUnregisterCallback( int32_t id, void* parameter );
    
    void receive( SocketStream& stream );

};


}


#endif
