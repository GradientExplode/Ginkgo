#ifndef _gkg_processing_socket_SocketTransferVector3dFrame_h_
#define _gkg_processing_socket_SocketTransferVector3dFrame_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-communication-socket/SocketFrame.h>


namespace gkg
{


template < class T >
class SocketTransferVector3dFrame : public SocketFrame
{

  public:

    SocketTransferVector3dFrame( int32_t id, const Vector3d< T >& vector );

    void sendData( SocketStream& server );

  private:

    const Vector3d< T >& _vector;

};


}


#endif
