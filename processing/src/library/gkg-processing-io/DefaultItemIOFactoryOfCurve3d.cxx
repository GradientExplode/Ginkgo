#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-io/DefaultAsciiItemWriterOfCurve3d_i.h>
#include <gkg-processing-io/DefaultBinaryItemWriterOfCurve3d_i.h>
#include <gkg-processing-io/DefaultBSwapItemWriterOfCurve3d_i.h>
#include <gkg-processing-io/DefaultAsciiItemReaderOfCurve3d_i.h>
#include <gkg-processing-io/DefaultBinaryItemReaderOfCurve3d_i.h>
#include <gkg-processing-io/DefaultBSwapItemReaderOfCurve3d_i.h>


//
// class DefaultItemIOFactory< Curve3d< float > >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Curve3d< float > >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Curve3d< float > >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< Curve3d< float > >& 
gkg::DefaultItemIOFactory< Curve3d< float > >::getWriter( bool ascii,
                                                          bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Curve3d< float > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Curve3d< float > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Curve3d< float > >::getInstance();

  }

}


template <>
gkg::ItemReader< Curve3d< float > >& 
gkg::DefaultItemIOFactory< Curve3d< float > >::getReader( bool ascii,
                                                          bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Curve3d< float > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Curve3d< float > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Curve3d< float > >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Curve3d< float > >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Curve3d< float > >& reader_curve3d_of_float =
gkg::DefaultItemIOFactory< gkg::Curve3d< float > >::getInstance().getReader(
                                                                        false,
                                                                        false );
gkg::ItemWriter< gkg::Curve3d< float > >& writer_curve3d_of_float =
gkg::DefaultItemIOFactory< gkg::Curve3d< float > >::getInstance().getWriter(
                                                                        false,
                                                                        false );

#endif



//
// class DefaultItemIOFactory< Curve3d< double > >
//


namespace gkg
{


template <>
DefaultItemIOFactory< Curve3d< double > >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< Curve3d< double > >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< Curve3d< double > >& 
gkg::DefaultItemIOFactory< Curve3d< double > >::getWriter( bool ascii,
                                                           bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< Curve3d< double > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< Curve3d< double > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< Curve3d< double > >::getInstance();

  }

}


template <>
gkg::ItemReader< Curve3d< double > >& 
gkg::DefaultItemIOFactory< Curve3d< double > >::getReader( bool ascii,
                                                           bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< Curve3d< double > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< Curve3d< double > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< Curve3d< double > >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::Curve3d< double > >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::Curve3d< double > >& reader_curve3d_of_double =
gkg::DefaultItemIOFactory< gkg::Curve3d< double > >::getInstance().getReader(
                                                                        false,
                                                                        false );
gkg::ItemWriter< gkg::Curve3d< double > >& writer_curve3d_of_double =
gkg::DefaultItemIOFactory< gkg::Curve3d< double > >::getInstance().getWriter(
                                                                        false,
                                                                        false );

#endif



