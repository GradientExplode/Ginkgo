#ifndef _gkg_core_io_DefaultBSwapItemWriterOfStdList_i_h_
#define _gkg_core_io_DefaultBSwapItemWriterOfStdList_i_h_


#include <gkg-core-io/DefaultBSwapItemWriterOfStdList.h>
#include <gkg-core-io/StdListBSwapItemWriter_i.h>
#include <string>


//
// class DefaultBSwapItemWriter< std::list< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBSwapItemWriter< std::list< T > >::DefaultBSwapItemWriter()
                                         : ItemWriter< std::list< T > >()
{
}


template < class T >
inline
DefaultBSwapItemWriter< std::list< T > >::~DefaultBSwapItemWriter()
{
}


template < class T >
inline
void DefaultBSwapItemWriter< std::list< T > >::write(
                                                    std::ostream & os,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListBSwapItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemWriter< std::list< T > >::write(
                                                    largefile_ofstream & os,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListBSwapItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemWriter< std::list< T > >::write(
                                                    largefile_fstream & fs,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListBSwapItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( fs, pItem[ i ] );

  }

}


}


#endif
