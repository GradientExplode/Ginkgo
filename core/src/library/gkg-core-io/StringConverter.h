#ifndef _gkg_core_io_StringConverter_h_
#define _gkg_core_io_StringConverter_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


struct StringConverter
{

    static std::string toPureAscii( const std::string& msg );
    static std::string toazAZ09( const std::string& msg );

    static bool toScalar( const std::string& msg,
                          std::string& value );
    static bool toScalar( const std::string& msg,
                          int8_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          uint8_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          int16_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          uint16_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          int32_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          uint32_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          int64_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          uint64_t& value, int32_t base = 10 );
    static bool toScalar( const std::string& msg,
                          float& value );
    static bool toScalar( const std::string& msg,
                          double& value );

#if ( defined(__APPLE__) && defined(__MACH__) )

    static bool toScalar( const std::string& msg,
                          size_t& value, int32_t base = 10 );

#endif

    static bool toScalar( const std::string& msg,
                          bool& value );

    static std::string toString( const std::string& value );
    static std::string toString( const int8_t& value, int32_t base = 10 );
    static std::string toString( const uint8_t& value, int32_t base = 10 );
    static std::string toString( const int16_t& value, int32_t base = 10 );
    static std::string toString( const uint16_t& value, int32_t base = 10 );
    static std::string toString( const int32_t& value, int32_t base = 10 );
    static std::string toString( const uint32_t& value, int32_t base = 10 );
    static std::string toString( const int64_t& value, int32_t base = 10 );
    static std::string toString( const uint64_t& value, int32_t base = 10 );
    static std::string toString( const float& value,
                                 char format,
                                 int32_t width,
                                 int32_t precision );
    static std::string toString( const double& value,
                                 char format,
                                 int32_t width,
                                 int32_t precision );
    static std::string toString( const float& value,
                                 int32_t width,
                                 int32_t precision );
    static std::string toString( const double& value,
                                 int32_t width,
                                 int32_t precision );


#if ( defined(__APPLE__) && defined(__MACH__) )

    static std::string toString( const size_t& value, int32_t base = 10 );

#endif

    static std::string toString( const bool& value );

    static std::string toLower( const std::string& msg );
    static std::string toUpper( const std::string& msg );

    static bool isSpace( char c );
    static bool isOkInBase( char c, int32_t base );

};


}


#endif
