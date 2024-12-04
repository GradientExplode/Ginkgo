#ifndef _gkg_core_io_DefaultAsciiItemWriterOfStdVector_i_h_
#define _gkg_core_io_DefaultAsciiItemWriterOfStdVector_i_h_


#include <gkg-core-io/DefaultAsciiItemWriterOfStdVector.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>
#include <string>


//
// class DefaultAsciiItemWriter< std::vector< T > >
//

namespace gkg
{


template < class T >
inline
DefaultAsciiItemWriter< std::vector< T > >::DefaultAsciiItemWriter()
                                         : ItemWriter< std::vector< T > >()
{
}


template < class T >
inline
DefaultAsciiItemWriter< std::vector< T > >::~DefaultAsciiItemWriter()
{
}


template < class T >
inline
void DefaultAsciiItemWriter< std::vector< T > >::write(
                                                  std::ostream & os,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorAsciiItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemWriter< std::vector< T > >::write(
                                                  largefile_ofstream & os,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorAsciiItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( os, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultAsciiItemWriter< std::vector< T > >::write(
                                                  largefile_fstream & fs,
                                                  const std::vector< T >* pItem,
                                                  size_t count ) const
{

  ItemWriter< std::vector< T > >& itemW_StdVector =
    StdVectorAsciiItemWriter< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemW_StdVector.write( fs, pItem[ i ] );

  }

}


}


#endif
