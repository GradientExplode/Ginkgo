#ifndef _gkg_core_io_DefaultItemIOFactoryOfStdVector_i_h_
#define _gkg_core_io_DefaultItemIOFactoryOfStdVector_i_h_


#include <gkg-core-io/DefaultItemIOFactoryOfStdVector.h>
#include <gkg-core-io/DefaultAsciiItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector_i.h>



template < class T >
inline
gkg::DefaultItemIOFactory< std::vector< T > >::DefaultItemIOFactory()
{
}


template < class T >
inline
gkg::DefaultItemIOFactory< std::vector< T > >::~DefaultItemIOFactory()
{
}


template < class T >
inline
gkg::ItemWriter< std::vector< T > >&
gkg::DefaultItemIOFactory< std::vector< T > >::getWriter( bool ascii,
                                                        bool bswap ) const
{

  if ( ascii )
  {

    return gkg::DefaultAsciiItemWriter< std::vector< T > >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::DefaultBSwapItemWriter< std::vector< T > >::getInstance();

  }
  else
  {

    return gkg::DefaultBinaryItemWriter< std::vector< T > >::getInstance();

  }

}


template < class T >
inline
gkg::ItemReader< std::vector< T > >&
gkg::DefaultItemIOFactory< std::vector< T > >::getReader( bool ascii,
                                                        bool bswap ) const
{

  if ( ascii )
  {

    return gkg::DefaultAsciiItemReader< std::vector< T > >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::DefaultBSwapItemReader< std::vector< T > >::getInstance();

  }
  else
  {

    return gkg::DefaultBinaryItemReader< std::vector< T > >::getInstance();

  }

}


#endif
