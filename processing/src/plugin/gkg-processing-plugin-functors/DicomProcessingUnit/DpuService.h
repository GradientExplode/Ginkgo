#ifndef _gkg_processing_plugin_functors_DicomProcessingUnit_DpuService_h_
#define _gkg_processing_plugin_functors_DicomProcessingUnit_DpuService_h_


#include <gkg-communication-socket/SocketService.h>
#include <gkg-communication-rpc/RpcCommandReader.h>


namespace gkg
{


class DpuService : public SocketService
{

  public:
  
    DpuService( int32_t port );
    ~DpuService();
    
  protected:
  
    void createReader( int32_t fd );
    
  private:
  
    gkg::RpcCommandReader* _reader;

};


}


#endif
