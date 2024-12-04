#include <gkg-core-io/DefaultAsciiItemWriter_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>

template class gkg::DefaultAsciiItemWriter< int8_t >;
template class gkg::DefaultAsciiItemWriter< uint8_t >;
template class gkg::DefaultAsciiItemWriter< int16_t >;
template class gkg::DefaultAsciiItemWriter< uint16_t >;
template class gkg::DefaultAsciiItemWriter< int32_t >;
template class gkg::DefaultAsciiItemWriter< uint32_t >;
template class gkg::DefaultAsciiItemWriter< int64_t >;
template class gkg::DefaultAsciiItemWriter< uint64_t >;
template class gkg::DefaultAsciiItemWriter< float >;
template class gkg::DefaultAsciiItemWriter< double >;
template class gkg::DefaultAsciiItemWriter< bool >;
template class gkg::DefaultAsciiItemWriter< std::complex< float > >;
template class gkg::DefaultAsciiItemWriter< std::complex< double > >;
template class gkg::DefaultAsciiItemWriter< std::string >;
