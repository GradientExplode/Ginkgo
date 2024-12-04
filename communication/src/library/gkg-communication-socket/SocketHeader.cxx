#include <gkg-communication-socket/SocketHeader.h>
#include <gkg-communication-socket/OStream.h>
#include <gkg-communication-socket/IStream.h>


// initialize the header for an outgoing socket request

gkg::SocketHeader::SocketHeader( void* writer, int32_t request )
                  : _writer( writer ),
                    _request( request )
{
}


// initialize the header for an incoming socket request.
gkg::SocketHeader::SocketHeader()
                  : _reader( 0 ),
                    _request( 0 )
{
}


// get information about the socket request.
uint64_t gkg::SocketHeader::getReader()
{

  return _reader;

}


int32_t gkg::SocketHeader::getRequest()
{

  return _request;

}


// if incomplete_request was set, explicitly call underflow in an
// attempt to complete an incoming socket request by reading additional
// data and reset the flag; set the stream in eof state if no
// additional data was available or in fail state if it was already in
// eof state; if the stream is still good, call read_request to check
// if the incoming socket request is complete yet; if it is, extract its
// header, else set setIncompleteRequest to note so; decrement the
// length (which counted the entire request) by the space that the
// header occupied to count only the data following the header
gkg::IStream& gkg::operator>>( gkg::IStream& client, gkg::SocketHeader& header )
{

  if ( client.good() )
  {

    client >> header._reader;
    client >> header._request;

  }
  return client;

}


// store the beginning of a new socket request; the SocketBuffer automatically
// initializes the length field of the previous socket request and skips
// the put pointer past the length field of the new socket request so
// there's no need to examine header._dataCount or insert its value.

gkg::OStream& gkg::operator<<( gkg::OStream& server,
                               const gkg::SocketHeader& header )
{

  if ( server.good() )
  {

    server << header._reader << header._request;

  }
  return server;

}
