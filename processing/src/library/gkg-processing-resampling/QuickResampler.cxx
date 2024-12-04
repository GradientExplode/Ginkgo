#include <gkg-processing-resampling/QuickResampler.h>


template < class T >
gkg::QuickResampler< T >::QuickResampler()
{
}


template < class T >
gkg::QuickResampler< T >::~QuickResampler()
{
}


template class gkg::QuickResampler< int8_t >;
template class gkg::QuickResampler< uint8_t >;
template class gkg::QuickResampler< int16_t >;
template class gkg::QuickResampler< uint16_t >;
template class gkg::QuickResampler< int32_t >;
template class gkg::QuickResampler< uint32_t >;
template class gkg::QuickResampler< int64_t >;
template class gkg::QuickResampler< uint64_t >;
template class gkg::QuickResampler< float  >;
template class gkg::QuickResampler< double >;
template class gkg::QuickResampler< std::complex< float > >;
template class gkg::QuickResampler< std::complex< double > >;
template class gkg::QuickResampler< gkg::RGBComponent >;
