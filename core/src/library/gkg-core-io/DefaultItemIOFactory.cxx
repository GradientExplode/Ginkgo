#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <complex>
#include <string>


template class gkg::DefaultItemIOFactory< int8_t >;
template class gkg::DefaultItemIOFactory< uint8_t >;
template class gkg::DefaultItemIOFactory< int16_t >;
template class gkg::DefaultItemIOFactory< uint16_t >;
template class gkg::DefaultItemIOFactory< int32_t >;
template class gkg::DefaultItemIOFactory< uint32_t >;
template class gkg::DefaultItemIOFactory< int64_t >;
template class gkg::DefaultItemIOFactory< uint64_t >;
template class gkg::DefaultItemIOFactory< float >;
template class gkg::DefaultItemIOFactory< double >;
template class gkg::DefaultItemIOFactory< bool >;
template class gkg::DefaultItemIOFactory< std::complex< float > >;
template class gkg::DefaultItemIOFactory< std::complex< double > >;
template class gkg::DefaultItemIOFactory< std::string >;
