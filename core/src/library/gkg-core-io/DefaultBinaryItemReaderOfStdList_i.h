#ifndef _gkg_core_io_DefaultBinaryItemReaderOfStdList_i_h_
#define _gkg_core_io_DefaultBinaryItemReaderOfStdList_i_h_


#include <gkg-core-io/DefaultBinaryItemReaderOfStdList.h>
#include <gkg-core-io/StdListBinaryItemReader_i.h>
#include <string>


//
// class DefaultBinaryItemReader< std::list< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBinaryItemReader< std::list< T > >::DefaultBinaryItemReader()
                                         : ItemReader< std::list< T > >()
{
}


template < class T >
inline
DefaultBinaryItemReader< std::list< T > >::~DefaultBinaryItemReader()
{
}


template < class T >
inline
void DefaultBinaryItemReader< std::list< T > >::read( std::istream & is,
                                             std::list< T >* pItem,
                                             size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListBinaryItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemReader< std::list< T > >::read( largefile_ifstream & is,
                                                     std::list< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListBinaryItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemReader< std::list< T > >::read( largefile_fstream & fs,
                                                     std::list< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListBinaryItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( fs, pItem[ i ] );

  }

}


}


#endif
