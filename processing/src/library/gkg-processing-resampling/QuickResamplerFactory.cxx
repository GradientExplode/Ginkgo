#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::QuickResamplerFactory< T >::QuickResamplerFactory()
{
}


template < class T >
gkg::QuickResamplerFactory< T >::~QuickResamplerFactory()
{
}

template < class T >
void gkg::QuickResamplerFactory< T >::registerQuickResampler(
                                      gkg::QuickResampler< T >* quickResampler )
{

  if ( quickResampler )
  {

    _quickResamplers[ quickResampler->getName() ] = quickResampler;

  }

}


template < class T >
gkg::QuickResampler< T >* gkg::QuickResamplerFactory< T >::getQuickResampler(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string, gkg::QuickResampler< T >* >::const_iterator
      s = _quickResamplers.find( name );
    if ( s != _quickResamplers.end() )
    {

      return s->second;

    }
    throw std::runtime_error( std::string( "no " ) +
                              name +
                              " quick resampler found" );
    return 0;

  }
  GKG_CATCH( "template < class T > gkg::QuickResampler< T >* "
             "gkg::QuickResamplerFactory< T >::getQuickResampler( "
             "const std::string& name ) const" );

}


template class gkg::QuickResamplerFactory< int8_t >;
template class gkg::QuickResamplerFactory< uint8_t >;
template class gkg::QuickResamplerFactory< int16_t >;
template class gkg::QuickResamplerFactory< uint16_t >;
template class gkg::QuickResamplerFactory< int32_t >;
template class gkg::QuickResamplerFactory< uint32_t >;
template class gkg::QuickResamplerFactory< int64_t >;
template class gkg::QuickResamplerFactory< uint64_t >;
template class gkg::QuickResamplerFactory< float >;
template class gkg::QuickResamplerFactory< double >;
template class gkg::QuickResamplerFactory< std::complex< float > >;
template class gkg::QuickResamplerFactory< std::complex< double > >;
template class gkg::QuickResamplerFactory< gkg::RGBComponent >;
