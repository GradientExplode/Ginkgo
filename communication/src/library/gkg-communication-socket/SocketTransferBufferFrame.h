#ifndef _gkg_communication_socket_SocketTransferBufferFrame_h_
#define _gkg_communication_socket_SocketTransferBufferFrame_h_


#include <gkg-communication-socket/SocketFrame.h>


namespace gkg
{


template < class T >
class SocketTransferBufferFrame : public SocketFrame
{

  public:

    SocketTransferBufferFrame();
    SocketTransferBufferFrame( int32_t id, 
                               const T* buffer, 
                               int64_t elementCount );
    SocketTransferBufferFrame( const SocketTransferBufferFrame& other );

    void sendData( SocketStream& server );

  protected:

    const T* _buffer;

};


}


#endif
