#ifndef _gkg_communication_socket_OStream_h_
#define _gkg_communication_socket_OStream_h_


#include <gkg-communication-socket/IOStreamBase.h>
#include <iostream>
#include <string>


namespace gkg
{


// modify std::ostream to insert unformatted data where possible for faster
// I/O throughput and to delimit formatted data automatically.
class OStream : public virtual IOStreamBase, public std::ostream
{

  public:

    OStream( std::streambuf* os );
    ~OStream();

    OStream& operator<<( int8_t c );
    OStream& operator<<( uint8_t uc );
    OStream& operator<<( int16_t s );
    OStream& operator<<( uint16_t us );
    OStream& operator<<( int32_t i);
    OStream& operator<<( uint32_t ui );
    OStream& operator<<( int64_t l );
    OStream& operator<<( uint64_t ul );
    OStream& operator<<( float f );
    OStream& operator<<( double d );
    OStream& operator<<( const char* p );
    OStream& operator<<( const unsigned char* up );
    OStream& operator<<( const std::string& s );

    OStream& operator<<( std::ostream& ( *f )( std::ostream& ) );
    OStream& operator<<( std::ios& ( *f )( std::ios& ) );

  protected:

    OStream();
    void fixWidth();

};


}


#endif
