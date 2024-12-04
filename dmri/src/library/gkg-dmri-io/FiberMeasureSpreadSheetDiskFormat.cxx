#include <gkg-dmri-io/FiberMeasureSpreadSheetDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>

//
// FiberMeasureSpreadSheet disk format singleton instanciation
//

template class gkg::FiberMeasureSpreadSheetDiskFormat< int16_t >;
template class gkg::FiberMeasureSpreadSheetDiskFormat< std::string >;


//
// registrating FiberMeasureSpreadSheet disk format(s) for 
// "FiberMeasureSpreadSheet" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::FiberMeasureSpreadSheet< int16_t > 
                         >::getInstance().registerDiskFormat(
        gkg::FiberMeasureSpreadSheetDiskFormat< int16_t >::getInstance().
                                                                      getName(),
        &gkg::FiberMeasureSpreadSheetDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::FiberMeasureSpreadSheet< std::string > 
                         >::getInstance().registerDiskFormat(
        gkg::FiberMeasureSpreadSheetDiskFormat< std::string >::getInstance().
                                                                      getName(),
        &gkg::FiberMeasureSpreadSheetDiskFormat< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "FiberMeasureSpreadSheet disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
