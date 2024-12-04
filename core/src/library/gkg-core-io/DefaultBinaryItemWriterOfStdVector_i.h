#ifndef _gkg_core_io_DefaultBinaryItemWriterOfStdVector_i_h_
#define _gkg_core_io_DefaultBinaryItemWriterOfStdVector_i_h_


#include <gkg-core-io/DefaultBinaryItemWriterOfStdVector.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>
#include <string>


//
// class DefaultBinaryItemWriter< std::vector< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBinaryItemWriter< std::vector< T > >::DefaultBinaryItemWriter()
                                         : ItemWriter< std::vector< T > >()
{
}


template < class T >
inline
DefaultBinaryItemWriter< std::vector< T > >::~DefaultBinaryItemWriter()
{
}


template < class T >
inline
void DefaultBinaryItemWriter< std::vector< T > >::write(
                                                  std::ostream & os,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorBinaryItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemWriter< std::vector< T > >::write(
                                                  largefile_ofstream & os,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorBinaryItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemWriter< std::vector< T > >::write(
                                                  largefile_fstream & fs,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorBinaryItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( fs, pItem[ i ] );

  }

}


}


#endif
