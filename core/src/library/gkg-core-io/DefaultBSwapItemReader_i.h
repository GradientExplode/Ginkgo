#ifndef _gkg_core_io_DefaultBSwapItemReader_i_h_
#define _gkg_core_io_DefaultBSwapItemReader_i_h_


#include <gkg-core-io/DefaultBSwapItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <algorithm>
#include <complex>
#include <string>


//
// default methods
//

template < class T >
inline
gkg::DefaultBSwapItemReader< T >::DefaultBSwapItemReader()
                                 : gkg::ItemReader< T >()
{
}


template < class T >
inline
gkg::DefaultBSwapItemReader< T >::~DefaultBSwapItemReader()
{
}


template < class T >
inline
void gkg::DefaultBSwapItemReader< T >::read( std::istream & is,
                                             T* pItem,
                                             size_t count ) const
{

  is.read( ( char* )pItem, sizeof( T ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( T ); k += sizeof( T ) )
  {

    for ( size_t b = 0; b < sizeof( T ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( T ) - b - 1 ] );

    }

  }

}


template < class T >
inline
void gkg::DefaultBSwapItemReader< T >::read( gkg::largefile_ifstream & is,
                                             T* pItem,
                                             size_t count ) const
{

  is.read( ( char* )pItem, sizeof( T ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( T ); k += sizeof( T ) )
  {

    for ( size_t b = 0; b < sizeof( T ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( T ) - b - 1 ] );

    }

  }

}


template < class T >
inline
void gkg::DefaultBSwapItemReader< T >::read( gkg::largefile_fstream & fs,
                                             T* pItem,
                                             size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( T ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( T ); k += sizeof( T ) )
  {

    for ( size_t b = 0; b < sizeof( T ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( T ) - b - 1 ] );

    }

  }

}


//
// specialization(s) for istream
//

namespace gkg
{


template <>
inline
void DefaultBSwapItemReader< int8_t >::read( std::istream& is,
                                             int8_t* pItem,
                                             size_t count ) const
{

  is.read( (char *)pItem, sizeof( int8_t ) * count );

}


template <>
inline
void DefaultBSwapItemReader< uint8_t >::read( std::istream& is,
                                              uint8_t* pItem,
                                              size_t count ) const
{

  is.read( ( char* )pItem, sizeof( uint8_t ) * count );

}


template <>
inline
void DefaultBSwapItemReader< bool >::read( std::istream& is,
                                           bool* pItem,
                                           size_t count ) const
{

  is.read( ( char* )pItem, sizeof( bool ) * count );

}


template <>
inline
void DefaultBSwapItemReader< std::complex< float > >::read(
                                                   std::istream& is,
                                                   std::complex< float >* pItem,
                                                   size_t count ) const
{

  is.read( ( char* )pItem, sizeof( std::complex< float > ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< float > );
         k += sizeof( float ) )
  {

    for ( size_t b = 0; b < sizeof( float ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( float ) - b - 1 ] );

    }

  }

}


template <>
inline
void DefaultBSwapItemReader< std::complex< double > >::read(
                                                  std::istream& is,
                                                  std::complex< double >* pItem,
                                                  size_t count ) const
{

  is.read( ( char* )pItem, sizeof( std::complex< double > ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< double > );
         k += sizeof( double ) )
  {

    for ( size_t b = 0; b < sizeof( double ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( double ) - b - 1 ] );

    }

  }

}


template <>
inline
void DefaultBSwapItemReader< std::string >::read( std::istream& is,
                                                  std::string* pItem,
                                                  size_t count ) const
{


  std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    uint32_t size;
    is.read( ( char* )&size, sizeof( uint32_t ) );
    uint8_t* ptr = ( uint8_t* )&size;

    for ( size_t b = 0; b < sizeof( uint32_t ) / 2; ++b )
    {

      std::swap( ptr[ b ], ptr[ sizeof( uint32_t ) - b - 1 ] );

    }

    char* source = new char[ size + 1 ];
    is.read( ( char* )source, sizeof( char ) * size );
    source[ size ] = '\0';

    *pointer = std::string( source );

  }

}


}

//
// specialization(s) for largefile_ifstream
//

