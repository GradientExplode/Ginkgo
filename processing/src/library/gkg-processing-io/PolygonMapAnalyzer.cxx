#include <gkg-processing-io/PolygonMapAnalyzer.h>
#include <gkg-processing-io/PolygonMapDiskFormat.h>
#include <gkg-processing-container/PolygonMap.h>
#include <gkg-core-exception/Exception.h>


template < uint32_t D >
gkg::PolygonMapAnalyzer< D >::PolygonMapAnalyzer()
                             : gkg::BaseDiskFormatAnalyzer()
{
}


template < uint32_t D >
gkg::PolygonMapAnalyzer< D >::~PolygonMapAnalyzer()
{
}


template < uint32_t D >
gkg::DiskFormat& gkg::PolygonMapAnalyzer< D >::getDiskFormat() const
{

  return gkg::PolygonMapDiskFormat< D >::getInstance();

}


template < uint32_t D >
void gkg::PolygonMapAnalyzer< D >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::PolygonMap< D > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    uint32_t polygonSize;
    object.getHeader().getAttribute( "polygon_size", polygonSize );
    analyzedObject.getHeader().addAttribute( "polygon_size", polygonSize );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMapAnalyzer< D >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::PolygonMapAnalyzer< 2U >;
template class gkg::PolygonMapAnalyzer< 3U >;
template class gkg::PolygonMapAnalyzer< 4U >;


//
// registrating standard PolygonMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::PolygonMapDiskFormat< 2U >::getInstance().getName(),
         &gkg::PolygonMapAnalyzer< 2U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::PolygonMapDiskFormat< 3U >::getInstance().getName(),
         &gkg::PolygonMapAnalyzer< 3U >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::PolygonMapDiskFormat< 4U >::getInstance().getName(),
         &gkg::PolygonMapAnalyzer< 4U >::getInstance() );

    return true;

  }
  GKG_CATCH( "Polygon Map analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
