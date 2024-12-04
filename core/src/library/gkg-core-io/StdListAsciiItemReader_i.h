#ifndef _gkg_core_io_StdListAsciiItemReader_i_h_
#define _gkg_core_io_StdListAsciiItemReader_i_h_


#include <gkg-core-io/StdListAsciiItemReader.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


template < class T >
inline
gkg::StdListAsciiItemReader< T >::StdListAsciiItemReader()
                                : gkg::ItemReader< std::list< T > >()
{
}


template < class T >
inline
gkg::StdListAsciiItemReader< T >::~StdListAsciiItemReader()
{
}


template < class T >
inline
void gkg::StdListAsciiItemReader< T >::read( std::istream& is,
                                             std::list< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                                  true, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( true, false );

  std::list< T >* pointer = pItem;
  uint32_t size = 0U;
  T t = T();
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    pointer->clear();

    itemR1.read( is, size );
    for ( size_t l = 0U; l < size; l++ )
    {

      itemR2.read( is, t );
      pointer->push_back( t );

    }

  }

}


template < class T >
inline
void gkg::StdListAsciiItemReader< T >::read( gkg::largefile_ifstream& is,
                                             std::list< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                                  true, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( true, false );

  std::list< T >* pointer = pItem;
  uint32_t size = 0U;
  T t = T();
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    pointer->clear();

    itemR1.read( is, size );
    for ( size_t l = 0U; l < size; l++ )
    {

      itemR2.read( is, t );
      pointer->push_back( t );

    }

  }

}


template < class T >
inline
void gkg::StdListAsciiItemReader< T >::read( gkg::largefile_fstream& fs,
                                             std::list< T >* pItem,
                                             size_t count ) const
{

  gkg::ItemReader< uint32_t >& itemR1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getReader( 
                                                           true, false );
  gkg::ItemReader< T >& itemR2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getReader( true, false );

  std::list< T >* pointer = pItem;
  uint32_t size = 0U;
  T t = T();
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    pointer->clear();

    itemR1.read( fs, size );
    for ( size_t l = 0U; l < size; l++ )
    {

      itemR2.read( fs, t );
      pointer->push_back( t );

    }

  }

}


#endif