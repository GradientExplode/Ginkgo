#include <gkg-core-io/DefaultBinaryItemWriter_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


template class gkg::DefaultBinaryItemWriter< int8_t >;
template class gkg::DefaultBinaryItemWriter< uint8_t >;
template class gkg::DefaultBinaryItemWriter< int16_t >;
template class gkg::DefaultBinaryItemWriter< uint16_t >;
template class gkg::DefaultBinaryItemWriter< int32_t >;
template class gkg::DefaultBinaryItemWriter< uint32_t >;
template class gkg::DefaultBinaryItemWriter< int64_t >;
template class gkg::DefaultBinaryItemWriter< uint64_t >;
template class gkg::DefaultBinaryItemWriter< float >;
template class gkg::DefaultBinaryItemWriter< double >;
template class gkg::DefaultBinaryItemWriter< bool >;
template class gkg::DefaultBinaryItemWriter< std::complex< float > >;
template class gkg::DefaultBinaryItemWriter< std::complex< double > >;
template class gkg::DefaultBinaryItemWriter< std::string >;
