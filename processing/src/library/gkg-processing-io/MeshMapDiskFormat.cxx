#include <gkg-processing-io/MeshMapDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>


template class gkg::MeshMapDiskFormat< int8_t, float, 2U >;
template class gkg::MeshMapDiskFormat< uint8_t, float, 2U >;
template class gkg::MeshMapDiskFormat< int16_t, float, 2U >;
template class gkg::MeshMapDiskFormat< uint16_t, float, 2U >;
template class gkg::MeshMapDiskFormat< int32_t, float, 2U >;
template class gkg::MeshMapDiskFormat< uint32_t, float, 2U >;
template class gkg::MeshMapDiskFormat< int64_t, float, 2U >;
template class gkg::MeshMapDiskFormat< uint64_t, float, 2U >;
template class gkg::MeshMapDiskFormat< float, float, 2U >;
template class gkg::MeshMapDiskFormat< double, float, 2U >;

template class gkg::MeshMapDiskFormat< int8_t, float, 3U >;
template class gkg::MeshMapDiskFormat< uint8_t, float, 3U >;
template class gkg::MeshMapDiskFormat< int16_t, float, 3U >;
template class gkg::MeshMapDiskFormat< uint16_t, float, 3U >;
template class gkg::MeshMapDiskFormat< int32_t, float, 3U >;
template class gkg::MeshMapDiskFormat< uint32_t, float, 3U >;
template class gkg::MeshMapDiskFormat< int64_t, float, 3U >;
template class gkg::MeshMapDiskFormat< uint64_t, float, 3U >;
template class gkg::MeshMapDiskFormat< float, float, 3U >;
template class gkg::MeshMapDiskFormat< double, float, 3U >;

template class gkg::MeshMapDiskFormat< int8_t, float, 4U >;
template class gkg::MeshMapDiskFormat< uint8_t, float, 4U >;
template class gkg::MeshMapDiskFormat< int16_t, float, 4U >;
template class gkg::MeshMapDiskFormat< uint16_t, float, 4U >;
template class gkg::MeshMapDiskFormat< int32_t, float, 4U >;
template class gkg::MeshMapDiskFormat< uint32_t, float, 4U >;
template class gkg::MeshMapDiskFormat< int64_t, float, 4U >;
template class gkg::MeshMapDiskFormat< uint64_t, float, 4U >;
template class gkg::MeshMapDiskFormat< float, float, 4U >;
template class gkg::MeshMapDiskFormat< double, float, 4U >;


//
// registrating standard MeshMap disk format(s) for "MeshMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::MeshMap< int8_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int8_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int8_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint8_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint8_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint8_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int16_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int16_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int16_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint16_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint16_t, float, 2U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint16_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int32_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int32_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint32_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint32_t, float, 2U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint32_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int64_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int64_t, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int64_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint64_t, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint64_t, float, 2U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint64_t, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< float, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< float, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< float, float, 2U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< double, float, 2U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< double, float, 2U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< double, float, 2U >::getInstance() );

    gkg::DiskFormatFactory< gkg::MeshMap< int8_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int8_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int8_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint8_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint8_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint8_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int16_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int16_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int16_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint16_t, float, 3U >
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint16_t, float, 3U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint16_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int32_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int32_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint32_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint32_t, float, 3U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint32_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int64_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int64_t, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int64_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint64_t, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint64_t, float, 3U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint64_t, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< float, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< float, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< float, float, 3U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< double, float, 3U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< double, float, 3U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< double, float, 3U >::getInstance() );

    gkg::DiskFormatFactory< gkg::MeshMap< int8_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int8_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int8_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint8_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint8_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint8_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int16_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int16_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int16_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint16_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint16_t, float, 4U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint16_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int32_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int32_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int32_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint32_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint32_t, float, 4U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint32_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< int64_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< int64_t, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< int64_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< uint64_t, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< uint64_t, float, 4U>::getInstance().getName(),
         &gkg::MeshMapDiskFormat< uint64_t, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< float, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< float, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< float, float, 4U >::getInstance() );
    gkg::DiskFormatFactory< gkg::MeshMap< double, float, 4U > 
                         >::getInstance().registerDiskFormat(
         gkg::MeshMapDiskFormat< double, float, 4U >::getInstance().getName(),
         &gkg::MeshMapDiskFormat< double, float, 4U >::getInstance() );

    return true;

  }
  GKG_CATCH( "MeshMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
