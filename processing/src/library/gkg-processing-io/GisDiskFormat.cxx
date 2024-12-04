#include <gkg-processing-io/GisDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>


//
// GIS disk format singleton instanciation
//

template class gkg::GisDiskFormat< int8_t >;
template class gkg::GisDiskFormat< uint8_t >;
template class gkg::GisDiskFormat< int16_t >;
template class gkg::GisDiskFormat< uint16_t >;
template class gkg::GisDiskFormat< int32_t >;
template class gkg::GisDiskFormat< uint32_t >;
template class gkg::GisDiskFormat< int64_t >;
template class gkg::GisDiskFormat< uint64_t >;
template class gkg::GisDiskFormat< float >;
template class gkg::GisDiskFormat< double >;
template class gkg::GisDiskFormat< bool >;
template class gkg::GisDiskFormat< std::complex< float > >;
template class gkg::GisDiskFormat< std::complex< double > >;
template class gkg::GisDiskFormat< gkg::RGBComponent >;
template class gkg::GisDiskFormat< gkg::Vector3d< float > >;
template class gkg::GisDiskFormat< gkg::Vector3d< double > >;


//
// registrating standard GIS disk format(s) for "Volume" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Volume< int8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< int8_t >::getInstance().getName(),
         &gkg::GisDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::GisDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< int16_t >::getInstance().getName(),
         &gkg::GisDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::GisDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< int32_t >::getInstance().getName(),
         &gkg::GisDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::GisDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< int64_t >::getInstance().getName(),
         &gkg::GisDiskFormat< int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::GisDiskFormat< uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< float > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< float >::getInstance().getName(),
         &gkg::GisDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< double > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< double >::getInstance().getName(),
         &gkg::GisDiskFormat< double >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< bool > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< bool >::getInstance().getName(),
         &gkg::GisDiskFormat< bool >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< std::complex< float > >::getInstance().getName(),
         &gkg::GisDiskFormat< std::complex< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< std::complex< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< std::complex< double > >::getInstance().getName(),
         &gkg::GisDiskFormat< std::complex< double > >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< gkg::RGBComponent > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< gkg::RGBComponent >::getInstance().getName(),
         &gkg::GisDiskFormat< gkg::RGBComponent >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< gkg::Vector3d< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< gkg::Vector3d< float > >::getInstance().getName(),
         &gkg::GisDiskFormat< gkg::Vector3d< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::Volume< gkg::Vector3d< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::GisDiskFormat< gkg::Vector3d< double> >::getInstance().getName(),
         &gkg::GisDiskFormat< gkg::Vector3d< double > >::getInstance() );

    return true;

  }
  GKG_CATCH( "GIS disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
