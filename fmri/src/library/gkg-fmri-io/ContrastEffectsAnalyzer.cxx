#include <gkg-fmri-io/ContrastEffectsAnalyzer.h>
#include <gkg-fmri-io/ContrastEffectsDiskFormat.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-fmri-contrast/ContrastEffects.h>
#include <gkg-core-exception/Exception.h>


gkg::ContrastEffectsAnalyzer::ContrastEffectsAnalyzer()
                             : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::ContrastEffectsAnalyzer::~ContrastEffectsAnalyzer()
{
}


gkg::DiskFormat& gkg::ContrastEffectsAnalyzer::getDiskFormat() const
{

  return gkg::ContrastEffectsDiskFormat::getInstance();

}


void gkg::ContrastEffectsAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::ContrastEffects object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::ContrastEffectsAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


//
// registrating standard ContrastEffects analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::ContrastEffectsDiskFormat::getInstance().getName(),
         &gkg::ContrastEffectsAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "ContrastEffects analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
