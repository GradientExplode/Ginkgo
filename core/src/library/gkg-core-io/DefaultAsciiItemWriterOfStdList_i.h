#ifndef _gkg_core_io_DefaultAsciiItemWriterOfStdList_i_h_
#define _gkg_core_io_DefaultAsciiItemWriterOfStdList_i_h_


#include <gkg-core-io/DefaultAsciiItemWriterOfStdList.h>
#include <gkg-core-io/StdListAsciiItemWriter_i.h>
#include <string>


//
// class DefaultAsciiItemWriter< std::list< T > >
//

namespace gkg
{


template < class T >
inline
DefaultAsciiItemWriter< std::list< T > >::DefaultAsciiItemWriter()
                                         : ItemWriter< std::list< T > >()
{
}


template < class T >
inline
DefaultAsciiItemWriter< std::list< T > >::~DefaultAsciiItemWriter()
{
}


template < class T >
inline
void DefaultAsciiItemWriter< std::list< T > >::write(
                                                    std::ostream & os,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListAsciiItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemWriter< std::list< T > >::write(
                                                    largefile_ofstream & os,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListAsciiItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemWriter< std::list< T > >::write(
                                                    largefile_fstream & fs,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListAsciiItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( fs, pItem[ i ] );

  }

}


}


#endif
