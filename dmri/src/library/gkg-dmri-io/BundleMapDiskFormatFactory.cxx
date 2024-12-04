#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


template class gkg::DiskFormatFactory< gkg::BundleMap< int8_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< uint8_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< int16_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< uint16_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< int32_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< uint32_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< int64_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< uint64_t > >;
template class gkg::DiskFormatFactory< gkg::BundleMap< std::string > >;


