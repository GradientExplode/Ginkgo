#include <gkg-communication-socket/IOStream.h>


// provide a constructor for derived classes to use and a constructor
// for public use.

gkg::IOStream::IOStream()
              : gkg::IStream(),
                gkg::OStream()
{
}


gkg::IOStream::IOStream( std::streambuf* b )
              : gkg::IStream( b ),
                gkg::OStream( b )
{
}


gkg::IOStream::~IOStream()
{
}


