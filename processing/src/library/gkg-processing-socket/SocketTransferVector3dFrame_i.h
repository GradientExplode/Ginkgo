#ifndef _gkg_processing_socket_SocketTransferVector3dFrame_i_h_
#define _gkg_processing_socket_SocketTransferVector3dFrame_i_h_


#include <gkg-processing-socket/SocketTransferVector3dFrame.h>


template < class T >
gkg::SocketTransferVector3dFrame< T >::SocketTransferVector3dFrame( 
                                              int32_t id, 
                                              const gkg::Vector3d< T >& vector )
                                      : gkg::SocketFrame( id,
                                                          (int32_t)sizeof( T ),
                                                          3 ),
                                        _vector( vector )
{
}


template < class T >
void gkg::SocketTransferVector3dFrame< T >::sendData( 
                                                     gkg::SocketStream& server )
{

  server.write( (const char*)&_vector.x, sizeof( T ) );
  server.write( (const char*)&_vector.y, sizeof( T ) );
  server.write( (const char*)&_vector.z, sizeof( T ) );

}


#endif
