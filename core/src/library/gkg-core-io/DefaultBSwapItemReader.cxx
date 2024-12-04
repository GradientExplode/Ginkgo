#include <gkg-core-io/DefaultBSwapItemReader_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


template class gkg::DefaultBSwapItemReader< int8_t >;
template class gkg::DefaultBSwapItemReader< uint8_t >;
template class gkg::DefaultBSwapItemReader< int16_t >;
template class gkg::DefaultBSwapItemReader< uint16_t >;
template class gkg::DefaultBSwapItemReader< int32_t >;
template class gkg::DefaultBSwapItemReader< uint32_t >;
template class gkg::DefaultBSwapItemReader< int64_t >;
template class gkg::DefaultBSwapItemReader< uint64_t >;
template class gkg::DefaultBSwapItemReader< float >;
template class gkg::DefaultBSwapItemReader< double >;
template class gkg::DefaultBSwapItemReader< bool >;
template class gkg::DefaultBSwapItemReader< std::complex< float > >;
template class gkg::DefaultBSwapItemReader< std::complex< double > >;
template class gkg::DefaultBSwapItemReader< std::string >;
