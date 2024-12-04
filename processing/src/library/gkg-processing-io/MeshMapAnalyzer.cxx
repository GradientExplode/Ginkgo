#include <gkg-processing-io/MeshMapAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>


template class gkg::MeshMapAnalyzer< int8_t, float, 2U >;
template class gkg::MeshMapAnalyzer< uint8_t, float, 2U >;
template class gkg::MeshMapAnalyzer< int16_t, float, 2U >;
template class gkg::MeshMapAnalyzer< uint16_t, float, 2U >;
template class gkg::MeshMapAnalyzer< int32_t, float, 2U >;
template class gkg::MeshMapAnalyzer< uint32_t, float, 2U >;
template class gkg::MeshMapAnalyzer< int64_t, float, 2U >;
template class gkg::MeshMapAnalyzer< uint64_t, float, 2U >;
template class gkg::MeshMapAnalyzer< float, float, 2U >;
template class gkg::MeshMapAnalyzer< double, float, 2U >;

template class gkg::MeshMapAnalyzer< int8_t, float, 3U >;
template class gkg::MeshMapAnalyzer< uint8_t, float, 3U >;
template class gkg::MeshMapAnalyzer< int16_t, float, 3U >;
template class gkg::MeshMapAnalyzer< uint16_t, float, 3U >;
template class gkg::MeshMapAnalyzer< int32_t, float, 3U >;
template class gkg::MeshMapAnalyzer< uint32_t, float, 3U >;
template class gkg::MeshMapAnalyzer< int64_t, float, 3U >;
template class gkg::MeshMapAnalyzer< uint64_t, float, 3U >;
template class gkg::MeshMapAnalyzer< float, float, 3U >;
template class gkg::MeshMapAnalyzer< double, float, 3U >;

template class gkg::MeshMapAnalyzer< int8_t, float, 4U >;
template class gkg::MeshMapAnalyzer< uint8_t, float, 4U >;
template class gkg::MeshMapAnalyzer< int16_t, float, 4U >;
template class gkg::MeshMapAnalyzer< uint16_t, float, 4U >;
template class gkg::MeshMapAnalyzer< int32_t, float, 4U >;
template class gkg::MeshMapAnalyzer< uint32_t, float, 4U >;
template class gkg::MeshMapAnalyzer< int64_t, float, 4U >;
template class gkg::MeshMapAnalyzer< uint64_t, float, 4U >;
template class gkg::MeshMapAnalyzer< float, float, 4U >;
template class gkg::MeshMapAnalyzer< double, float, 4U >;


//
// registrating standard MeshMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int8_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int8_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint8_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint8_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int16_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int16_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint16_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint16_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int32_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int32_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint32_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint32_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int64_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int64_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint64_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint64_t, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< float, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< float, float, 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< double, float, 2U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< double, float, 2U >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int8_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int8_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint8_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint8_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int16_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int16_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint16_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint16_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int32_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int32_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint32_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint32_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int64_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int64_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint64_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint64_t, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< float, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< float, float, 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< double, float, 3U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< double, float, 3U >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int8_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int8_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint8_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint8_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int16_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int16_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint16_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint16_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int32_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int32_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint32_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint32_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< int64_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< int64_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< uint64_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< uint64_t, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< float, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< float, float, 4U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MeshMapDiskFormat< double, float, 4U >::getInstance().getName(),
         &gkg::MeshMapAnalyzer< double, float, 4U >::getInstance() );

    return true;

  }
  GKG_CATCH( "MeshMap analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
