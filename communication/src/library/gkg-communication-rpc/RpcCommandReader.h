#ifndef _gkg_communication_rpc_RpcCommandReader_h_
#define _gkg_communication_rpc_RpcCommandReader_h_


#include <gkg-communication-rpc/RpcCallback.h>
#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/SocketStream.h>
#include <map>


namespace gkg
{


class RpcCommandReader : public RpcReader
{

  public:
  
    RpcCommandReader( int32_t port );
    ~RpcCommandReader();

    void add( RpcCallback* callback );
    void remove( RpcCallback* callback );

    RpcCallback* getCallback( int32_t id );

  protected:  
  
    enum
    {
    
      RECEIVE,
      CALLBACK_COUNT
    
    }; 
    
    static void receive( SocketReader* reader,
                         SocketHeader& header,
			 SocketStream& stream );

    void connectionClosed( int32_t fd );
    bool getBinary();
 
  private:
   
   std::map< int32_t, RpcCallback* > _callbacks;
   
};


}


#endif
