#ifndef _gkg_graphics_X11_GkgTestRpcServer02_Rpc_h
#define _gkg_graphics_X11_GkgTestRpcServer02_Rpc_h

#include <gkg-communication-rpc/RpcReader.h>
#include <gkg-communication-socket/SocketService.h>


class Reader : public gkg::RpcReader
{

  public:

    Reader( int32_t port, gkg::SocketService* service );
    virtual ~Reader();

  protected:

    enum { STRING, INTEGER, NFCNS };

    virtual void connectionClosed( int32_t fd );
    static void receiveString( gkg::SocketReader* reader,
                               gkg::SocketHeader& header,
                               gkg::SocketStream& stream );
    static void receiveInt( gkg::SocketReader* reader,
                            gkg::SocketHeader& header,
                            gkg::SocketStream& stream );
    bool getBinary();

    gkg::SocketService* _service;

};


class Service : public gkg::SocketService 
{

  public:

    Service( int32_t port );
    virtual ~Service();

  protected:

    virtual void createReader( int32_t fd );

  protected:

    Reader* _reader;

};


#endif
