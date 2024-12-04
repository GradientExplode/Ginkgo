#ifndef _gkg_graphics_X11_GkgTestRpcClient02_Rpc_h
#define _gkg_graphics_X11_GkgTestRpcClient02_Rpc_h


#include <gkg-communication-rpc/RpcWriter.h>


class Writer : public gkg::RpcWriter
{

  public:

    enum { STRING, INTEGER, NFCNS };

    Writer( in_port_t port );
    void send( char* msg );
    void send( int32_t i );
    void flush();

  protected:

    bool getFatal();
    bool getBinary();

};


#endif
