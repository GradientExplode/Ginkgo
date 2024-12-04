#ifndef _gkg_communication_rpc_RpcReader_h_
#define _gkg_communication_rpc_RpcReader_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-communication-socket/SocketReader.h>
#include <string>


namespace gkg
{


// read RPC requests from a client; derived classes initialize the
// function array with addresses of static member functions to
// unmarshall RPC requests and implement the virtual function called
// when the client closes the connection

class RpcReader : public SocketReader
{

  protected:

    RpcReader( SocketStream* client, bool nonBlocking = true );
    RpcReader( int32_t fd, bool binary, bool nonBlocking = true );

    void setProlog( const std::string& prolog );
    void setEpilog( const std::string& epilog );

    virtual int32_t isInputReady( int32_t fd );

    std::string _prolog;    // prolog that begins request
    std::string _epilog;    // epilog that finishes request

  private:

    // deny access since unimplemented and member-wise won't work
    RpcReader( const RpcReader& );
    RpcReader& operator=( const RpcReader& );

};


}


#endif
