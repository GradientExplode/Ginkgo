#include <gkg-processing-resampling/SpreaderFactory.h>
#include <gkg-processing-resampling/PartialVolumingSpreader.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::SpreaderFactory< T >::SpreaderFactory()
{
}


template < class T >
gkg::SpreaderFactory< T >::~SpreaderFactory()
{
}

template < class T >
void gkg::SpreaderFactory< T >::registerSpreader( gkg::Spreader< T >* spreader )
{

  if ( spreader )
  {

    _spreaders[ spreader->getName() ] = spreader;

  }

}


template < class T >
gkg::Spreader< T >* gkg::SpreaderFactory< T >::getSpreader(
                                                 const std::string& name ) const
{

  try
  {

    typename std::map< std::string, gkg::Spreader< T >* >::const_iterator
      s = _spreaders.find( name );
    if ( s != _spreaders.end() )
    {

      return s->second;

    }
    throw std::runtime_error( std::string( "no " ) +
                              name +
                              " spreader found" );
    return 0;

  }
  GKG_CATCH( "template < class T > gkg::Spreader< T >* "
             "gkg::SpreaderFactory< T >::getSpreader( "
             "const std::string& name ) const" );

}


template class gkg::SpreaderFactory< int8_t >;
template class gkg::SpreaderFactory< uint8_t >;
template class gkg::SpreaderFactory< int16_t >;
template class gkg::SpreaderFactory< uint16_t >;
template class gkg::SpreaderFactory< int32_t >;
template class gkg::SpreaderFactory< uint32_t >;
template class gkg::SpreaderFactory< int64_t >;
template class gkg::SpreaderFactory< uint64_t >;
template class gkg::SpreaderFactory< float >;
template class gkg::SpreaderFactory< double >;
template class gkg::SpreaderFactory< std::complex< float > >;
template class gkg::SpreaderFactory< std::complex< double > >;
template class gkg::SpreaderFactory< gkg::RGBComponent >;
