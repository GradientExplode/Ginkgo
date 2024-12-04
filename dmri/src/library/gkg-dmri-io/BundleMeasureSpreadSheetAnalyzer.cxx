#include <gkg-dmri-io/BundleMeasureSpreadSheetAnalyzer_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <string>


template class gkg::BundleMeasureSpreadSheetAnalyzer< int16_t >;
template class gkg::BundleMeasureSpreadSheetAnalyzer< std::string >;


//
// registrating standard FiberMeasureSpreadSheet analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMeasureSpreadSheetDiskFormat< int16_t >::getInstance().
                                                                      getName(),
         &gkg::BundleMeasureSpreadSheetAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMeasureSpreadSheetDiskFormat< std::string >::getInstance().
                                                                      getName(),
         &gkg::BundleMeasureSpreadSheetAnalyzer< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "BundleMeasureSpreadSheet analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
