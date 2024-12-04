#include <gkg-core-io/DefaultAsciiItemReader_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


template class gkg::DefaultAsciiItemReader< int8_t >;
template class gkg::DefaultAsciiItemReader< uint8_t >;
template class gkg::DefaultAsciiItemReader< int16_t >;
template class gkg::DefaultAsciiItemReader< uint16_t >;
template class gkg::DefaultAsciiItemReader< int32_t >;
template class gkg::DefaultAsciiItemReader< uint32_t >;
template class gkg::DefaultAsciiItemReader< int64_t >;
template class gkg::DefaultAsciiItemReader< uint64_t >;
template class gkg::DefaultAsciiItemReader< float >;
template class gkg::DefaultAsciiItemReader< double >;
template class gkg::DefaultAsciiItemReader< bool >;
template class gkg::DefaultAsciiItemReader< std::complex< float > >;
template class gkg::DefaultAsciiItemReader< std::complex< double > >;
template class gkg::DefaultAsciiItemReader< std::string >;
