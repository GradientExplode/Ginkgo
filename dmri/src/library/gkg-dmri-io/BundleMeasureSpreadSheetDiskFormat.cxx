#include <gkg-dmri-io/BundleMeasureSpreadSheetDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>

//
// BundleMeasureSpreadSheet disk format singleton instanciation
//

template class gkg::BundleMeasureSpreadSheetDiskFormat< int16_t >;
template class gkg::BundleMeasureSpreadSheetDiskFormat< std::string >;


//
// registrating BundleMeasureSpreadSheet disk format(s) for 
// "BundleMeasureSpreadSheet" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::BundleMeasureSpreadSheet< int16_t > 
                         >::getInstance().registerDiskFormat(
        gkg::BundleMeasureSpreadSheetDiskFormat< int16_t >::getInstance().
                                                                      getName(),
        &gkg::BundleMeasureSpreadSheetDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMeasureSpreadSheet< std::string > 
                         >::getInstance().registerDiskFormat(
        gkg::BundleMeasureSpreadSheetDiskFormat< std::string >::getInstance().
                                                                      getName(),
        &gkg::BundleMeasureSpreadSheetDiskFormat< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "BundleMeasureSpreadSheet disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
