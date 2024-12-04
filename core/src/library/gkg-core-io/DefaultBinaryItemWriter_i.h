#ifndef _gkg_core_io_DefaultBinaryItemWriter_i_h_
#define _gkg_core_io_DefaultBinaryItemWriter_i_h_


#include <gkg-core-io/DefaultBinaryItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


//
// default methods
//

template < class T >
inline
gkg::DefaultBinaryItemWriter< T >::DefaultBinaryItemWriter()
                                  : gkg::ItemWriter< T >()
{
}


template < class T >
inline
gkg::DefaultBinaryItemWriter< T >::~DefaultBinaryItemWriter()
{
}


template < class T >
inline
void gkg::DefaultBinaryItemWriter< T >::write( std::ostream & os,
                                               const T* pItem,
                                               size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( T ) * count );

}


template < class T >
inline
void gkg::DefaultBinaryItemWriter< T >::write( gkg::largefile_ofstream & os,
                                               const T* pItem,
                                               size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( T ) * count );

}


template < class T >
inline
void gkg::DefaultBinaryItemWriter< T >::write( gkg::largefile_fstream & fs,
                                               const T* pItem,
                                               size_t count ) const
{

  fs.write( ( const char* )pItem, sizeof( T ) * count );

}


// specialization(s)

namespace gkg
{


template <>
inline
void DefaultBinaryItemWriter< std::string >::write( std::ostream & os,
                                                    const std::string* pItem,
                                                    size_t count ) const
{

  const std::string* pointer = pItem;
  uint32_t size = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    size = ( uint32_t )pointer->size();
    os.write( ( const char* )&size, sizeof( uint32_t ) );
    os.write(  pointer->c_str(), sizeof( char ) * pointer->size() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< std::string >::write( largefile_ofstream & os,
                                                    const std::string* pItem,
                                                    size_t count ) const
{

  const std::string* pointer = pItem;
  uint32_t size = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    size = ( uint32_t )pointer->size();
    os.write( ( const char* )&size, sizeof( uint32_t ) );
    os.write(  pointer->c_str(), sizeof( char ) * pointer->size() );

  }

}


template <>
inline
void DefaultBinaryItemWriter< std::string >::write( largefile_fstream & fs,
                                                    const std::string* pItem,
                                                    size_t count ) const
{

  const std::string* pointer = pItem;
  uint32_t size = 0;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    size = ( uint32_t )pointer->size();
    fs.write( ( const char* )&size, sizeof( uint32_t ) );
    fs.write(  pointer->c_str(), sizeof( char ) * pointer->size() );

  }

}


}


#endif
