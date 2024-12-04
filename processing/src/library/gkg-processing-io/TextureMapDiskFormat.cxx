#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>
#include <list>

//
// TextureMap disk format singleton instanciation
//

template class gkg::TextureMapDiskFormat< int8_t >;
template class gkg::TextureMapDiskFormat< uint8_t >;
template class gkg::TextureMapDiskFormat< int16_t >;
template class gkg::TextureMapDiskFormat< uint16_t >;
template class gkg::TextureMapDiskFormat< int32_t >;
template class gkg::TextureMapDiskFormat< uint32_t >;
template class gkg::TextureMapDiskFormat< int64_t >;
template class gkg::TextureMapDiskFormat< uint64_t >;
template class gkg::TextureMapDiskFormat< float >;
template class gkg::TextureMapDiskFormat< double >;
template class gkg::TextureMapDiskFormat< bool >;
template class gkg::TextureMapDiskFormat< std::complex< float > >;
template class gkg::TextureMapDiskFormat< std::complex< double > >;
template class gkg::TextureMapDiskFormat< gkg::RGBComponent >;
template class gkg::TextureMapDiskFormat< gkg::Vector3d< float > >;
template class gkg::TextureMapDiskFormat< gkg::Vector3d< double > >;
template class gkg::TextureMapDiskFormat< std::list< int8_t > >;
template class gkg::TextureMapDiskFormat< std::list< uint8_t > >;
template class gkg::TextureMapDiskFormat< std::list< int16_t > >;
template class gkg::TextureMapDiskFormat< std::list< uint16_t > >;
template class gkg::TextureMapDiskFormat< std::list< int32_t > >;
template class gkg::TextureMapDiskFormat< std::list< uint32_t > >;
template class gkg::TextureMapDiskFormat< std::list< int64_t > >;
template class gkg::TextureMapDiskFormat< std::list< uint64_t > >;
template class gkg::TextureMapDiskFormat< std::list< float > >;
template class gkg::TextureMapDiskFormat< std::list< double > >;


//
// registrating TextureMap disk format(s) for "TextureMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::TextureMap< int8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< int8_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< int16_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< int32_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< int64_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< float > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< float >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< double > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< double >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< double >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< bool > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< bool >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< bool >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::complex< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::complex< float >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::complex< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::complex< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::complex< double >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::complex< double > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< gkg::RGBComponent > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< gkg::RGBComponent
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< gkg::RGBComponent >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< gkg::Vector3d< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< gkg::Vector3d< float >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< gkg::Vector3d< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< gkg::Vector3d< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< gkg::Vector3d< double>
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< gkg::Vector3d< double > >::getInstance() );

    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int8_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< int8_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< int8_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint8_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< uint8_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< uint8_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int16_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< int16_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< int16_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint16_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< uint16_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< uint16_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int32_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< int32_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< int32_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint32_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< uint32_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< uint32_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int64_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< int64_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< int64_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint64_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< uint64_t >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< uint64_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< float >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::TextureMapDiskFormat< std::list< double >
                                   >::getInstance().getName(),
         &gkg::TextureMapDiskFormat< std::list< double > >::getInstance() );

    return true;

  }
  GKG_CATCH( "TextureMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
