#include <gkg-dmri-io/BundleMapDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-cppext/StdInt.h>
#include <string>

//
// BundleMap disk format singleton instanciation
//

template class gkg::BundleMapDiskFormat< int8_t >;
template class gkg::BundleMapDiskFormat< uint8_t >;
template class gkg::BundleMapDiskFormat< int16_t >;
template class gkg::BundleMapDiskFormat< uint16_t >;
template class gkg::BundleMapDiskFormat< int32_t >;
template class gkg::BundleMapDiskFormat< uint32_t >;
template class gkg::BundleMapDiskFormat< int64_t >;
template class gkg::BundleMapDiskFormat< uint64_t >;
template class gkg::BundleMapDiskFormat< std::string >;


//
// registrating BundleMap disk format(s) for "BundleMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::BundleMap< int8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< int8_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< int16_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< int32_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< int64_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< std::string > 
                         >::getInstance().registerDiskFormat(
         gkg::BundleMapDiskFormat< std::string >::getInstance().getName(),
         &gkg::BundleMapDiskFormat< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "BundleMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
