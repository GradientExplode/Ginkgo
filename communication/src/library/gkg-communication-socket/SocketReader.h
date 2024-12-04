#ifndef _gkg_communication_socket_SocketReader_h_
#define _gkg_communication_socket_SocketReader_h_


#include <gkg-communication-dispatcher/IOHandler.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>
#include <string>
#include <map>


namespace gkg
{


class SocketHeader;
class SocketStream;


// read socket requests from a client; derived classes initialize the
// function array with addresses of static member functions to
// unmarshall socket requests and implement the virtual function called
// when the client closes the connection

class SocketReader : public IOHandler
{

  public:

    typedef void ( *PF )( SocketReader*, SocketHeader&, SocketStream& );

    virtual ~SocketReader();

    void addFunction( int32_t id, PF function );
    PF getFunction( int32_t id );

  protected:

    SocketReader( SocketStream* client, bool nonBlocking = true );
    SocketReader( int32_t fd, bool binary, bool nonBlocking = true );

    SocketStream& getClient();
    virtual int32_t isInputReady( int32_t fd );
    virtual SocketReader* map( uint64_t );
    bool execute( SocketReader*, SocketHeader& );
    virtual void connectionClosed( int32_t fd ) = 0;

    int32_t _fd;            // file number of connection with client
    int32_t _nfcns;         // size of function array
    std::map< int32_t, PF > _functions; // function array indexed by 
                                        // request number
    SocketStream* _client;  // source of socket requests coming from client
    bool _delete;           // should the destructor delete _client?

  private:

    // deny access since unimplemented and member-wise won't work
    SocketReader( const SocketReader& );
    SocketReader& operator=( const SocketReader& );

};


}


#endif
