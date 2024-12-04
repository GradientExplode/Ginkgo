#include <gkg-mri-reconstruction-io/GehcSignaGenesis2Analyzer.h>
#include <gkg-mri-reconstruction-io/GehcSignaGenesis2DiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::GehcSignaGenesis2Analyzer< T >::GehcSignaGenesis2Analyzer()
                                   : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::GehcSignaGenesis2Analyzer< T >::~GehcSignaGenesis2Analyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::GehcSignaGenesis2Analyzer< T >::getDiskFormat() const
{

  return gkg::GehcSignaGenesis2DiskFormat< T >::getInstance();

}


template < class T >
void gkg::GehcSignaGenesis2Analyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::Volume< T > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );
    analyzedObject.getHeader().addAttribute( "item_type", itemType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::GehcSignaGenesis2Analyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::GehcSignaGenesis2Analyzer< uint8_t >;
template class gkg::GehcSignaGenesis2Analyzer< int16_t >;


//
// registrating standard GEHC Signa Genesis 2 analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GehcSignaGenesis2DiskFormat< uint8_t >::getInstance().getName(),
         &gkg::GehcSignaGenesis2Analyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GehcSignaGenesis2DiskFormat< int16_t >::getInstance().getName(),
         &gkg::GehcSignaGenesis2Analyzer< int16_t >::getInstance() );

    return true;

  }
  GKG_CATCH( "GEHC Signa Genesis 2 analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
