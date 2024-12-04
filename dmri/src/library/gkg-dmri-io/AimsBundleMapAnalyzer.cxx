#include <gkg-dmri-io/AimsBundleMapAnalyzer.h>
#include <gkg-dmri-io/AimsBundleMapDiskFormat.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>
#include <string>


gkg::AimsBundleMapAnalyzer::AimsBundleMapAnalyzer()
                           : gkg::BaseDiskFormatAnalyzer()
{
}


gkg::AimsBundleMapAnalyzer::~AimsBundleMapAnalyzer()
{
}


gkg::DiskFormat& gkg::AimsBundleMapAnalyzer::getDiskFormat() const
{

  return gkg::AimsBundleMapDiskFormat::getInstance();

}


void gkg::AimsBundleMapAnalyzer::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::BundleMap< std::string > object;
    getDiskFormat().readHeader( name, object );

    if ( object.getHeader().hasAttribute( "object_type" ) )
    {

      std::string objectType;
      object.getHeader().getAttribute( "object_type", objectType );

      if ( objectType != "BundleMap" )
      {

        throw std::runtime_error( "wrong object type" );

      }

    }
    analyzedObject.getHeader().addAttribute( "object_type", 
                                             std::string( "BundleMap" ) );

    if ( object.getHeader().hasAttribute( "label_type" ) )
    {

      std::string labelType;
      object.getHeader().getAttribute( "label_type", labelType );

      if ( !gkg::TypeOf< std::string >::isSameType( labelType ) )
      {

        throw std::runtime_error( "wrong label type" );

      }
      analyzedObject.getHeader().addAttribute( "label_type", labelType );

    }

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::AimsBundleMapAnalyzer::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}




//
// registrating Aims BundleMap analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::AimsBundleMapDiskFormat::getInstance().getName(),
         &gkg::AimsBundleMapAnalyzer::getInstance() );

    return true;

  }
  GKG_CATCH( "Aims BundleMap analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
