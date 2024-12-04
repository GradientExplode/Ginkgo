#ifndef _gkg_communication_socket_SocketBuffer_h_
#define _gkg_communication_socket_SocketBuffer_h_


#include <string>
#include <gkg-communication-socket/IOStream.h>
#include <gkg-core-cppext/FStream.h>


namespace gkg
{


// specialize std::streambuf to sending and receiving requests to and
// from remote machines.
class SocketBuffer : public FStream
{

  public:

    enum 
    {

      anyport = 0

    };

    SocketBuffer( int32_t fd, FStream::OpenMode mode );
    virtual ~SocketBuffer();

    void setStream( IOStream* stream );

    int32_t getFd();
    bool isOpened();
    bool isNonBlocking();

    SocketBuffer* listen();
    SocketBuffer* connect();
    SocketBuffer* accept( int32_t& fd );
    SocketBuffer* close();
    SocketBuffer* setNonBlocking( bool value );
    SocketBuffer* setVerbose( bool value );

  protected:

    void setError( const std::string& message );
    void setSysError( const std::string& message );
    IOStream& getStream();

    int32_t _fd;                // my IPC connection's file descriptor
    IOStream* _stream;          // reference to the stream that uses me
    bool _opened;		// do I have an open file descriptor?
    bool _close;                // should I close my file descriptor on exit?
    bool _nonBlocking;          // can I read or write without blocking?
    bool _verbose;              // should I print system error messages?

};


}


#endif
