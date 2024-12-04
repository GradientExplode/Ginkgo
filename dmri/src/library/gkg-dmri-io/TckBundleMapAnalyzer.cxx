#include <gkg-dmri-io/TckBundleMapAnalyzer.h>
#include <gkg-dmri-io/TckBundleMapDiskFormat.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::TckBundleMapAnalyzer::TckBundleMapAnalyzer()
                          : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::TckBundleMapAnalyzer::~TckBundleMapAnalyzer()
{
}


gkg::DiskFormat& gkg::TckBundleMapAnalyzer::getDiskFormat() const
{

  return gkg::TckBundleMapDiskFormat::getInstance();

}


void gkg::TckBundleMapAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::BundleMap< std::string > object;
    getDiskFormat().readHeader( name, object );

    std::string mrtrix_version;
    object.getHeader().getAttribute( "mrtrix_version", mrtrix_version ); 
    analyzedObject.getHeader().addAttribute( "object_type",
                                             std::string( "BundleMap" ) );

    analyzedObject.getHeader().addAttribute(
                                        "label_type",
                                        gkg::TypeOf< std::string >::getName() );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::TckBundleMapAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}



//
// registrating standard BundleMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::TckBundleMapDiskFormat::getInstance().getName(),
         &gkg::TckBundleMapAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "MRTrix Tck BundleMap analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
