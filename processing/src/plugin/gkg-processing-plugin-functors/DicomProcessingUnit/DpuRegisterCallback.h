#ifndef _gkg_processing_plugin_functors_DicomProcessingUnit_DpuRegisterCallback_h_
#define _gkg_processing_plugin_functors_DicomProcessingUnit_DpuRegisterCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>


namespace gkg
{


class DpuRegisterCallback : public RpcCallback
{

  public:
  
    DpuRegisterCallback( int32_t id, void* parameter );
    
    void receive( SocketStream& stream );

};


}


#endif
