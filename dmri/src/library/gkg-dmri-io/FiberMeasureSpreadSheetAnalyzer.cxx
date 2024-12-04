#include <gkg-dmri-io/FiberMeasureSpreadSheetAnalyzer_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <string>


template class gkg::FiberMeasureSpreadSheetAnalyzer< int16_t >;
template class gkg::FiberMeasureSpreadSheetAnalyzer< std::string >;


//
// registrating standard FiberMeasureSpreadSheet analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::FiberMeasureSpreadSheetDiskFormat< int16_t >::getInstance().
                                                                      getName(),
         &gkg::FiberMeasureSpreadSheetAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::FiberMeasureSpreadSheetDiskFormat< std::string >::getInstance().
                                                                      getName(),
         &gkg::FiberMeasureSpreadSheetAnalyzer< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "FiberMeasureSpreadSheet analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
