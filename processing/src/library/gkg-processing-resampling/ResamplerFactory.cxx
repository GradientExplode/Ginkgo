#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/NearestNeighborResampler.h>
#include <gkg-processing-resampling/LinearResampler.h>
#include <gkg-processing-resampling/QuadraticResampler.h>
#include <gkg-processing-resampling/CubicResampler.h>
#include <gkg-processing-resampling/QuarticResampler.h>
#include <gkg-processing-resampling/QuinticResampler.h>
#include <gkg-processing-resampling/SixthOrderResampler.h>
#include <gkg-processing-resampling/SeventhOrderResampler.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::ResamplerFactory< T >::ResamplerFactory()
{
}


template < class T >
gkg::ResamplerFactory< T >::~ResamplerFactory()
{
}

template < class T >
void gkg::ResamplerFactory< T >::registerResampler( 
                                                gkg::Resampler< T >* resampler )
{

  if ( resampler )
  {

    _resamplers[ resampler->getOrder() ] = resampler;

  }

}


template < class T >
gkg::Resampler< T >* 
gkg::ResamplerFactory< T >::getResampler( int32_t order ) const
{

  try
  {

    typename std::map< int32_t, gkg::Resampler< T >* >::const_iterator
      r = _resamplers.find( order );
    if ( r != _resamplers.end() )
    {

      return r->second;

    }
    throw std::runtime_error( std::string( "no " ) +
                              gkg::StringConverter::toString( order ) +
                              " order resampler found" );
    return 0;

  }
  GKG_CATCH( "template < class T > gkg::Resampler< T >* "
             "gkg::ResamplerFactory< T >::getResampler( int order ) const" );

}


template class gkg::ResamplerFactory< int8_t >;
template class gkg::ResamplerFactory< uint8_t >;
template class gkg::ResamplerFactory< int16_t >;
template class gkg::ResamplerFactory< uint16_t >;
template class gkg::ResamplerFactory< int32_t >;
template class gkg::ResamplerFactory< uint32_t >;
template class gkg::ResamplerFactory< int64_t >;
template class gkg::ResamplerFactory< uint64_t >;
template class gkg::ResamplerFactory< float >;
template class gkg::ResamplerFactory< double >;
template class gkg::ResamplerFactory< std::complex< float > >;
template class gkg::ResamplerFactory< std::complex< double > >;
template class gkg::ResamplerFactory< gkg::RGBComponent >;
