#include <gkg-mri-reconstruction-io/GehcSignaPFileAnalyzer.h>
#include <gkg-mri-reconstruction-io/GehcSignaPFileDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::GehcSignaPFileAnalyzer< T >::GehcSignaPFileAnalyzer()
                                 : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::GehcSignaPFileAnalyzer< T >::~GehcSignaPFileAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::GehcSignaPFileAnalyzer< T >::getDiskFormat() const
{

  return gkg::GehcSignaPFileDiskFormat< T >::getInstance();

}


template < class T >
void gkg::GehcSignaPFileAnalyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::Volume< std::complex< T > > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );
    analyzedObject.getHeader().addAttribute( "item_type", itemType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::GehcSignaPFileAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::GehcSignaPFileAnalyzer< float >;
template class gkg::GehcSignaPFileAnalyzer< double >;


//
// registrating standard GEHC Signa PFile analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GehcSignaPFileDiskFormat< float >::getInstance().getName(),
         &gkg::GehcSignaPFileAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::GehcSignaPFileDiskFormat< double >::getInstance().getName(),
         &gkg::GehcSignaPFileAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "GEHC Signa PFile analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
