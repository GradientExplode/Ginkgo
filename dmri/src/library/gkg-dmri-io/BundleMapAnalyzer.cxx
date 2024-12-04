#include <gkg-dmri-io/BundleMapAnalyzer_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <string>


template class gkg::BundleMapAnalyzer< int8_t >;
template class gkg::BundleMapAnalyzer< uint8_t >;
template class gkg::BundleMapAnalyzer< int16_t >;
template class gkg::BundleMapAnalyzer< uint16_t >;
template class gkg::BundleMapAnalyzer< int32_t >;
template class gkg::BundleMapAnalyzer< uint32_t >;
template class gkg::BundleMapAnalyzer< int64_t >;
template class gkg::BundleMapAnalyzer< uint64_t >;
template class gkg::BundleMapAnalyzer< std::string >;


//
// registrating standard BundleMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< int8_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< int8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< int16_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< int32_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< int64_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BundleMapDiskFormat< std::string >::getInstance().getName(),
         &gkg::BundleMapAnalyzer< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "BundleMap analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
