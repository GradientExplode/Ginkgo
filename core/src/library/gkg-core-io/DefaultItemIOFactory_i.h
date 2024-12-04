#ifndef _gkg_core_io_DefaultItemIOFactory_i_h_
#define _gkg_core_io_DefaultItemIOFactory_i_h_


#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-core-io/DefaultBSwapItemReader_i.h>



template < class T >
inline
gkg::DefaultItemIOFactory< T >::DefaultItemIOFactory()
{
}


template < class T >
inline
gkg::DefaultItemIOFactory< T >::~DefaultItemIOFactory()
{
}


template < class T >
inline
gkg::ItemWriter< T >&
gkg::DefaultItemIOFactory< T >::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::DefaultAsciiItemWriter< T >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::DefaultBSwapItemWriter< T >::getInstance();

  }
  else
  {

    return gkg::DefaultBinaryItemWriter< T >::getInstance();

  }

}


template < class T >
inline
gkg::ItemReader< T >&
gkg::DefaultItemIOFactory< T >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::DefaultAsciiItemReader< T >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::DefaultBSwapItemReader< T >::getInstance();

  }
  else
  {

    return gkg::DefaultBinaryItemReader< T >::getInstance();

  }

}


#endif
