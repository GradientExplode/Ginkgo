#ifndef _gkg_core_cppext_HashMap_h_
#define _gkg_core_cppext_HashMap_h_

#ifdef __GNUC__

#include <string>

#if __GNUG__ >= 3

#include <ext/hash_map>
#define HASHMAP_NAMESPACE __gnu_cxx

#else

#include <hash_map>
#define HASHMAP_NAMESPACE std

#endif

namespace HASHMAP_NAMESPACE
{


template <> struct hash< std::string >
{

  size_t operator()( std::string s ) const
  {

    hash< const char* > hasher;
    return hasher.operator()( s.c_str() );

  }

};


}


namespace gkg
{


template < class Key >
struct Hash : public HASHMAP_NAMESPACE::hash< Key >
{
};


template < class Key,
           class Tp,
           class HashFcn  = Hash< Key >,
           class EqualKey = std::equal_to< Key >,
           class Alloc =  std::allocator< Tp > >
class HashMap : public HASHMAP_NAMESPACE::hash_map< Key, Tp,
                                                    HashFcn, EqualKey, Alloc >
{

  public:

    HashMap() : HASHMAP_NAMESPACE::hash_map< Key, Tp, HashFcn,
                                             EqualKey, Alloc >() {}
    HashMap( typename 
             HASHMAP_NAMESPACE::hash_map< Key, Tp, HashFcn,
                                          EqualKey, Alloc >::size_type n ) :
               HASHMAP_NAMESPACE::hash_map< Key, Tp, HashFcn,
                                            EqualKey, Alloc >( n ) {}

};


}


#else


// code to be written for non GNU compiler

#endif


namespace gkg
{


template < class T >
inline
unsigned long getKeyToHash( const T& value )
{

  return ( unsigned long )value;

}


template <>
inline
unsigned long getKeyToHash( const std::string& value )
{

  const char* p;
  unsigned long v = 0;
  const char* q = value.c_str() + value.length() - 1;
  for ( p = value.c_str(); p <= q; p++ )
  {

    v = ( v << 1 ) ^ ( *p );

  }
  unsigned long t = v >> 10;
  t ^= ( t >> 10 );
  return v ^ t;

}


}


#endif
