#ifndef _gkg_core_io_DefaultItemIOFactoryOfStdList_i_h_
#define _gkg_core_io_DefaultItemIOFactoryOfStdList_i_h_


#include <gkg-core-io/DefaultItemIOFactoryOfStdList.h>
#include <gkg-core-io/DefaultAsciiItemWriterOfStdList_i.h>
#include <gkg-core-io/DefaultBinaryItemWriterOfStdList_i.h>
#include <gkg-core-io/DefaultBSwapItemWriterOfStdList_i.h>
#include <gkg-core-io/DefaultAsciiItemReaderOfStdList_i.h>
#include <gkg-core-io/DefaultBinaryItemReaderOfStdList_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdList_i.h>



template < class T >
inline
gkg::DefaultItemIOFactory< std::list< T > >::DefaultItemIOFactory()
{
}


template < class T >
inline
gkg::DefaultItemIOFactory< std::list< T > >::~DefaultItemIOFactory()
{
}


template < class T >
inline
gkg::ItemWriter< std::list< T > >&
gkg::DefaultItemIOFactory< std::list< T > >::getWriter( bool ascii,
                                                        bool bswap ) const
{

  if ( ascii )
  {

    return gkg::DefaultAsciiItemWriter< std::list< T > >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::DefaultBSwapItemWriter< std::list< T > >::getInstance();

  }
  else
  {

    return gkg::DefaultBinaryItemWriter< std::list< T > >::getInstance();

  }

}


template < class T >
inline
gkg::ItemReader< std::list< T > >&
gkg::DefaultItemIOFactory< std::list< T > >::getReader( bool ascii,
                                                        bool bswap ) const
{

  if ( ascii )
  {

    return gkg::DefaultAsciiItemReader< std::list< T > >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::DefaultBSwapItemReader< std::list< T > >::getInstance();

  }
  else
  {

    return gkg::DefaultBinaryItemReader< std::list< T > >::getInstance();

  }

}


#endif
