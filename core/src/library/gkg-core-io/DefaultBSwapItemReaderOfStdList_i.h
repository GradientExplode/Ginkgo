#ifndef _gkg_core_io_DefaultBSwapItemReaderOfStdList_i_h_
#define _gkg_core_io_DefaultBSwapItemReaderOfStdList_i_h_

#include <gkg-core-io/DefaultBSwapItemReaderOfStdList.h>
#include <gkg-core-io/StdListBSwapItemReader_i.h>
#include <string>


//
// class DefaultBSwapItemReader< std::list< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBSwapItemReader< std::list< T > >::DefaultBSwapItemReader()
                                         : ItemReader< std::list< T > >()
{
}


template < class T >
inline
DefaultBSwapItemReader< std::list< T > >::~DefaultBSwapItemReader()
{
}


template < class T >
inline
void DefaultBSwapItemReader< std::list< T > >::read( std::istream & is,
                                             std::list< T >* pItem,
                                             size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListBSwapItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemReader< std::list< T > >::read( largefile_ifstream & is,
                                                     std::list< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListBSwapItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemReader< std::list< T > >::read( largefile_fstream & fs,
                                                     std::list< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListBSwapItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( fs, pItem[ i ] );

  }

}


}


#endif
