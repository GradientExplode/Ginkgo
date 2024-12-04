#ifndef _gkg_core_io_DefaultBinaryItemReader_i_h_
#define _gkg_core_io_DefaultBinaryItemReader_i_h_


#include <gkg-core-io/DefaultBinaryItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <string>


//
// default methods
//

template < class T >
inline
gkg::DefaultBinaryItemReader< T >::DefaultBinaryItemReader()
                                  : gkg::ItemReader< T >()
{
}


template < class T >
inline
gkg::DefaultBinaryItemReader< T >::~DefaultBinaryItemReader()
{
}


template < class T >
inline
void gkg::DefaultBinaryItemReader< T >::read( std::istream & is,
                                              T* pItem,
                                              size_t count ) const
{

  is.read( ( char* )pItem, sizeof( T ) * count );

}


template < class T >
inline
void gkg::DefaultBinaryItemReader< T >::read( largefile_ifstream & is,
                                              T* pItem,
                                              size_t count ) const
{

  is.read( ( char* )pItem, sizeof( T ) * count );

}


template < class T >
inline
void gkg::DefaultBinaryItemReader< T >::read( largefile_fstream & fs,
                                              T* pItem,
                                              size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( T ) * count );

}


// specialization(s)

namespace gkg
{


template <>
inline
void DefaultBinaryItemReader< std::string >::read( std::istream & is,
                                                   std::string* pItem,
                                                   size_t count ) const
{

  std::string* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    is.read( ( char* )&size, sizeof( uint32_t ) );
    char* destination = new char[ size + 1 ];
    is.read(  destination, size );
    destination[ size ] = '\0';

    *pointer = std::string( destination );

    delete [] destination;

  }

}


template <>
inline
void DefaultBinaryItemReader< std::string >::read( largefile_ifstream & is,
                                                   std::string* pItem,
                                                   size_t count ) const
{

  std::string* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    is.read( ( char* )&size, sizeof( uint32_t ) );
    char* destination = new char[ size + 1 ];
    is.read(  destination, size );
    destination[ size ] = '\0';

    *pointer = std::string( destination );

    delete [] destination;

  }

}


template <>
inline
void DefaultBinaryItemReader< std::string >::read( largefile_fstream & fs,
                                                   std::string* pItem,
                                                   size_t count ) const
{

  std::string* pointer = pItem;
  uint32_t size = 0U;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    fs.read( ( char* )&size, sizeof( uint32_t ) );
    char* destination = new char[ size + 1 ];
    fs.read(  destination, size );
    destination[ size ] = '\0';

    *pointer = std::string( destination );

    delete [] destination;

  }

}


}


#endif
