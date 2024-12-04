#ifndef _gkg_core_io_DefaultBinaryItemReaderOfStdVector_i_h_
#define _gkg_core_io_DefaultBinaryItemReaderOfStdVector_i_h_


#include <gkg-core-io/DefaultBinaryItemReaderOfStdVector.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>
#include <string>


//
// class DefaultBinaryItemReader< std::vector< T > >
//

namespace gkg
{


template < class T >
inline
DefaultBinaryItemReader< std::vector< T > >::DefaultBinaryItemReader()
                                         : ItemReader< std::vector< T > >()
{
}


template < class T >
inline
DefaultBinaryItemReader< std::vector< T > >::~DefaultBinaryItemReader()
{
}


template < class T >
inline
void DefaultBinaryItemReader< std::vector< T > >::read( std::istream & is,
                                                        std::vector< T >* pItem,
                                                        size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorBinaryItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemReader< std::vector< T > >::read( largefile_ifstream & is,
                                                        std::vector< T >* pItem,
                                                        size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorBinaryItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( is, pItem[ i ] );

  }

}


template < class T >
inline
void DefaultBinaryItemReader< std::vector< T > >::read( largefile_fstream & fs,
                                                        std::vector< T >* pItem,
                                                        size_t count ) const
{

  ItemReader< std::vector< T > >& itemR_Stdvector =
    StdVectorBinaryItemReader< T >::getInstance();
  
  size_t i;
  for ( i = 0; i < count; i++ )
  {

    itemR_Stdvector.read( fs, pItem[ i ] );

  }

}


}


#endif
