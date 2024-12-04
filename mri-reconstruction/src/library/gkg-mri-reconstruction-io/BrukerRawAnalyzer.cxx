#include <gkg-mri-reconstruction-io/BrukerRawAnalyzer.h>
#include <gkg-mri-reconstruction-io/BrukerRawDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::BrukerRawAnalyzer< T >::BrukerRawAnalyzer()
                            : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::BrukerRawAnalyzer< T >::~BrukerRawAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::BrukerRawAnalyzer< T >::getDiskFormat() const
{

  return gkg::BrukerRawDiskFormat< T >::getInstance();

}


template < class T >
void gkg::BrukerRawAnalyzer< T >::analyze(
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
  GKG_CATCH( "void gkg::BrukerRawAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::BrukerRawAnalyzer< int16_t >;
template class gkg::BrukerRawAnalyzer< float >;
template class gkg::BrukerRawAnalyzer< double >;


//
// registrating standard Bruker Raw analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BrukerRawDiskFormat< float >::getInstance().getName(),
         &gkg::BrukerRawAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BrukerRawDiskFormat< double >::getInstance().getName(),
         &gkg::BrukerRawAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Bruker Raw analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
