#include <gkg-processing-io/AimsMeshAnalyzer.h>
#include <gkg-processing-io/AimsMeshDiskFormat.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>


template < uint32_t D >
gkg::AimsMeshAnalyzer< D >::AimsMeshAnalyzer()
                          : gkg::BaseDiskFormatAnalyzer()
{
}


template < uint32_t D >
gkg::AimsMeshAnalyzer< D >::~AimsMeshAnalyzer()
{
}


template < uint32_t D >
gkg::DiskFormat& gkg::AimsMeshAnalyzer< D >::getDiskFormat() const
{

  return gkg::AimsMeshDiskFormat< D >::getInstance();

}


template < uint32_t D >
void gkg::AimsMeshAnalyzer< D >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::MeshMap< int32_t, float, D > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    analyzedObject.getHeader().addAttribute( "item_type",
                                             gkg::TypeOf< float >::getName() );
    analyzedObject.getHeader().addAttribute(
                                            "rank_type", 
                                            gkg::TypeOf< int32_t >::getName() );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::AimsMeshAnalyzer< D >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::AimsMeshAnalyzer< 2U >;
template class gkg::AimsMeshAnalyzer< 3U >;
template class gkg::AimsMeshAnalyzer< 4U >;


//
// registrating standard AimsMesh analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsMeshDiskFormat< 2U >::getInstance().getName(),
         &gkg::AimsMeshAnalyzer< 2U >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsMeshDiskFormat< 3U >::getInstance().getName(),
         &gkg::AimsMeshAnalyzer< 3U >::getInstance() );

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsMeshDiskFormat< 4U >::getInstance().getName(),
         &gkg::AimsMeshAnalyzer< 4U >::getInstance() );

    return true;

  }
  GKG_CATCH( "AimsMesh analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
