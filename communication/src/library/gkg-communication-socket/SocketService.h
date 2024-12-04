#ifndef _gkg_communication_socket_SocketService_h_
#define _gkg_communication_socket_SocketService_h_


#include <gkg-communication-dispatcher/IOHandler.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <gkg-core-cppext/SysTime.h>
#include <string>


namespace gkg
{


class SocketBuffer;


// support communication between a socket service and its clients; derived 
// classes create readers to read socket requests from connections with clients

class SocketService : public IOHandler 
{

  public:

    virtual ~SocketService();

    void startSocket( in_port_t port = 0 );
    void stopSocket();
    void restartSocket( in_port_t port = 0 );

    void run( timeval* timeout = 0 );
    void quitRunning();

  protected:

    SocketService( in_port_t port );
    SocketService( const std::string& path, in_port_t port = 0 );

    int32_t createSocket();
    void startListening();
    void stopListening();
    virtual int32_t isInputReady( int32_t fd );
    virtual void createReader( int32_t fd ) = 0;

    std::string _path;          // my registration's path
    in_port_t _port;            // my port's address
    int32_t _fd;                // file descriptor for socket
    SocketBuffer* _buffer;      // my network socket
    volatile bool _running;     // am I running my dispatch loop?

  private:

    // deny access since unimplemented and member-wise won't work
    SocketService( const SocketService& );
    SocketService& operator=( const SocketService& );

};


}


#endif