namespace gkg
{


template <>
inline
void DefaultBSwapItemReader< int8_t >::read( largefile_ifstream& is,
                                             int8_t* pItem,
                                             size_t count ) const
{

  is.read( ( char* )pItem, sizeof( int8_t ) * count );

}


template <>
inline
void DefaultBSwapItemReader< uint8_t >::read( largefile_ifstream& is,
                                              uint8_t* pItem,
                                              size_t count ) const
{

  is.read( ( char* )pItem, sizeof( uint8_t ) * count );

}


template <>
inline
void DefaultBSwapItemReader< bool >::read( largefile_ifstream& is,
                                           bool* pItem,
                                           size_t count ) const
{

  is.read( ( char* )pItem, sizeof( bool ) * count );

}


template <>
inline
void DefaultBSwapItemReader< std::complex< float > >::read(
                                                   largefile_ifstream& is,
                                                   std::complex< float >* pItem,
                                                   size_t count ) const
{

  is.read( ( char* )pItem, sizeof( std::complex< float > ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< float > );
         k += sizeof( float ) )
  {

    for ( size_t b = 0; b < sizeof( float ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( float ) - b - 1 ] );

    }

  }

}


template <>
inline
void DefaultBSwapItemReader< std::complex< double > >::read(
                                                  largefile_ifstream& is,
                                                  std::complex< double >* pItem,
                                                  size_t count ) const
{

  is.read( ( char* )pItem, sizeof( std::complex< double > ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< double > );
         k += sizeof( double ) )
  {

    for ( size_t b = 0; b < sizeof( double ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( double ) - b - 1 ] );

    }

  }

}


template <>
inline
void DefaultBSwapItemReader< std::string >::read( largefile_ifstream& is,
                                                  std::string* pItem,
                                                  size_t count ) const
{


  std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    uint32_t size;
    is.read( ( char* )&size, sizeof( uint32_t ) );
    uint8_t* ptr = ( uint8_t* )&size;

    for ( size_t b = 0; b < sizeof( uint32_t ) / 2; ++b )
    {

      std::swap( ptr[ b ], ptr[ sizeof( uint32_t ) - b - 1 ] );

    }

    char* source = new char[ size + 1 ];
    is.read( ( char* )source, sizeof( char ) * size );
    source[ size ] = '\0';

    *pointer = std::string( source );

  }

}


}


//
// specialization(s) for largefile_fstream
//

namespace gkg
{


template <>
inline
void DefaultBSwapItemReader< int8_t >::read( largefile_fstream& fs,
                                             int8_t* pItem,
                                             size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( int8_t ) * count );

}


template <>
inline
void DefaultBSwapItemReader< uint8_t >::read( largefile_fstream& fs,
                                              uint8_t* pItem,
                                              size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( uint8_t ) * count );

}


template <>
inline
void DefaultBSwapItemReader< bool >::read( largefile_fstream& fs,
                                           bool* pItem,
                                           size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( bool ) * count );

}


template <>
inline
void DefaultBSwapItemReader< std::complex< float > >::read(
                                                   largefile_fstream& fs,
                                                   std::complex< float >* pItem,
                                                   size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( std::complex< float > ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< float > );
         k += sizeof( float ) )
  {

    for ( size_t b = 0; b < sizeof( float ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( float ) - b - 1 ] );

    }

  }

}


template <>
inline
void DefaultBSwapItemReader< std::complex< double > >::read(
                                                  largefile_fstream& fs,
                                                  std::complex< double >* pItem,
                                                  size_t count ) const
{

  fs.read( ( char* )pItem, sizeof( std::complex< double > ) * count );
  uint8_t* pointer = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< double > );
         k += sizeof( double ) )
  {

    for ( size_t b = 0; b < sizeof( double ) / 2; ++b )
    {

      std::swap( pointer[ k + b ], pointer[ k + sizeof( double ) - b - 1 ] );

    }

  }

}


template <>
inline
void DefaultBSwapItemReader< std::string >::read( largefile_fstream& fs,
                                                  std::string* pItem,
                                                  size_t count ) const
{


  std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    uint32_t size;
    fs.read( ( char* )&size, sizeof( uint32_t ) );
    uint8_t* ptr = ( uint8_t* )&size;

    for ( size_t b = 0; b < sizeof( uint32_t ) / 2; ++b )
    {

      std::swap( ptr[ b ], ptr[ sizeof( uint32_t ) - b - 1 ] );

    }

    char* source = new char[ size + 1 ];
    fs.read( ( char* )source, sizeof( char ) * size );
    source[ size ] = '\0';

    *pointer = std::string( source );

  }

}


}


#endif
