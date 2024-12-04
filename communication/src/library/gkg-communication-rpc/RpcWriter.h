#ifndef _gkg_communication_rpc_RpcWriter_h_
#define _gkg_communication_rpc_RpcWriter_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-communication-socket/SocketWriter.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <string>


namespace gkg
{


// write RPC requests to a server; derived classes should add member
// functions corresponding to the RPC service's protocol

class RpcWriter : public SocketWriter
{

  protected:

    RpcWriter( const std::string& path, 
               bool fatal, 
               bool binary,
               bool noDelay = false );
    RpcWriter( const std::string& host, 
               in_port_t port, 
               bool fatal, 
               bool binary,
               bool noDelay = false );
    RpcWriter( int32_t fd, bool fatal, bool binary, bool noDelay = false );
    RpcWriter( SocketStream* server, bool noDelay = false );

    void setProlog( const std::string& prolog );
    void setEpilog( const std::string& epilog );

    std::string _prolog;       // prolog that begins request
    std::string _epilog;       // epilog that finishes request

  private:

    // deny access since unimplemented and member-wise won't work
    RpcWriter( const RpcWriter& );
    RpcWriter& operator=( const RpcWriter& );

};


}


#endif
