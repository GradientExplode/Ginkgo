#include <gkg-processing-io/SiteMapAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>


template class gkg::SiteMapAnalyzer< int8_t, int16_t >;
template class gkg::SiteMapAnalyzer< uint8_t, int16_t >;
template class gkg::SiteMapAnalyzer< int16_t, int16_t >;
template class gkg::SiteMapAnalyzer< uint16_t, int16_t >;
template class gkg::SiteMapAnalyzer< int32_t, int16_t >;
template class gkg::SiteMapAnalyzer< uint32_t, int16_t >;
template class gkg::SiteMapAnalyzer< int64_t, int16_t >;
template class gkg::SiteMapAnalyzer< uint64_t, int16_t >;
template class gkg::SiteMapAnalyzer< float, int16_t >;
template class gkg::SiteMapAnalyzer< double, int16_t >;

template class gkg::SiteMapAnalyzer< int8_t, int32_t >;
template class gkg::SiteMapAnalyzer< uint8_t, int32_t >;
template class gkg::SiteMapAnalyzer< int16_t, int32_t >;
template class gkg::SiteMapAnalyzer< uint16_t, int32_t >;
template class gkg::SiteMapAnalyzer< int32_t, int32_t >;
template class gkg::SiteMapAnalyzer< uint32_t, int32_t >;
template class gkg::SiteMapAnalyzer< int64_t, int32_t >;
template class gkg::SiteMapAnalyzer< uint64_t, int32_t >;
template class gkg::SiteMapAnalyzer< float, int32_t >;
template class gkg::SiteMapAnalyzer< double, int32_t >;

template class gkg::SiteMapAnalyzer< int8_t, int64_t >;
template class gkg::SiteMapAnalyzer< uint8_t, int64_t >;
template class gkg::SiteMapAnalyzer< int16_t, int64_t >;
template class gkg::SiteMapAnalyzer< uint16_t, int64_t >;
template class gkg::SiteMapAnalyzer< int32_t, int64_t >;
template class gkg::SiteMapAnalyzer< uint32_t, int64_t >;
template class gkg::SiteMapAnalyzer< int64_t, int64_t >;
template class gkg::SiteMapAnalyzer< uint64_t, int64_t >;
template class gkg::SiteMapAnalyzer< float, int64_t >;
template class gkg::SiteMapAnalyzer< double, int64_t >;

template class gkg::SiteMapAnalyzer< int8_t, uint16_t >;
template class gkg::SiteMapAnalyzer< uint8_t, uint16_t >;
template class gkg::SiteMapAnalyzer< int16_t, uint16_t >;
template class gkg::SiteMapAnalyzer< uint16_t, uint16_t >;
template class gkg::SiteMapAnalyzer< int32_t, uint16_t >;
template class gkg::SiteMapAnalyzer< uint32_t, uint16_t >;
template class gkg::SiteMapAnalyzer< int64_t, uint16_t >;
template class gkg::SiteMapAnalyzer< uint64_t, uint16_t >;
template class gkg::SiteMapAnalyzer< float, uint16_t >;
template class gkg::SiteMapAnalyzer< double, uint16_t >;

template class gkg::SiteMapAnalyzer< int8_t, uint32_t >;
template class gkg::SiteMapAnalyzer< uint8_t, uint32_t >;
template class gkg::SiteMapAnalyzer< int16_t, uint32_t >;
template class gkg::SiteMapAnalyzer< uint16_t, uint32_t >;
template class gkg::SiteMapAnalyzer< int32_t, uint32_t >;
template class gkg::SiteMapAnalyzer< uint32_t, uint32_t >;
template class gkg::SiteMapAnalyzer< int64_t, uint32_t >;
template class gkg::SiteMapAnalyzer< uint64_t, uint32_t >;
template class gkg::SiteMapAnalyzer< float, uint32_t >;
template class gkg::SiteMapAnalyzer< double, uint32_t >;

template class gkg::SiteMapAnalyzer< int8_t, uint64_t >;
template class gkg::SiteMapAnalyzer< uint8_t, uint64_t >;
template class gkg::SiteMapAnalyzer< int16_t, uint64_t >;
template class gkg::SiteMapAnalyzer< uint16_t, uint64_t >;
template class gkg::SiteMapAnalyzer< int32_t, uint64_t >;
template class gkg::SiteMapAnalyzer< uint32_t, uint64_t >;
template class gkg::SiteMapAnalyzer< int64_t, uint64_t >;
template class gkg::SiteMapAnalyzer< uint64_t, uint64_t >;
template class gkg::SiteMapAnalyzer< float, uint64_t >;
template class gkg::SiteMapAnalyzer< double, uint64_t >;

template class gkg::SiteMapAnalyzer< int8_t, float >;
template class gkg::SiteMapAnalyzer< uint8_t, float >;
template class gkg::SiteMapAnalyzer< int16_t, float >;
template class gkg::SiteMapAnalyzer< uint16_t, float >;
template class gkg::SiteMapAnalyzer< int32_t, float >;
template class gkg::SiteMapAnalyzer< uint32_t, float >;
template class gkg::SiteMapAnalyzer< int64_t, float >;
template class gkg::SiteMapAnalyzer< uint64_t, float >;
template class gkg::SiteMapAnalyzer< float, float >;
template class gkg::SiteMapAnalyzer< double, float >;

template class gkg::SiteMapAnalyzer< int8_t, double >;
template class gkg::SiteMapAnalyzer< uint8_t, double >;
template class gkg::SiteMapAnalyzer< int16_t, double >;
template class gkg::SiteMapAnalyzer< uint16_t, double >;
template class gkg::SiteMapAnalyzer< int32_t, double >;
template class gkg::SiteMapAnalyzer< uint32_t, double >;
template class gkg::SiteMapAnalyzer< int64_t, double >;
template class gkg::SiteMapAnalyzer< uint64_t, double >;
template class gkg::SiteMapAnalyzer< float, double >;
template class gkg::SiteMapAnalyzer< double, double >;


//
// registrating standard SiteMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, int16_t >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, int32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, int32_t >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, int64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, int16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, int16_t >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, uint16_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, uint16_t >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, uint32_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, uint32_t >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, uint64_t >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, uint64_t >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, float >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, float >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int8_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int8_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint8_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint8_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int16_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int16_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint16_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint16_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int32_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int32_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint32_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint32_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< int64_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< int64_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< uint64_t, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< uint64_t, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< float, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< float, double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SiteMapDiskFormat< double, double >::getInstance().getName(),
         &gkg::SiteMapAnalyzer< double, double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Site Map analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
