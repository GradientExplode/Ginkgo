#include <gkg-processing-io/AimsTextureDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <complex>
#include <list>


//
// AimsTexture disk format singleton instanciation
//

template class gkg::AimsTextureDiskFormat< int8_t >;
template class gkg::AimsTextureDiskFormat< uint8_t >;
template class gkg::AimsTextureDiskFormat< int16_t >;
template class gkg::AimsTextureDiskFormat< uint16_t >;
template class gkg::AimsTextureDiskFormat< int32_t >;
template class gkg::AimsTextureDiskFormat< uint32_t >;
template class gkg::AimsTextureDiskFormat< int64_t >;
template class gkg::AimsTextureDiskFormat< uint64_t >;
template class gkg::AimsTextureDiskFormat< float >;
template class gkg::AimsTextureDiskFormat< double >;
template class gkg::AimsTextureDiskFormat< bool >;
template class gkg::AimsTextureDiskFormat< std::complex< float > >;
template class gkg::AimsTextureDiskFormat< std::complex< double > >;
template class gkg::AimsTextureDiskFormat< gkg::RGBComponent >;
template class gkg::AimsTextureDiskFormat< gkg::Vector3d< float > >;
template class gkg::AimsTextureDiskFormat< gkg::Vector3d< double > >;
template class gkg::AimsTextureDiskFormat< std::list< int8_t > >;
template class gkg::AimsTextureDiskFormat< std::list< uint8_t > >;
template class gkg::AimsTextureDiskFormat< std::list< int16_t > >;
template class gkg::AimsTextureDiskFormat< std::list< uint16_t > >;
template class gkg::AimsTextureDiskFormat< std::list< int32_t > >;
template class gkg::AimsTextureDiskFormat< std::list< uint32_t > >;
template class gkg::AimsTextureDiskFormat< std::list< int64_t > >;
template class gkg::AimsTextureDiskFormat< std::list< uint64_t > >;
template class gkg::AimsTextureDiskFormat< std::list< float > >;
template class gkg::AimsTextureDiskFormat< std::list< double > >;


//
// registrating AimsTexture disk format(s) for "TextureMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::TextureMap< int8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< int8_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< int16_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< int32_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< int64_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< float > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< float >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< double > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< double >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< double >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< bool > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< bool >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< bool >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::complex< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::complex< float >
                                   >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::complex< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::complex< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::complex< double >
                                   >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::complex< double > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< gkg::RGBComponent > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< gkg::RGBComponent
                                   >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< gkg::RGBComponent >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< gkg::Vector3d< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< gkg::Vector3d< float >
                                   >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< gkg::Vector3d< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< gkg::Vector3d< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< gkg::Vector3d< double>
                                   >::getInstance().getName(),
         &gkg::AimsTextureDiskFormat< gkg::Vector3d< double > >::
                                                                getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int8_t > >
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< int8_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< int8_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint8_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< uint8_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< uint8_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int16_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< int16_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< int16_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint16_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< uint16_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< uint16_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int32_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< int32_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< int32_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint32_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< uint32_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< uint32_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< int64_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< int64_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< int64_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< uint64_t > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< uint64_t > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< uint64_t > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< float > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< float > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< float > >::getInstance() );
    gkg::DiskFormatFactory< gkg::TextureMap< std::list< double > > 
                         >::getInstance().registerDiskFormat(
         gkg::AimsTextureDiskFormat< std::list< double > >::
                                                        getInstance().getName(),
         &gkg::AimsTextureDiskFormat< std::list< double > >::getInstance() );

    return true;

  }
  GKG_CATCH( "AimsTexture disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
