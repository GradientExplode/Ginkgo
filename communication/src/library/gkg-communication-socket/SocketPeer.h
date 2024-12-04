#ifndef _gkg_communication_socket_SocketPeer_h_
#define _gkg_communication_socket_SocketPeer_h_


#include <string>
#include <gkg-communication-dispatcher/IOHandler.h>
#include <gkg-core-cppext/NetInetIn.h>


namespace gkg
{


class SocketBuffer;


// Support bi-directional socket between two peers.  Derived classes
// create both a reader and a writer so each peer can send socket
// requests to its opposite over the same connection.

class SocketPeer : public IOHandler
{

  public:

    virtual ~SocketPeer();

    void run( timeval* timeout = 0 );
    void quitRunning();

  protected:

    SocketPeer( const std::string& lPath, in_port_t lPort = 0 );

    void initialize( const std::string& rPath );
    int32_t createSocket();
    void startListening();
    void stopListening();
    virtual int32_t isInputReady( int32_t fd );
    virtual bool createReaderAndWriter( const std::string& rHost,
                                        int32_t rPort ) = 0;
    virtual void createReaderAndWriter( int32_t fd ) = 0;

    std::string _path;          // my registration's path
    in_port_t _port;            // my port's address
    int32_t _fd;                // file descriptor for socket
    SocketBuffer* _buffer;      // my network socket
    volatile bool _running;     // am I running my dispatch loop?

    bool _remote;               // am I connected to a remote Socket service?
    std::string _host;          // dynamically allocated storage to be freed

  private:

    // deny access since unimplemented and member-wise won't work
    SocketPeer( const SocketPeer& );
    SocketPeer& operator=( const SocketPeer& );

};


}


#endif
