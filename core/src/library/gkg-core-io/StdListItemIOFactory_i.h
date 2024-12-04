#ifndef _gkg_core_io_StdListItemIOFactory_i_h_
#define _gkg_core_io_StdListItemIOFactory_i_h_


#include <gkg-core-io/StdListItemIOFactory.h>
#include <gkg-core-io/StdListAsciiItemWriter_i.h>
#include <gkg-core-io/StdListBinaryItemWriter_i.h>
#include <gkg-core-io/StdListBSwapItemWriter_i.h>
#include <gkg-core-io/StdListAsciiItemReader_i.h>
#include <gkg-core-io/StdListBinaryItemReader_i.h>
#include <gkg-core-io/StdListBSwapItemReader_i.h>


template < class T >
inline
gkg::StdListItemIOFactory< T >::StdListItemIOFactory()
{
}


template < class T >
inline
gkg::StdListItemIOFactory< T >::~StdListItemIOFactory()
{
}


template < class T >
inline
gkg::ItemWriter< std::list< T > >& 
gkg::StdListItemIOFactory< T >::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::StdListAsciiItemWriter< T >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::StdListBSwapItemWriter< T >::getInstance();

  }
  else
  {

    return gkg::StdListBinaryItemWriter< T >::getInstance();

  }

}


template < class T >
inline
gkg::ItemReader< std::list< T > >& 
gkg::StdListItemIOFactory< T >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::StdListAsciiItemReader< T >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::StdListBSwapItemReader< T >::getInstance();

  }
  else
  {

    return gkg::StdListBinaryItemReader< T >::getInstance();

  }

}


#endif
