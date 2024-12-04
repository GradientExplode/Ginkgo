#ifndef _gkg_core_io_StdVectorItemIOFactory_i_h_
#define _gkg_core_io_StdVectorItemIOFactory_i_h_


#include <gkg-core-io/StdVectorItemIOFactory.h>
#include <gkg-core-io/StdVectorAsciiItemWriter_i.h>
#include <gkg-core-io/StdVectorBinaryItemWriter_i.h>
#include <gkg-core-io/StdVectorBSwapItemWriter_i.h>
#include <gkg-core-io/StdVectorAsciiItemReader_i.h>
#include <gkg-core-io/StdVectorBinaryItemReader_i.h>
#include <gkg-core-io/StdVectorBSwapItemReader_i.h>


template < class T >
inline
gkg::StdVectorItemIOFactory< T >::StdVectorItemIOFactory()
{
}


template < class T >
inline
gkg::StdVectorItemIOFactory< T >::~StdVectorItemIOFactory()
{
}


template < class T >
inline
gkg::ItemWriter< std::vector< T > >& 
gkg::StdVectorItemIOFactory< T >::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::StdVectorAsciiItemWriter< T >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::StdVectorBSwapItemWriter< T >::getInstance();

  }
  else
  {

    return gkg::StdVectorBinaryItemWriter< T >::getInstance();

  }

}


template < class T >
inline
gkg::ItemReader< std::vector< T > >&
gkg::StdVectorItemIOFactory< T >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return gkg::StdVectorAsciiItemReader< T >::getInstance();

  }
  else if ( bswap )
  {

    return gkg::StdVectorBSwapItemReader< T >::getInstance();

  }
  else
  {

    return gkg::StdVectorBinaryItemReader< T >::getInstance();

  }

}


#endif
