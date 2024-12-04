#ifndef _gkg_core_io_DefaultAsciiItemReaderOfStdVector_i_h_
#define _gkg_core_io_DefaultAsciiItemReaderOfStdVector_i_h_


#include <gkg-core-io/DefaultAsciiItemReaderOfStdVector.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>
#include <string>


//
// class DefaultAsciiItemReader< std::vector< T > >
//

namespace gkg
{


template < class T >
inline
DefaultAsciiItemReader< std::vector< T > >::DefaultAsciiItemReader()
                                         : ItemReader< std::vector< T > >()
{
}


template < class T >
inline
DefaultAsciiItemReader< std::vector< T > >::~DefaultAsciiItemReader()
{
}


template < class T >
inline
void DefaultAsciiItemReader< std::vector< T > >::read( std::istream & is,
                                             std::vector< T >* pItem,
                                             size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorAsciiItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemReader< std::vector< T > >::read( largefile_ifstream & is,
                                                     std::vector< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorAsciiItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemReader< std::vector< T > >::read( largefile_fstream & fs,
                                                     std::vector< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorAsciiItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( fs, pItem[ i ] );

  }

}


}


#endif
