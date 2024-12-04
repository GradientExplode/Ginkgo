#ifndef _gkg_communication_socket_IStream_h_
#define _gkg_communication_socket_IStream_h_


#include <gkg-communication-socket/IOStreamBase.h>
#include <iostream>


namespace gkg
{


// modify std::istream to extract unformatted data where possible for
// faster I/O throughput and to discard delimiters automatically.
class IStream : public virtual IOStreamBase, public std::istream
{

  public:

    IStream( std::streambuf* b );
    ~IStream();

    IStream& operator>>( int8_t& c );
    IStream& operator>>( uint8_t& uc );
    IStream& operator>>( int16_t& s );
    IStream& operator>>( uint16_t& us );
    IStream& operator>>( int32_t& i );
    IStream& operator>>( uint32_t& ui );
    IStream& operator>>( int64_t& l );
    IStream& operator>>( uint64_t& ul );
    IStream& operator>>( float& f );
    IStream& operator>>( double& d );
    IStream& operator>>( char* p );
    IStream& operator>>( signed char* p );
    IStream& operator>>( unsigned char* up );

    IStream& operator>>( std::istream& ( *f )( std::istream& ) );
    IStream& operator>>( std::ios& ( *f )( std::ios& ) );

  protected:

    IStream();

};


}


#endif
