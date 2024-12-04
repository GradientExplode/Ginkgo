#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-dmri-container/FiberMeasureSpreadSheet_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>


template class gkg::DiskFormatFactory<
                                  gkg::FiberMeasureSpreadSheet< int16_t > >;
template class gkg::DiskFormatFactory<
                                  gkg::FiberMeasureSpreadSheet< std::string > >;


