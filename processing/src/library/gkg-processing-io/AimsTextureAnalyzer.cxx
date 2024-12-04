#include <gkg-processing-io/AimsTextureAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>
#include <list>


template class gkg::AimsTextureAnalyzer< int8_t >;
template class gkg::AimsTextureAnalyzer< uint8_t >;
template class gkg::AimsTextureAnalyzer< int16_t >;
template class gkg::AimsTextureAnalyzer< uint16_t >;
template class gkg::AimsTextureAnalyzer< int32_t >;
template class gkg::AimsTextureAnalyzer< uint32_t >;
template class gkg::AimsTextureAnalyzer< int64_t >;
template class gkg::AimsTextureAnalyzer< uint64_t >;
template class gkg::AimsTextureAnalyzer< float >;
template class gkg::AimsTextureAnalyzer< double >;
template class gkg::AimsTextureAnalyzer< bool >;
template class gkg::AimsTextureAnalyzer< std::complex< float > >;
template class gkg::AimsTextureAnalyzer< std::complex< double > >;
template class gkg::AimsTextureAnalyzer< gkg::RGBComponent >;
template class gkg::AimsTextureAnalyzer< gkg::Vector3d< float > >;
template class gkg::AimsTextureAnalyzer< gkg::Vector3d< double > >;
template class gkg::AimsTextureAnalyzer< std::list< int8_t > >;
template class gkg::AimsTextureAnalyzer< std::list< uint8_t > >;
template class gkg::AimsTextureAnalyzer< std::list< int16_t > >;
template class gkg::AimsTextureAnalyzer< std::list< uint16_t > >;
template class gkg::AimsTextureAnalyzer< std::list< int32_t > >;
template class gkg::AimsTextureAnalyzer< std::list< uint32_t > >;
template class gkg::AimsTextureAnalyzer< std::list< int64_t > >;
template class gkg::AimsTextureAnalyzer< std::list< uint64_t > >;
template class gkg::AimsTextureAnalyzer< std::list< float > >;
template class gkg::AimsTextureAnalyzer< std::list< double > >;


//
// registrating standard AimsTexture analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< int8_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< int8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< int16_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< int32_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< int64_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< float >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< double >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< bool >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< bool >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::complex< float >
                                   >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::complex< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::complex< double >
                                   >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::complex< double > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< gkg::RGBComponent
                                   >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< gkg::RGBComponent >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< gkg::Vector3d< float >
                                   >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< gkg::Vector3d< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< gkg::Vector3d< double>
                                   >::getInstance().getName(),
         &gkg::AimsTextureAnalyzer< gkg::Vector3d< double > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< int8_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< int8_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< uint8_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< uint8_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< int16_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< int16_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< uint16_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< uint16_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< int32_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< int32_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< uint32_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< uint32_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< int64_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< int64_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< uint64_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< uint64_t > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< float > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsTextureDiskFormat< std::list< double > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureAnalyzer< std::list< double > >::getInstance() );

    return true;

  }
  GKG_CATCH( "AimsTexture analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
