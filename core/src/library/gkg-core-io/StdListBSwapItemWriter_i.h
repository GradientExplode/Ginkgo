#ifndef _gkg_core_io_StdListBSwapItemWriter_i_h_
#define _gkg_core_io_StdListBSwapItemWriter_i_h_


#include <gkg-core-io/StdListBSwapItemWriter.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>


template < class T >
inline
gkg::StdListBSwapItemWriter< T >::StdListBSwapItemWriter()
                              : gkg::ItemWriter< std::list< T > >()
{
}


template < class T >
inline
gkg::StdListBSwapItemWriter< T >::~StdListBSwapItemWriter()
{
}


template < class T >
inline
void gkg::StdListBSwapItemWriter< T >::write( std::ostream & os,
                                           const std::list< T >* pItem,
                                           size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           false, true );
  gkg::ItemWriter< T >& itemW2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getWriter( false, true );

  const std::list< T >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    typename std::list< T >::const_iterator l = pointer->begin(),
                                            le = pointer->end();
    while ( l != le )
    {

      itemW2.write( os, *l );
      ++ l;

    }

  }

}


template < class T >
inline
void gkg::StdListBSwapItemWriter< T >::write( gkg::largefile_ofstream & os,
                                           const std::list< T >* pItem,
                                           size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           false, true );
  gkg::ItemWriter< T >& itemW2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getWriter( false, true );

  const std::list< T >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( os, pointer->size() );
    typename std::list< T >::const_iterator l = pointer->begin(),
                                            le = pointer->end();
    while ( l != le )
    {

      itemW2.write( os, *l );
      ++ l;

    }

  }

}


template < class T >
inline
void gkg::StdListBSwapItemWriter< T >::write( gkg::largefile_fstream & fs,
                                           const std::list< T >* pItem,
                                           size_t count ) const
{

  gkg::ItemWriter< uint32_t >& itemW1 =
    gkg::DefaultItemIOFactory< uint32_t >::getInstance().getWriter( 
                                                           false, true );
  gkg::ItemWriter< T >& itemW2 =
    gkg::DefaultItemIOFactory< T >::getInstance().getWriter( false, true );

  const std::list< T >* pointer = pItem;
  for ( size_t i = 0; i < count; ++i, ++pointer )
  {

    itemW1.write( fs, pointer->size() );
    typename std::list< T >::const_iterator l = pointer->begin(),
                                            le = pointer->end();
    while ( l != le )
    {

      itemW2.write( fs, *l );
      ++ l;

    }

  }

}


#endif
