#ifndef _gkg_communication_socket_SocketHeader_h_
#define _gkg_communication_socket_SocketHeader_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/NetInetIn.h>


namespace gkg
{


class OStream;
class IStream;


// insert or extract this header to send or receive a socket request.
class SocketHeader
{

  public:

    SocketHeader( void* writer, int32_t request );
    SocketHeader();

    uint64_t getReader();
    int32_t getRequest();

  protected:

    friend IStream&
      operator>>( IStream& client, SocketHeader& header );
    friend OStream&
      operator<<( OStream& client, const SocketHeader& header );

    union
    {

      void* _writer;           // stores writer sending this socket request
      uint64_t _reader;        // maps to reader for this socket request

    };
    int32_t _request;          // maps to member function to be called

};


IStream& operator>>( IStream& client, SocketHeader& header );
OStream& operator<<( OStream& client, const SocketHeader& header );


}


#endif
