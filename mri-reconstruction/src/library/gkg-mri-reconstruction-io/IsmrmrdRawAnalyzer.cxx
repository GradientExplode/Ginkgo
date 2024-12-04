#include <gkg-mri-reconstruction-io/IsmrmrdRawAnalyzer.h>
#include <gkg-mri-reconstruction-io/IsmrmrdRawDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::IsmrmrdRawAnalyzer< T >::IsmrmrdRawAnalyzer()
                             : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::IsmrmrdRawAnalyzer< T >::~IsmrmrdRawAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::IsmrmrdRawAnalyzer< T >::getDiskFormat() const
{

  try
  {

    return gkg::IsmrmrdRawDiskFormat< T >::getInstance();

  }
  GKG_CATCH( "template < class T > "
             "gkg::DiskFormat& gkg::IsmrmrdRawAnalyzer< T >::getDiskFormat() "
             "const" );

}


template < class T >
void gkg::IsmrmrdRawAnalyzer< T >::analyze(
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
  GKG_CATCH( "void gkg::IsmrmrdRawAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::IsmrmrdRawAnalyzer< float >;
template class gkg::IsmrmrdRawAnalyzer< double >;


//
// registrating standard Ismrmrd Raw analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRawDiskFormat< float >::getInstance().getName(),
         &gkg::IsmrmrdRawAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRawDiskFormat< double >::getInstance().getName(),
         &gkg::IsmrmrdRawAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Ismrmrd Raw analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
