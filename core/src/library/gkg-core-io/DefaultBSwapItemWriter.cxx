#include <gkg-core-io/DefaultBSwapItemWriter_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


template class gkg::DefaultBSwapItemWriter< int8_t >;
template class gkg::DefaultBSwapItemWriter< uint8_t >;
template class gkg::DefaultBSwapItemWriter< int16_t >;
template class gkg::DefaultBSwapItemWriter< uint16_t >;
template class gkg::DefaultBSwapItemWriter< int32_t >;
template class gkg::DefaultBSwapItemWriter< uint32_t >;
template class gkg::DefaultBSwapItemWriter< int64_t >;
template class gkg::DefaultBSwapItemWriter< uint64_t >;
template class gkg::DefaultBSwapItemWriter< float >;
template class gkg::DefaultBSwapItemWriter< double >;
template class gkg::DefaultBSwapItemWriter< bool >;
template class gkg::DefaultBSwapItemWriter< std::complex< float > >;
template class gkg::DefaultBSwapItemWriter< std::complex< double > >;
template class gkg::DefaultBSwapItemWriter< std::string >;
