#ifndef _gkg_core_io_DefaultBSwapItemWriterOfStdVector_i_h_
#define _gkg_core_io_DefaultBSwapItemWriterOfStdVector_i_h_


#include <gkg-core-io/DefaultBSwapItemWriterOfStdVector.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>
#include <string>


//
// class DefaultBSwapItemWriter< std::vector< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBSwapItemWriter< std::vector< T > >::DefaultBSwapItemWriter()
                                         : ItemWriter< std::vector< T > >()
{
}


template < class T >
inline
DefaultBSwapItemWriter< std::vector< T > >::~DefaultBSwapItemWriter()
{
}


template < class T >
inline
void DefaultBSwapItemWriter< std::vector< T > >::write(
                                                  std::ostream & os,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorBSwapItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemWriter< std::vector< T > >::write(
                                                  largefile_ofstream & os,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorBSwapItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBSwapItemWriter< std::vector< T > >::write(
                                                   largefile_fstream & fs,
                                                   const std::vector< T >* pItem,
                                                   size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorBSwapItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( fs, pItem[ i ] );

  }

}


}


#endif
