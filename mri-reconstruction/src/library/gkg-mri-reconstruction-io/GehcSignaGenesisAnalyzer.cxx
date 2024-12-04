#include <gkg-mri-reconstruction-io/GehcSignaGenesisAnalyzer.h>
#include <gkg-mri-reconstruction-io/GehcSignaGenesisDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::GehcSignaGenesisAnalyzer< T >::GehcSignaGenesisAnalyzer()
                                   : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::GehcSignaGenesisAnalyzer< T >::~GehcSignaGenesisAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::GehcSignaGenesisAnalyzer< T >::getDiskFormat() const
{

  return gkg::GehcSignaGenesisDiskFormat< T >::getInstance();

}


template < class T >
void gkg::GehcSignaGenesisAnalyzer< T >::analyze(
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
  GKG_CATCH( "void gkg::GehcSignaGenesisAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::GehcSignaGenesisAnalyzer< uint8_t >;
template class gkg::GehcSignaGenesisAnalyzer< int16_t >;


//
// registrating standard GEHC Signa Genesis analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GehcSignaGenesisDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::GehcSignaGenesisAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GehcSignaGenesisDiskFormat< int16_t >::getInstance().getName(),
         &gkg::GehcSignaGenesisAnalyzer< int16_t >::getInstance() );

    return true;

  }
  GKG_CATCH( "GEHC Signa Genesis analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
