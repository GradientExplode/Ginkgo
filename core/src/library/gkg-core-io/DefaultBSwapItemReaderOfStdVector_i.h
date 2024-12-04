#ifndef _gkg_core_io_DefaultBSwapItemReaderOfStdVector_i_h_
#define _gkg_core_io_DefaultBSwapItemReaderOfStdVector_i_h_

#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>
#include <string>


//
// class DefaultBSwapItemReader< std::vector< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBSwapItemReader< std::vector< T > >::DefaultBSwapItemReader()
                                         : ItemReader< std::vector< T > >()
{
}


template < class T >
inline
DefaultBSwapItemReader< std::vector< T > >::~DefaultBSwapItemReader()
{
}


template < class T >
inline
void DefaultBSwapItemReader< std::vector< T > >::read( std::istream & is,
                                                       std::vector< T >* pItem,
                                                      size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorBSwapItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemReader< std::vector< T > >::read( largefile_ifstream & is,
                                                       std::vector< T >* pItem,
                                                       size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorBSwapItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemReader< std::vector< T > >::read( largefile_fstream & fs,
                                                       std::vector< T >* pItem,
                                                       size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorBSwapItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( fs, pItem[ i ] );

  }

}


}


#endif
