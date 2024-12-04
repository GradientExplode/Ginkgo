#ifndef _gkg_communication_socket_SocketStream_h_
#define _gkg_communication_socket_SocketStream_h_


#include <gkg-communication-socket/SocketBuffer.h>


namespace gkg
{


// modify IOStream to store a SocketBuffer and provide operations on the
// SocketBuffer, therefore specializing IOStream to socket requests
class SocketStream : public /*virtual*/ IOStream
{

  public:

    SocketStream( SocketBuffer* buffer );
    ~SocketStream();

    void listen();
    void connect();
    int32_t accept();
    void close();
    void setNonBlocking( bool value );
    void setVerbose( bool value );
    void negotiate( bool binary );

    void setPacketsSize( int32_t packetsSize );
    void writePackets( const char* buffer, int64_t count );
    void readPackets( char* buffer, int64_t count );

    SocketBuffer* getSocketBuffer();

  protected:

    void verify( int32_t );

    SocketBuffer* _buffer; // streambuf specialized to socket requests
    int32_t _packetsSize;

};


}


#endif
