#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>
#include <list>


template class gkg::TextureMapAnalyzer< int8_t >;
template class gkg::TextureMapAnalyzer< uint8_t >;
template class gkg::TextureMapAnalyzer< int16_t >;
template class gkg::TextureMapAnalyzer< uint16_t >;
template class gkg::TextureMapAnalyzer< int32_t >;
template class gkg::TextureMapAnalyzer< uint32_t >;
template class gkg::TextureMapAnalyzer< int64_t >;
template class gkg::TextureMapAnalyzer< uint64_t >;
template class gkg::TextureMapAnalyzer< float >;
template class gkg::TextureMapAnalyzer< double >;
template class gkg::TextureMapAnalyzer< bool >;
template class gkg::TextureMapAnalyzer< std::complex< float > >;
template class gkg::TextureMapAnalyzer< std::complex< double > >;
template class gkg::TextureMapAnalyzer< gkg::RGBComponent >;
template class gkg::TextureMapAnalyzer< gkg::Vector3d< float > >;
template class gkg::TextureMapAnalyzer< gkg::Vector3d< double > >;
template class gkg::TextureMapAnalyzer< std::list< int8_t > >;
template class gkg::TextureMapAnalyzer< std::list< uint8_t > >;
template class gkg::TextureMapAnalyzer< std::list< int16_t > >;
template class gkg::TextureMapAnalyzer< std::list< uint16_t > >;
template class gkg::TextureMapAnalyzer< std::list< int32_t > >;
template class gkg::TextureMapAnalyzer< std::list< uint32_t > >;
template class gkg::TextureMapAnalyzer< std::list< int64_t > >;
template class gkg::TextureMapAnalyzer< std::list< uint64_t > >;
template class gkg::TextureMapAnalyzer< std::list< float > >;
template class gkg::TextureMapAnalyzer< std::list< double > >;


//
// registrating standard TextureMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< int8_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< int8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< int16_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< int32_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< int64_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< float >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< double >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< bool >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< bool >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::complex< float >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::complex< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::complex< double >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::complex< double > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< gkg::RGBComponent
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< gkg::RGBComponent >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< gkg::Vector3d< float >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< gkg::Vector3d< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< gkg::Vector3d< double >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< gkg::Vector3d< double > >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< int8_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< int8_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< uint8_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< uint8_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< int16_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< int16_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< uint16_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< uint16_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< int32_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< int32_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< uint32_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< uint32_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< int64_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< int64_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< uint64_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< uint64_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< float >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TextureMapDiskFormat< std::list< double >
                                   >::getInstance().getName(),
         &gkg::TextureMapAnalyzer< std::list< double > >::getInstance() );

    return true;

  }
  GKG_CATCH( "TextureMap analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
