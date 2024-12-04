#include <gkg-core-io/DefaultItemIOFactoryOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemWriterOfStdVector_i.h>
#include <gkg-core-io/DefaultAsciiItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBinaryItemReaderOfStdVector_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdVector_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <vector>


template class gkg::DefaultItemIOFactory< std::vector< int8_t > >;
template class gkg::DefaultItemIOFactory< std::vector< uint8_t > >;
template class gkg::DefaultItemIOFactory< std::vector< int16_t > >;
template class gkg::DefaultItemIOFactory< std::vector< uint16_t > >;
template class gkg::DefaultItemIOFactory< std::vector< int32_t > >;
template class gkg::DefaultItemIOFactory< std::vector< uint32_t > >;
template class gkg::DefaultItemIOFactory< std::vector< int64_t > >;
template class gkg::DefaultItemIOFactory< std::vector< uint64_t > >;
template class gkg::DefaultItemIOFactory< std::vector< float > >;
template class gkg::DefaultItemIOFactory< std::vector< double > >;
