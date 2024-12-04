#ifndef _gkg_core_io_TypeOf_h_
#define _gkg_core_io_TypeOf_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>
#include <complex>
#include <vector>
#include <list>
#include <deque>
#include <set>


namespace gkg
{


template < class T >
class TypeOf
{

  public:

    static std::string getObjectName();
    static std::string getItemName();
    static std::string getName();
    static bool isSameType( const std::string& other );

};


}


template < class T >
inline
std::string gkg::TypeOf< T >::getObjectName()
{

  return "";

}


template < class T >
inline
std::string gkg::TypeOf< T >::getItemName()
{

  return "undefined type";

}


template < class T >
inline
std::string gkg::TypeOf< T >::getName()
{

  return getItemName();

}


template < class T >
inline
bool gkg::TypeOf< T >::isSameType( const std::string& other )
{

  return ( getName() == other );

}


#define RegisterBaseType( TYPE, NAME )                      \
namespace gkg                                               \
{                                                           \
                                                            \
template <>                                                 \
inline                                                      \
std::string TypeOf< TYPE >::getItemName()                   \
{                                                           \
                                                            \
  return #NAME;                                             \
                                                            \
}                                                           \
                                                            \
                                                            \
template <>                                                 \
inline                                                      \
bool TypeOf< TYPE >::isSameType( const std::string& other ) \
{                                                           \
                                                            \
  return ( other == #NAME );                                \
                                                            \
}                                                           \
                                                            \
}


#define RegisterBaseTypeDual( TYPE, NAME1, NAME2 )          \
namespace gkg                                               \
{                                                           \
                                                            \
template <>                                                 \
inline                                                      \
std::string TypeOf< TYPE >::getItemName()                   \
{                                                           \
                                                            \
  return #NAME1;                                            \
                                                            \
}                                                           \
                                                            \
                                                            \
template <>                                                 \
inline                                                      \
bool TypeOf< TYPE >::isSameType( const std::string& other ) \
{                                                           \
                                                            \
  return ( ( other == #NAME1 ) || ( other == #NAME2 ) );    \
                                                            \
}                                                           \
                                                            \
}


#define RegisterTemplateType( TYPE, NAME )                       \
namespace gkg                                                    \
{                                                                \
                                                                 \
template < class T >                                             \
class TypeOf< TYPE< T > >                                        \
{                                                                \
                                                                 \
  public:                                                        \
                                                                 \
    static std::string getObjectName();                          \
    static std::string getItemName();                            \
    static std::string getName();                                \
    static bool isSameType( const std::string& other );          \
                                                                 \
};                                                               \
                                                                 \
                                                                 \
                                                                 \
template < class T >                                             \
inline                                                           \
std::string TypeOf< TYPE< T > >::getObjectName()                 \
{                                                                \
                                                                 \
  return #NAME;                                                  \
                                                                 \
}                                                                \
                                                                 \
                                                                 \
template < class T >                                             \
inline                                                           \
std::string TypeOf< TYPE< T > >::getItemName()                   \
{                                                                \
                                                                 \
  return TypeOf< T >::getName();                                 \
                                                                 \
}                                                                \
                                                                 \
                                                                 \
template < class T >                                             \
inline                                                           \
std::string TypeOf< TYPE< T > >::getName()                       \
{                                                                \
                                                                 \
  return getObjectName() + "_of_" + getItemName();               \
                                                                 \
}                                                                \
                                                                 \
                                                                 \
template < class T >                                             \
inline                                                           \
bool TypeOf< TYPE< T > >::isSameType( const std::string& other ) \
{                                                                \
                                                                 \
  return ( getName() == other );                                 \
                                                                 \
}                                                                \
                                                                 \
}


RegisterBaseType( void, void );
RegisterBaseTypeDual( int8_t, int8_t, char );
RegisterBaseTypeDual( uint8_t, uint8_t, unsigned_char );
RegisterBaseTypeDual( int16_t, int16_t, short );
RegisterBaseTypeDual( uint16_t, uint16_t, unsigned_short );
RegisterBaseTypeDual( int32_t, int32_t, int );
RegisterBaseTypeDual( uint32_t, uint32_t, unsigned_int );
RegisterBaseType( int64_t, int64_t );
RegisterBaseType( uint64_t, uint64_t );
RegisterBaseType( float, float );
RegisterBaseType( double, double );
RegisterBaseType( bool, bool );
RegisterBaseType( std::string, std_string );

RegisterTemplateType( std::complex, std_complex );
RegisterTemplateType( std::vector, std_vector );
RegisterTemplateType( std::list, std_list );
RegisterTemplateType( std::deque, std_deque );
RegisterTemplateType( std::set, std_set );


#endif
