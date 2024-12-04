#ifndef _gkg_core_io_DefaultAsciiItemReaderOfStdList_i_h_
#define _gkg_core_io_DefaultAsciiItemReaderOfStdList_i_h_


#include <gkg-core-io/DefaultAsciiItemReaderOfStdList.h>
#include <gkg-core-io/StdListAsciiItemReader_i.h>
#include <string>


//
// class DefaultAsciiItemReader< std::list< T > >
//

namespace gkg
{


template < class T >
inline
DefaultAsciiItemReader< std::list< T > >::DefaultAsciiItemReader()
                                         : ItemReader< std::list< T > >()
{
}


template < class T >
inline
DefaultAsciiItemReader< std::list< T > >::~DefaultAsciiItemReader()
{
}


template < class T >
inline
void DefaultAsciiItemReader< std::list< T > >::read( std::istream & is,
                                             std::list< T >* pItem,
                                             size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListAsciiItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemReader< std::list< T > >::read( largefile_ifstream & is,
                                                     std::list< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListAsciiItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemReader< std::list< T > >::read( largefile_fstream & fs,
                                                     std::list< T >* pItem,
                                                     size_t count ) const
{

  ItemReader< std::list< T > >& itemR_Stdlist =
    StdListAsciiItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdlist.read( fs, pItem[ i ] );

  }

}


}


#endif
