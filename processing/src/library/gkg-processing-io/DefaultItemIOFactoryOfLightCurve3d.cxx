#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-processing-io/DefaultAsciiItemWriterOfLightCurve3d_i.h>
#include <gkg-processing-io/DefaultBinaryItemWriterOfLightCurve3d_i.h>
#include <gkg-processing-io/DefaultBSwapItemWriterOfLightCurve3d_i.h>
#include <gkg-processing-io/DefaultAsciiItemReaderOfLightCurve3d_i.h>
#include <gkg-processing-io/DefaultBinaryItemReaderOfLightCurve3d_i.h>
#include <gkg-processing-io/DefaultBSwapItemReaderOfLightCurve3d_i.h>


//
// class DefaultItemIOFactory< LightCurve3d< float > >
//


namespace gkg
{


template <>
DefaultItemIOFactory< LightCurve3d< float > >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< LightCurve3d< float > >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< LightCurve3d< float > >& 
gkg::DefaultItemIOFactory< LightCurve3d< float > >::getWriter(
                                                              bool ascii,
                                                              bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< LightCurve3d< float > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< LightCurve3d< float > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< LightCurve3d< float > >::getInstance();

  }

}


template <>
gkg::ItemReader< LightCurve3d< float > >& 
gkg::DefaultItemIOFactory< LightCurve3d< float > >::getReader(
                                                              bool ascii,
                                                              bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< LightCurve3d< float > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< LightCurve3d< float > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< LightCurve3d< float > >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::LightCurve3d< float > >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::LightCurve3d< float > >& reader_curve3d_of_float =
gkg::DefaultItemIOFactory< gkg::LightCurve3d< float > >::getInstance().
                                                             getReader( false,
                                                                        false );
gkg::ItemWriter< gkg::LightCurve3d< float > >& writer_curve3d_of_float =
gkg::DefaultItemIOFactory< gkg::LightCurve3d< float > >::getInstance().
                                                             getWriter( false,
                                                                        false );

#endif



//
// class DefaultItemIOFactory< LightCurve3d< double > >
//


namespace gkg
{


template <>
DefaultItemIOFactory< LightCurve3d< double > >::DefaultItemIOFactory()
{
}


template <>
DefaultItemIOFactory< LightCurve3d< double > >::~DefaultItemIOFactory()
{
}


template <>
gkg::ItemWriter< LightCurve3d< double > >& 
gkg::DefaultItemIOFactory< LightCurve3d< double > >::getWriter( 
                                                              bool ascii,
                                                              bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemWriter< LightCurve3d< double > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemWriter< LightCurve3d< double > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemWriter< LightCurve3d< double > >::getInstance();

  }

}


template <>
gkg::ItemReader< LightCurve3d< double > >& 
gkg::DefaultItemIOFactory< LightCurve3d< double > >::getReader(
                                                              bool ascii,
                                                              bool bswap ) const
{

  if ( ascii )
  {

    return DefaultAsciiItemReader< LightCurve3d< double > >::getInstance();

  }
  else if ( bswap )
  {

    return DefaultBSwapItemReader< LightCurve3d< double > >::getInstance();

  }
  else
  {

    return DefaultBinaryItemReader< LightCurve3d< double > >::getInstance();

  }

}


}


template class gkg::DefaultItemIOFactory< gkg::LightCurve3d< double > >;


// forcing method instanciation under gcc-3
#if defined( __GNUC__ ) && (__GNUC__-0 < 3)

gkg::ItemReader< gkg::LightCurve3d< double > >& reader_curve3d_of_double =
gkg::DefaultItemIOFactory< gkg::LightCurve3d< double > >::getInstance().
                                                             getReader( false,
                                                                        false );
gkg::ItemWriter< gkg::LightCurve3d< double > >& writer_curve3d_of_double =
gkg::DefaultItemIOFactory< gkg::LightCurve3d< double > >::getInstance().
                                                             getWriter( false,
                                                                        false );

#endif



