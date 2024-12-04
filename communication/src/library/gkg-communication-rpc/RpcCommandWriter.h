#ifndef _gkg_communication_rpc_RpcCommandWriter_h_
#define _gkg_communication_rpc_RpcCommandWriter_h_


#include <gkg-communication-rpc/RpcWriter.h>
#include <gkg-communication-rpc/RpcCommand.h>


namespace gkg
{


class RpcCommandWriter : public RpcWriter
{

  public:
  
    enum
    {
    
      SEND,
      CALLBACK_COUNT
    
    }; 
  
    RpcCommandWriter( const std::string& host, 
                      in_port_t port,
                      bool noDelay = false );
    
    void send( RpcCommand& command );
    void flush();
    
  protected:
  
    bool getFatal();
    bool getBinary();

};


}


#endif
