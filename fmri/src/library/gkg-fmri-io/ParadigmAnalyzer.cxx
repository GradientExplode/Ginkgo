#include <gkg-fmri-io/ParadigmAnalyzer.h>
#include <gkg-fmri-io/ParadigmDiskFormat.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-fmri-paradigm/Paradigm.h>
#include <gkg-core-exception/Exception.h>


gkg::ParadigmAnalyzer::ParadigmAnalyzer()
                      : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::ParadigmAnalyzer::~ParadigmAnalyzer()
{
}


gkg::DiskFormat& gkg::ParadigmAnalyzer::getDiskFormat() const
{

  return gkg::ParadigmDiskFormat::getInstance();

}


void gkg::ParadigmAnalyzer::analyze( const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::Paradigm object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::ParadigmAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


//
// registrating standard Paradigm analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::ParadigmDiskFormat::getInstance().getName(),
         &gkg::ParadigmAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "Paradigm analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
