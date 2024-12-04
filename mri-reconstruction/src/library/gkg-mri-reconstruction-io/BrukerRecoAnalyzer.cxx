#include <gkg-mri-reconstruction-io/BrukerRecoAnalyzer.h>
#include <gkg-mri-reconstruction-io/BrukerRecoDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::BrukerRecoAnalyzer< T >::BrukerRecoAnalyzer()
                            : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::BrukerRecoAnalyzer< T >::~BrukerRecoAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::BrukerRecoAnalyzer< T >::getDiskFormat() const
{

  return gkg::BrukerRecoDiskFormat< T >::getInstance();

}


template < class T >
void gkg::BrukerRecoAnalyzer< T >::analyze(
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
  GKG_CATCH( "void gkg::BrukerRecoAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::BrukerRecoAnalyzer< uint8_t >;
template class gkg::BrukerRecoAnalyzer< int16_t >;
template class gkg::BrukerRecoAnalyzer< int32_t >;
template class gkg::BrukerRecoAnalyzer< float >;


//
// registrating standard Bruker Raw analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BrukerRecoDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::BrukerRecoAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BrukerRecoDiskFormat< int16_t >::getInstance().getName(),
         &gkg::BrukerRecoAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BrukerRecoDiskFormat< int32_t >::getInstance().getName(),
         &gkg::BrukerRecoAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::BrukerRecoDiskFormat< float >::getInstance().getName(),
         &gkg::BrukerRecoAnalyzer< float >::getInstance() );

    return true;

  }
  GKG_CATCH( "Bruker Reco analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
