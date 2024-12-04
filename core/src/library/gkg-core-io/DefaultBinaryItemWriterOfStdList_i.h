#ifndef _gkg_core_io_DefaultBinaryItemWriterOfStdList_i_h_
#define _gkg_core_io_DefaultBinaryItemWriterOfStdList_i_h_


#include <gkg-core-io/DefaultBinaryItemWriterOfStdList.h>
#include <gkg-core-io/StdListBinaryItemWriter_i.h>
#include <string>


//
// class DefaultBinaryItemWriter< std::list< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBinaryItemWriter< std::list< T > >::DefaultBinaryItemWriter()
                                         : ItemWriter< std::list< T > >()
{
}


template < class T >
inline
DefaultBinaryItemWriter< std::list< T > >::~DefaultBinaryItemWriter()
{
}


template < class T >
inline
void DefaultBinaryItemWriter< std::list< T > >::write(
                                                    std::ostream & os,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListBinaryItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemWriter< std::list< T > >::write(
                                                    largefile_ofstream & os,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListBinaryItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemWriter< std::list< T > >::write(
                                                    largefile_fstream & fs,
                                                    const std::list< T >* pItem,
                                                    size_t count ) const
{

  ItemWriter< std::list< T > >& itemW_StdList =
    StdListBinaryItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdList.write( fs, pItem[ i ] );

  }

}


}


#endif
