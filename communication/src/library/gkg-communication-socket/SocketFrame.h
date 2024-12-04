#ifndef _gkg_communication_socket_SocketFrame_h_
#define _gkg_communication_socket_SocketFrame_h_


#include <gkg-communication-socket/SocketStream.h>


namespace gkg
{


class SocketFrame
{

  public:

    SocketFrame();
    SocketFrame( int32_t id, int32_t elementSize, int64_t elementCount );
    virtual ~SocketFrame();

    int32_t getId();

    void send( SocketStream& server );
    virtual void sendData( SocketStream& server ) = 0;

  protected:

    int32_t _id;
    int32_t _elementSize;
    int64_t _elementCount;

};


}


#endif
