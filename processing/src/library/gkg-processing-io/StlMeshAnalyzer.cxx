#include <gkg-processing-io/StlMeshAnalyzer.h>
#include <gkg-processing-io/StlMeshDiskFormat.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>


gkg::StlMeshAnalyzer::StlMeshAnalyzer()
                     : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::StlMeshAnalyzer::~StlMeshAnalyzer()
{
}


gkg::DiskFormat& gkg::StlMeshAnalyzer::getDiskFormat() const
{

  return gkg::StlMeshDiskFormat::getInstance();

}


void gkg::StlMeshAnalyzer::analyze( const std::string& name,
                                    std::string& format,
                                    gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::MeshMap< int32_t, float, 3U > object;
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
  GKG_CATCH( "void gkg::StlMeshAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


//
// registrating standard StlMesh analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::StlMeshDiskFormat::getInstance().getName(),
         &gkg::StlMeshAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "StlMesh analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
