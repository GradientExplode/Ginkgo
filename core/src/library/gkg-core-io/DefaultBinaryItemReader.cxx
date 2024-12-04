#include <gkg-core-io/DefaultBinaryItemReader_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


template class gkg::DefaultBinaryItemReader< int8_t >;
template class gkg::DefaultBinaryItemReader< uint8_t >;
template class gkg::DefaultBinaryItemReader< int16_t >;
template class gkg::DefaultBinaryItemReader< uint16_t >;
template class gkg::DefaultBinaryItemReader< int32_t >;
template class gkg::DefaultBinaryItemReader< uint32_t >;
template class gkg::DefaultBinaryItemReader< int64_t >;
template class gkg::DefaultBinaryItemReader< uint64_t >;
template class gkg::DefaultBinaryItemReader< float >;
template class gkg::DefaultBinaryItemReader< double >;
template class gkg::DefaultBinaryItemReader< bool >;
template class gkg::DefaultBinaryItemReader< std::complex< float > >;
template class gkg::DefaultBinaryItemReader< std::complex< double > >;
template class gkg::DefaultBinaryItemReader< std::string >;
