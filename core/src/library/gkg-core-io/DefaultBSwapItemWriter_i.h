#ifndef _gkg_core_io_DefaultBSwapItemWriter_i_h_
#define _gkg_core_io_DefaultBSwapItemWriter_i_h_


#include <gkg-core-io/DefaultBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


//
// default methods
//

template < class T >
inline
gkg::DefaultBSwapItemWriter< T >::DefaultBSwapItemWriter()
                                 : gkg::ItemWriter< T >()
{
}


template < class T >
inline
gkg::DefaultBSwapItemWriter< T >::~DefaultBSwapItemWriter()
{
}


template < class T >
inline
void gkg::DefaultBSwapItemWriter< T >::write( std::ostream & os,
                                              const T* pItem,
                                              size_t count ) const
{

  uint8_t* destination = new uint8_t[ count * sizeof( T ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( T ); k += sizeof( T ) )
  {

    for ( size_t b = 0; b < sizeof( T ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( T ) - b - 1 ];

    }

  }
  os.write( ( const char* )destination, sizeof( T ) * count );
  delete [] destination;

}


template < class T >
inline
void gkg::DefaultBSwapItemWriter< T >::write( gkg::largefile_ofstream & os,
                                              const T* pItem,
                                              size_t count ) const
{

  uint8_t* destination = new uint8_t[ count * sizeof( T ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( T ); k += sizeof( T ) )
  {

    for ( size_t b = 0; b < sizeof( T ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( T ) - b - 1 ];

    }

  }
  os.write( ( const char* )destination, sizeof( T ) * count );
  delete [] destination;

}


template < class T >
inline
void gkg::DefaultBSwapItemWriter< T >::write( gkg::largefile_fstream & fs,
                                              const T* pItem,
                                              size_t count ) const
{

  uint8_t* destination = new uint8_t[ count * sizeof( T ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( T ); k += sizeof( T ) )
  {

    for ( size_t b = 0; b < sizeof( T ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( T ) - b - 1 ];

    }

  }
  fs.write( ( const char* )destination, sizeof( T ) * count );
  delete [] destination;

}


//
// specialization(s) for ostream
//

namespace gkg
{


template <>
inline
void DefaultBSwapItemWriter< int8_t >::write( std::ostream& os,
                                              const int8_t* pItem,
                                              size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( int8_t ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< uint8_t >::write( std::ostream& os,
                                               const uint8_t* pItem,
                                               size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( uint8_t ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< bool >::write( std::ostream& os,
                                            const bool* pItem,
                                            size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( bool ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< std::complex< float > >::write(
                                             std::ostream& os,
                                             const std::complex< float >* pItem,
                                             size_t count ) const
{

  uint8_t* destination = new uint8_t[ count * sizeof( std::complex< float > ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< float > );
        k += sizeof( float ) )
  {

    for ( size_t b = 0; b < sizeof( float ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( float ) - b - 1 ];

    }

  }
  os.write( ( const char* )destination,
            sizeof( std::complex< float > ) * count );
  delete [] destination;

}


template <>
inline
void DefaultBSwapItemWriter< std::complex< double > >::write(
                                            std::ostream& os,
                                            const std::complex< double >* pItem,
                                            size_t count ) const
{

  uint8_t* 
    destination = new uint8_t[ count * sizeof( std::complex< double > ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< double > );
        k += sizeof( double ) )
  {

    for ( size_t b = 0; b < sizeof( double ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( double ) - b - 1 ];

    }

  }
  os.write( ( const char* )destination,
            sizeof( std::complex< double > ) * count );
  delete [] destination;

}


template <>
inline
void DefaultBSwapItemWriter< std::string >::write( std::ostream & os,
                                                   const std::string* pItem,
                                                   size_t count ) const
{

  uint8_t* destination = new uint8_t[ sizeof( uint32_t ) ];

  const std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    uint32_t size = ( uint32_t )pointer->size();

    const uint8_t* source = ( uint8_t* )&size;
    for ( size_t b = 0; b < sizeof( uint32_t ) / 2; ++b )
    {

      destination[ b ] = source[ sizeof( uint32_t ) - b - 1 ];

    }

    os.write( ( const char* )destination, sizeof( uint32_t ) );
    os.write(  pointer->c_str(), sizeof( char ) * pointer->size() );

  }

  delete [] destination;

}


}

//
// specialization(s) for largefile_ofstream
//

namespace gkg
{


template <>
inline
void DefaultBSwapItemWriter< int8_t >::write( largefile_ofstream& os,
                                              const int8_t* pItem,
                                              size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( int8_t ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< uint8_t >::write( largefile_ofstream& os,
                                               const uint8_t* pItem,
                                               size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( uint8_t ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< bool >::write( largefile_ofstream& os,
                                            const bool* pItem,
                                            size_t count ) const
{

  os.write( ( const char* )pItem, sizeof( bool ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< std::complex< float > >::write(
                                             largefile_ofstream& os,
                                             const std::complex< float >* pItem,
                                             size_t count ) const
{

  uint8_t* destination = new uint8_t[ count * sizeof( std::complex< float > ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< float > );
        k += sizeof( float ) )
  {

    for ( size_t b = 0; b < sizeof( float ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( float ) - b - 1 ];

    }

  }
  os.write( ( const char* )destination,
            sizeof( std::complex< float > ) * count );
  delete [] destination;

}


template <>
inline
void DefaultBSwapItemWriter< std::complex< double > >::write(
                                            largefile_ofstream& os,
                                            const std::complex< double >* pItem,
                                            size_t count ) const
{

  uint8_t*
    destination = new uint8_t[ count * sizeof( std::complex< double > ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< double > );
        k += sizeof( double ) )
  {

    for ( size_t b = 0; b < sizeof( double ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( double ) - b - 1 ];

    }

  }
  os.write( ( const char* )destination,
            sizeof( std::complex< double > ) * count );
  delete [] destination;

}


template <>
inline
void DefaultBSwapItemWriter< std::string >::write( largefile_ofstream & os,
                                                   const std::string* pItem,
                                                   size_t count ) const
{

  uint8_t* destination = new uint8_t[ sizeof( uint32_t ) ];

  const std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    uint32_t size = ( uint32_t )pointer->size();

    const uint8_t* source = ( uint8_t* )&size;
    for ( size_t b = 0; b < sizeof( uint32_t ) / 2; ++b )
    {

      destination[ b ] = source[ sizeof( uint32_t ) - b - 1 ];

    }

    os.write( ( const char* )destination, sizeof( uint32_t ) );
    os.write(  pointer->c_str(), sizeof( char ) * pointer->size() );

  }

  delete [] destination;

}


}


//
// specialization(s) for largefile_fstream
//

namespace gkg
{


template <>
inline
void DefaultBSwapItemWriter< int8_t >::write( largefile_fstream& fs,
                                              const int8_t* pItem,
                                              size_t count ) const
{

  fs.write( ( const char* )pItem, sizeof( int8_t ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< uint8_t >::write( largefile_fstream& fs,
                                               const uint8_t* pItem,
                                               size_t count ) const
{

  fs.write( ( const char* )pItem, sizeof( uint8_t ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< bool >::write( largefile_fstream& fs,
                                            const bool* pItem,
                                            size_t count ) const
{

  fs.write( ( const char* )pItem, sizeof( bool ) * count );

}


template <>
inline
void DefaultBSwapItemWriter< std::complex< float > >::write(
                                             largefile_fstream& fs,
                                             const std::complex< float >* pItem,
                                             size_t count ) const
{

  uint8_t* destination = new uint8_t[ count * sizeof( std::complex< float > ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< float > );
        k += sizeof( float ) )
  {

    for ( size_t b = 0; b < sizeof( float ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( float ) - b - 1 ];

    }

  }
  fs.write( ( const char* )destination,
            sizeof( std::complex< float > ) * count );
  delete [] destination;

}


template <>
inline
void DefaultBSwapItemWriter< std::complex< double > >::write(
                                            largefile_fstream& fs,
                                            const std::complex< double >* pItem,
                                            size_t count ) const
{

  uint8_t*
    destination = new uint8_t[ count * sizeof( std::complex< double > ) ];
  const uint8_t* source = ( uint8_t* )pItem;
  for ( size_t k = 0; k < count * sizeof( std::complex< double > );
        k += sizeof( double ) )
  {

    for ( size_t b = 0; b < sizeof( double ) / 2; ++b )
    {

      destination[ k + b ] = source[ k + sizeof( double ) - b - 1 ];

    }

  }
  fs.write( ( const char* )destination,
            sizeof( std::complex< double > ) * count );
  delete [] destination;

}


template <>
inline
void DefaultBSwapItemWriter< std::string >::write( largefile_fstream & fs,
                                                   const std::string* pItem,
                                                   size_t count ) const
{

  uint8_t* destination = new uint8_t[ sizeof( uint32_t ) ];

  const std::string* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    uint32_t size = ( uint32_t )pointer->size();

    const uint8_t* source = ( uint8_t* )&size;
    for ( size_t b = 0; b < sizeof( uint32_t ) / 2; ++b )
    {

      destination[ b ] = source[ sizeof( uint32_t ) - b - 1 ];

    }

    fs.write( ( const char* )destination, sizeof( uint32_t ) );
    fs.write(  pointer->c_str(), sizeof( char ) * pointer->size() );

  }

  delete [] destination;

}


}


#endif
