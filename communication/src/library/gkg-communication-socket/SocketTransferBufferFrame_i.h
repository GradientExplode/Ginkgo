#ifndef _gkg_communication_socket_SocketTransferBufferFrame_i_h_
#define _gkg_communication_socket_SocketTransferBufferFrame_i_h_


#include <gkg-communication-socket/SocketTransferBufferFrame.h>


template < class T >
gkg::SocketTransferBufferFrame< T >::SocketTransferBufferFrame()
                                    : gkg::SocketFrame(),
                                      _buffer( 0 )
{
}


template < class T >
gkg::SocketTransferBufferFrame< T >::SocketTransferBufferFrame( 
                                                          int32_t id,
                                                          const T* buffer,
                                                          int64_t elementCount )
                                    : gkg::SocketFrame( id,
                                                        (int32_t)sizeof( T ),
                                                        elementCount ),
                                      _buffer( buffer )
{
}


template < class T >
gkg::SocketTransferBufferFrame< T >::SocketTransferBufferFrame( 
                              const gkg::SocketTransferBufferFrame< T >& other )
                                    : gkg::SocketFrame( other._id,
                                                        (int32_t)sizeof( T ),
                                                        other._elementCount ),
                                      _buffer( other._buffer )
{
}


template < class T >
void gkg::SocketTransferBufferFrame< T >::sendData( gkg::SocketStream& server )
{

  server.writePackets( (const char*)_buffer, _elementCount * sizeof( T ) );

}


#endif
