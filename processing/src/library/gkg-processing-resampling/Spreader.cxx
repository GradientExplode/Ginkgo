#include <gkg-processing-resampling/Spreader.h>


template < class T >
gkg::Spreader< T >::Spreader()
{
}


template < class T >
gkg::Spreader< T >::~Spreader()
{
}


template class gkg::Spreader< int8_t >;
template class gkg::Spreader< uint8_t >;
template class gkg::Spreader< int16_t >;
template class gkg::Spreader< uint16_t >;
template class gkg::Spreader< int32_t >;
template class gkg::Spreader< uint32_t >;
template class gkg::Spreader< int64_t >;
template class gkg::Spreader< uint64_t >;
template class gkg::Spreader< float  >;
template class gkg::Spreader< double >;
template class gkg::Spreader< std::complex< float > >;
template class gkg::Spreader< std::complex< double > >;
template class gkg::Spreader< gkg::RGBComponent >;
