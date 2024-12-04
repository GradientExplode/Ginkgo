#include <gkg-core-io/DefaultItemIOFactoryOfStdList_i.h>
#include <gkg-core-io/DefaultAsciiItemWriterOfStdList_i.h>
#include <gkg-core-io/DefaultBinaryItemWriterOfStdList_i.h>
#include <gkg-core-io/DefaultBSwapItemWriterOfStdList_i.h>
#include <gkg-core-io/DefaultAsciiItemReaderOfStdList_i.h>
#include <gkg-core-io/DefaultBinaryItemReaderOfStdList_i.h>
#include <gkg-core-io/DefaultBSwapItemReaderOfStdList_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <list>


template class gkg::DefaultItemIOFactory< std::list< int8_t > >;
template class gkg::DefaultItemIOFactory< std::list< uint8_t > >;
template class gkg::DefaultItemIOFactory< std::list< int16_t > >;
template class gkg::DefaultItemIOFactory< std::list< uint16_t > >;
template class gkg::DefaultItemIOFactory< std::list< int32_t > >;
template class gkg::DefaultItemIOFactory< std::list< uint32_t > >;
template class gkg::DefaultItemIOFactory< std::list< int64_t > >;
template class gkg::DefaultItemIOFactory< std::list< uint64_t > >;
template class gkg::DefaultItemIOFactory< std::list< float > >;
template class gkg::DefaultItemIOFactory< std::list< double > >;
