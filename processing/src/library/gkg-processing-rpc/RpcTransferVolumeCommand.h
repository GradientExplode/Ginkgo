#ifndef _gkg_processing_rpc_RpcCommandTransferVolume_h_
#define _gkg_processing_rpc_RpcCommandTransferVolume_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-communication-rpc/RpcCommand.h>


namespace gkg
{


template < class T >
class RpcTransferVolumeCommand : public RpcCommand
{

  public:
  
    RpcTransferVolumeCommand( int32_t id );
    
    Volume< T >& getVolume();
    
    void send( SocketStream& server );
    
  private:
  
    Volume< T > _volume;

};


}


#endif
