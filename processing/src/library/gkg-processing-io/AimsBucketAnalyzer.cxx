#include <gkg-processing-io/AimsBucketAnalyzer.h>
#include <gkg-processing-io/AimsBucketDiskFormat.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


gkg::AimsBucketAnalyzer::AimsBucketAnalyzer()
                        : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::AimsBucketAnalyzer::~AimsBucketAnalyzer()
{
}


gkg::DiskFormat& gkg::AimsBucketAnalyzer::getDiskFormat() const
{

  return gkg::AimsBucketDiskFormat::getInstance();

}


void gkg::AimsBucketAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::SiteMap< int32_t, int32_t, std::less< int32_t > > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string dataType;
    object.getHeader().getAttribute( "data_type", dataType );

    if ( dataType != "VOID" )
    {

      throw std::runtime_error( "not a VOID bucket" ); 

    }

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::AimsBucketAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}



//
// registrating standard SiteMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsBucketDiskFormat::getInstance().getName(),
         &gkg::AimsBucketAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "Site Map analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
