#include <gkg-core-io/DefaultItemIOFactory.h>

#include <gkg-simulation-io/DefaultAsciiItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemWriterOfAtom_i.h>
#include <gkg-simulation-io/DefaultAsciiItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBinaryItemReaderOfAtom_i.h>
#include <gkg-simulation-io/DefaultBSwapItemReaderOfAtom_i.h>

#include <gkg-simulation-io/DefaultAsciiItemWriterOfCell_i.h>
#include <gkg-simulation-io/DefaultBinaryItemWriterOfCell_i.h>
#include <gkg-simulation-io/DefaultBSwapItemWriterOfCell_i.h>
#include <gkg-simulation-io/DefaultAsciiItemReaderOfCell_i.h>
#include <gkg-simulation-io/DefaultBinaryItemReaderOfCell_i.h>
#include <gkg-simulation-io/DefaultBSwapItemReaderOfCell_i.h>


//
// class DefaultItemIOFactory< PAtom >
//


namespace gkg
{


template <>
DefaultItemIOFactory< PAtom >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< PAtom >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< PAtom >& 
gkg::DefaultItemIOFactory< PAtom >::getWriter( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< PAtom >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< PAtom >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< PAtom >::getInstance();

  }

}


template <>
gkg::ItemReader< PAtom >& 
gkg::DefaultItemIOFactory< PAtom >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< PAtom >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< PAtom >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< PAtom >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::PAtom >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::PAtom >& reader_sphere_atom =
gkg::DefaultItemIOFactory< gkg::PAtom >::getInstance().getReader( false,
                                                                  false );
gkg::ItemWriter< gkg::PAtom >& writer_sphere_atom =
gkg::DefaultItemIOFactory< gkg::PAtom >::getInstance().getWriter( false,
                                                                  false );

#endif



//
// class DefaultItemIOFactory< Cell >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Cell >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Cell >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< Cell >& 
gkg::DefaultItemIOFactory< Cell >::getWriter(  bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Cell >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Cell >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Cell >::getInstance();

  }

}


template <>
gkg::ItemReader< Cell >& 
gkg::DefaultItemIOFactory< Cell >::getReader( bool ascii, bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Cell >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Cell >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Cell >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Cell >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Cell >& reader_cell =
gkg::DefaultItemIOFactory< gkg::Cell >::getInstance().getReader( false,
                                                                 false );
gkg::ItemWriter< gkg::Cell >& writer_cell =
gkg::DefaultItemIOFactory< gkg::Cell >::getInstance().getWriter( false,
                                                                 false );

#endif


