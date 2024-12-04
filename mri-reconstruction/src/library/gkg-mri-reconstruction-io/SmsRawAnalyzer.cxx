#include <gkg-mri-reconstruction-io/SmsRawAnalyzer.h>
#include <gkg-mri-reconstruction-io/SmsRawDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::SmsRawAnalyzer< T >::SmsRawAnalyzer()
                                 : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::SmsRawAnalyzer< T >::~SmsRawAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::SmsRawAnalyzer< T >::getDiskFormat() const
{

  return gkg::SmsRawDiskFormat< T >::getInstance();

}


template < class T >
void gkg::SmsRawAnalyzer< T >::analyze(
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
  GKG_CATCH( "void gkg::SmsRawAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::SmsRawAnalyzer< float >;
template class gkg::SmsRawAnalyzer< double >;


//
// registrating standard SMS Raw analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SmsRawDiskFormat< float >::getInstance().getName(),
         &gkg::SmsRawAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::SmsRawDiskFormat< double >::getInstance().getName(),
         &gkg::SmsRawAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "SMS Raw analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
