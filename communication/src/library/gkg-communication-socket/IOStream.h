#ifndef _gkg_communication_socket_IOStream_h_
#define _gkg_communication_socket_IOStream_h_


#include <gkg-communication-socket/IStream.h>
#include <gkg-communication-socket/OStream.h>


namespace gkg
{


// replace std::iostream with a stream that inserts and extracts unformatted
// data where possible for faster I/O throughput and delimits formatted data
// automatically.
class IOStream : public IStream, public OStream
{

  public:

    IOStream( std::streambuf* );
    ~IOStream();

  protected:

    IOStream();

};


}


#endif
