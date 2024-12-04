#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


template class gkg::DiskFormatFactory<
                                 gkg::BundleMeasureSpreadSheet< int16_t > >;
template class gkg::DiskFormatFactory<
                                 gkg::BundleMeasureSpreadSheet< std::string > >;


