#ifndef _gkg_communication_socket_SocketWriter_h_
#define _gkg_communication_socket_SocketWriter_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <string>


namespace gkg
{


class SocketStream;

// write socket requests to a server; derived classes should add member
// functions corresponding to the socket service's protocol

class SocketWriter 
{

  public:

    ~SocketWriter();

    SocketStream& getServer();
    bool isConnected();

  protected:

    SocketWriter( const std::string& path, 
                  bool fatal, 
                  bool binary,
                  bool noDelay = false );
    SocketWriter( const std::string& host, 
                  in_port_t port, 
                  bool fatal, 
                  bool binary,
                  bool noDelay = false );
    SocketWriter( int32_t fd, bool fatal, bool binary, bool noDelay = false );
    SocketWriter( SocketStream* server, bool noDelay = false );

    int32_t createSocket();

    SocketStream* _server;     // sink of socket requests going to server
    bool _delete;              // should the destructor delete _server?
    std::string _host;         // dynamically allocated storage to be deleted
    in_port_t _port;           // my port's address
    int32_t _fd;               // file descriptor for socket
    bool _noDelay;             // disable nagle's algorithm (disable buffering)

  private:

    // deny access since unimplemented and member-wise won't work
    SocketWriter( const SocketWriter& );
    SocketWriter& operator=( const SocketWriter& );

};


}


#endif
