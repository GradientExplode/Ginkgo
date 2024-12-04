#ifndef _gkg_communication_socket_SocketFrameWriter_h_
#define _gkg_communication_socket_SocketFrameWriter_h_


#include <gkg-communication-socket/SocketWriter.h>
#include <gkg-communication-socket/SocketFrame.h>


namespace gkg
{


class SocketFrameWriter : public SocketWriter
{

  public:

    SocketFrameWriter( const std::string& host,
                       int32_t port,
                       bool noDelay = false );


    void send( SocketFrame& frame );
    void flush();

  protected:

    bool getFatal();
    bool getBinary();

};


}


#endif
