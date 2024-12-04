#ifndef _gkg_processing_rpc_RpcTransferVolumeCallback_h_
#define _gkg_processing_rpc_RpcTransferVolumeCallback_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include <gkg-processing-container/Volume.h>


namespace gkg
{


template < class T >
class RpcTransferVolumeCallback : public RpcCallback
{

  public:
  
    RpcTransferVolumeCallback( int32_t id, 
                               Volume< T >& volume, 
			       void* parameter );
    
    void receive( SocketStream& stream );
    
  protected:
  
    Volume< T >& _volume;

};


}


#endif
