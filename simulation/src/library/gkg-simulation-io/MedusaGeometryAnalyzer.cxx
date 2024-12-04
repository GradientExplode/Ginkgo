#include <gkg-simulation-io/MedusaGeometryAnalyzer.h>
#include <gkg-simulation-io/MedusaGeometryDiskFormat.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-core-exception/Exception.h>


gkg::MedusaGeometryAnalyzer::MedusaGeometryAnalyzer()
                            : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::MedusaGeometryAnalyzer::~MedusaGeometryAnalyzer()
{
}


gkg::DiskFormat& gkg::MedusaGeometryAnalyzer::getDiskFormat() const
{

  return gkg::MedusaGeometryDiskFormat::getInstance();

}


void gkg::MedusaGeometryAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::VirtualTissue object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::MedusaGeometryAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


//
// registrating standard Medusa Geometry analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::MedusaGeometryDiskFormat::getInstance().getName(),
         &gkg::MedusaGeometryAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "Medusa Geometry analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
