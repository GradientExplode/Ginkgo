#include <gkg-processing-io/GisAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>


template class gkg::GisAnalyzer< int8_t >;
template class gkg::GisAnalyzer< uint8_t >;
template class gkg::GisAnalyzer< int16_t >;
template class gkg::GisAnalyzer< uint16_t >;
template class gkg::GisAnalyzer< int32_t >;
template class gkg::GisAnalyzer< uint32_t >;
template class gkg::GisAnalyzer< int64_t >;
template class gkg::GisAnalyzer< uint64_t >;
template class gkg::GisAnalyzer< float >;
template class gkg::GisAnalyzer< double >;
template class gkg::GisAnalyzer< bool >;
template class gkg::GisAnalyzer< std::complex< float > >;
template class gkg::GisAnalyzer< std::complex< double > >;
template class gkg::GisAnalyzer< gkg::RGBComponent >;
template class gkg::GisAnalyzer< gkg::Vector3d< float > >;
template class gkg::GisAnalyzer< gkg::Vector3d< double > >;


//
// registrating standard GIS analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< int8_t >::getInstance().getName(),
         &gkg::GisAnalyzer< int8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::GisAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< int16_t >::getInstance().getName(),
         &gkg::GisAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::GisAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< int32_t >::getInstance().getName(),
         &gkg::GisAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::GisAnalyzer< uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< int64_t >::getInstance().getName(),
         &gkg::GisAnalyzer< int64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::GisAnalyzer< uint64_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< float >::getInstance().getName(),
         &gkg::GisAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< double >::getInstance().getName(),
         &gkg::GisAnalyzer< double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< bool >::getInstance().getName(),
         &gkg::GisAnalyzer< bool >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< std::complex< float > >::getInstance().getName(),
         &gkg::GisAnalyzer< std::complex< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< std::complex< double > >::getInstance().getName(),
         &gkg::GisAnalyzer< std::complex< double > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< gkg::RGBComponent >::getInstance().getName(),
         &gkg::GisAnalyzer< gkg::RGBComponent >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< gkg::Vector3d< float > >::getInstance().getName(),
         &gkg::GisAnalyzer< gkg::Vector3d< float > >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GisDiskFormat< gkg::Vector3d< double> >::getInstance().getName(),
         &gkg::GisAnalyzer< gkg::Vector3d< double > >::getInstance() );

    return true;

  }
  GKG_CATCH( "GIS analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
