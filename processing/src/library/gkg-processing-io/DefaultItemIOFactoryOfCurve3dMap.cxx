#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-io/DefaultAsciiItemWriterOfCurve3dMap_i.h>
#include <gkg-processing-io/DefaultBinaryItemWriterOfCurve3dMap_i.h>
#include <gkg-processing-io/DefaultBSwapItemWriterOfCurve3dMap_i.h>
#include <gkg-processing-io/DefaultAsciiItemReaderOfCurve3dMap_i.h>
#include <gkg-processing-io/DefaultBinaryItemReaderOfCurve3dMap_i.h>
#include <gkg-processing-io/DefaultBSwapItemReaderOfCurve3dMap_i.h>


//
// class DefaultItemIOFactory< Curve3dMap< float > >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Curve3dMap< float > >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Curve3dMap< float > >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< Curve3dMap< float > >& 
gkg::DefaultItemIOFactory< Curve3dMap< float > >::getWriter( bool ascii,
                                                             bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Curve3dMap< float > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Curve3dMap< float > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Curve3dMap< float > >::getInstance();

  }

}


template <>
gkg::ItemReader< Curve3dMap< float > >& 
gkg::DefaultItemIOFactory< Curve3dMap< float > >::getReader( bool ascii,
                                                             bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Curve3dMap< float > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Curve3dMap< float > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Curve3dMap< float > >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Curve3dMap< float > >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Curve3dMap< float > >& reader_curve3dmap_of_float =
gkg::DefaultItemIOFactory< gkg::Curve3dMap< float > >::getInstance().getReader(
                                                                        false,
                                                                        false );
gkg::ItemWriter< gkg::Curve3dMap< float > >& writer_curve3dmap_of_float =
gkg::DefaultItemIOFactory< gkg::Curve3dMap< float > >::getInstance().getWriter(
                                                                        false,
                                                                        false );

#endif



//
// class DefaultItemIOFactory< Curve3dMap< double > >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Curve3dMap< double > >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Curve3dMap< double > >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< Curve3dMap< double > >& 
gkg::DefaultItemIOFactory< Curve3dMap< double > >::getWriter( bool ascii,
                                                              bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Curve3dMap< double > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Curve3dMap< double > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Curve3dMap< double > >::getInstance();

  }

}


template <>
gkg::ItemReader< Curve3dMap< double > >& 
gkg::DefaultItemIOFactory< Curve3dMap< double > >::getReader( bool ascii,
                                                              bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Curve3dMap< double > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Curve3dMap< double > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Curve3dMap< double > >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Curve3dMap< double > >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Curve3dMap< double > >& reader_curve3dmap_of_double =
gkg::DefaultItemIOFactory< gkg::Curve3dMap< double > >::getInstance().getReader(
                                                                        false,
                                                                        false );
gkg::ItemWriter< gkg::Curve3dMap< double > >& writer_curve3dmap_of_double =
gkg::DefaultItemIOFactory< gkg::Curve3dMap< double > >::getInstance().getWriter(
                                                                        false,
                                                                        false );

#endif



