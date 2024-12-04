#include <gkg-processing-io/SiteMapDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>


//
// SiteMap disk format singleton instanciation
//

template class gkg::SiteMapDiskFormat< int8_t, int16_t >;
template class gkg::SiteMapDiskFormat< uint8_t, int16_t >;
template class gkg::SiteMapDiskFormat< int16_t, int16_t >;
template class gkg::SiteMapDiskFormat< uint16_t, int16_t >;
template class gkg::SiteMapDiskFormat< int32_t, int16_t >;
template class gkg::SiteMapDiskFormat< uint32_t, int16_t >;
template class gkg::SiteMapDiskFormat< int64_t, int16_t >;
template class gkg::SiteMapDiskFormat< uint64_t, int16_t >;
template class gkg::SiteMapDiskFormat< float, int16_t >;
template class gkg::SiteMapDiskFormat< double, int16_t >;

template class gkg::SiteMapDiskFormat< int8_t, int32_t >;
template class gkg::SiteMapDiskFormat< uint8_t, int32_t >;
template class gkg::SiteMapDiskFormat< int16_t, int32_t >;
template class gkg::SiteMapDiskFormat< uint16_t, int32_t >;
template class gkg::SiteMapDiskFormat< int32_t, int32_t >;
template class gkg::SiteMapDiskFormat< uint32_t, int32_t >;
template class gkg::SiteMapDiskFormat< int64_t, int32_t >;
template class gkg::SiteMapDiskFormat< uint64_t, int32_t >;
template class gkg::SiteMapDiskFormat< float, int32_t >;
template class gkg::SiteMapDiskFormat< double, int32_t >;

template class gkg::SiteMapDiskFormat< int8_t, int64_t >;
template class gkg::SiteMapDiskFormat< uint8_t, int64_t >;
template class gkg::SiteMapDiskFormat< int16_t, int64_t >;
template class gkg::SiteMapDiskFormat< uint16_t, int64_t >;
template class gkg::SiteMapDiskFormat< int32_t, int64_t >;
template class gkg::SiteMapDiskFormat< uint32_t, int64_t >;
template class gkg::SiteMapDiskFormat< int64_t, int64_t >;
template class gkg::SiteMapDiskFormat< uint64_t, int64_t >;
template class gkg::SiteMapDiskFormat< float, int64_t >;
template class gkg::SiteMapDiskFormat< double, int64_t >;

template class gkg::SiteMapDiskFormat< int8_t, uint16_t >;
template class gkg::SiteMapDiskFormat< uint8_t, uint16_t >;
template class gkg::SiteMapDiskFormat< int16_t, uint16_t >;
template class gkg::SiteMapDiskFormat< uint16_t, uint16_t >;
template class gkg::SiteMapDiskFormat< int32_t, uint16_t >;
template class gkg::SiteMapDiskFormat< uint32_t, uint16_t >;
template class gkg::SiteMapDiskFormat< int64_t, uint16_t >;
template class gkg::SiteMapDiskFormat< uint64_t, uint16_t >;
template class gkg::SiteMapDiskFormat< float, uint16_t >;
template class gkg::SiteMapDiskFormat< double, uint16_t >;

template class gkg::SiteMapDiskFormat< int8_t, uint32_t >;
template class gkg::SiteMapDiskFormat< uint8_t, uint32_t >;
template class gkg::SiteMapDiskFormat< int16_t, uint32_t >;
template class gkg::SiteMapDiskFormat< uint16_t, uint32_t >;
template class gkg::SiteMapDiskFormat< int32_t, uint32_t >;
template class gkg::SiteMapDiskFormat< uint32_t, uint32_t >;
template class gkg::SiteMapDiskFormat< int64_t, uint32_t >;
template class gkg::SiteMapDiskFormat< uint64_t, uint32_t >;
template class gkg::SiteMapDiskFormat< float, uint32_t >;
template class gkg::SiteMapDiskFormat< double, uint32_t >;

template class gkg::SiteMapDiskFormat< int8_t, uint64_t >;
template class gkg::SiteMapDiskFormat< uint8_t, uint64_t >;
template class gkg::SiteMapDiskFormat< int16_t, uint64_t >;
template class gkg::SiteMapDiskFormat< uint16_t, uint64_t >;
template class gkg::SiteMapDiskFormat< int32_t, uint64_t >;
template class gkg::SiteMapDiskFormat< uint32_t, uint64_t >;
template class gkg::SiteMapDiskFormat< int64_t, uint64_t >;
template class gkg::SiteMapDiskFormat< uint64_t, uint64_t >;
template class gkg::SiteMapDiskFormat< float, uint64_t >;
template class gkg::SiteMapDiskFormat< double, uint64_t >;

template class gkg::SiteMapDiskFormat< int8_t, float >;
template class gkg::SiteMapDiskFormat< uint8_t, float >;
template class gkg::SiteMapDiskFormat< int16_t, float >;
template class gkg::SiteMapDiskFormat< uint16_t, float >;
template class gkg::SiteMapDiskFormat< int32_t, float >;
template class gkg::SiteMapDiskFormat< uint32_t, float >;
template class gkg::SiteMapDiskFormat< int64_t, float >;
template class gkg::SiteMapDiskFormat< uint64_t, float >;
template class gkg::SiteMapDiskFormat< float, float >;
template class gkg::SiteMapDiskFormat< double, float >;

template class gkg::SiteMapDiskFormat< int8_t, double >;
template class gkg::SiteMapDiskFormat< uint8_t, double >;
template class gkg::SiteMapDiskFormat< int16_t, double >;
template class gkg::SiteMapDiskFormat< uint16_t, double >;
template class gkg::SiteMapDiskFormat< int32_t, double >;
template class gkg::SiteMapDiskFormat< uint32_t, double >;
template class gkg::SiteMapDiskFormat< int64_t, double >;
template class gkg::SiteMapDiskFormat< uint64_t, double >;
template class gkg::SiteMapDiskFormat< float, double >;
template class gkg::SiteMapDiskFormat< double, double >;


//
// registrating standard SiteMap disk format(s) for "SiteMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, int16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, int16_t >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, int32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, int32_t >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, int64_t> 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, int64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, int64_t >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, uint16_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, uint16_t >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, uint32_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, uint32_t >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, uint64_t> 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, uint64_t >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, uint64_t >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, float> 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, float >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, float > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, float >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, float >::getInstance() );

    gkg::DiskFormatFactory< gkg::SiteMap< int8_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int8_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int8_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint8_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint8_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint8_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int16_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int16_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int16_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint16_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint16_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint16_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int32_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int32_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int32_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint32_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint32_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint32_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< int64_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< int64_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< int64_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< uint64_t, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< uint64_t, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< uint64_t, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< float, double> 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< float, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< float, double >::getInstance() );
    gkg::DiskFormatFactory< gkg::SiteMap< double, double > 
                         >::getInstance().registerDiskFormat(
         gkg::SiteMapDiskFormat< double, double >::getInstance().getName(),
         &gkg::SiteMapDiskFormat< double, double >::getInstance() );

    return true;

  }
  GKG_CATCH( "SiteMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
