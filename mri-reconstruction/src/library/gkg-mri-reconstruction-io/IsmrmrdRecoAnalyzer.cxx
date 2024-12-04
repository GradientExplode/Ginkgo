#include <gkg-mri-reconstruction-io/IsmrmrdRecoAnalyzer.h>
#include <gkg-mri-reconstruction-io/IsmrmrdRecoDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::IsmrmrdRecoAnalyzer< T >::IsmrmrdRecoAnalyzer()
                              : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::IsmrmrdRecoAnalyzer< T >::~IsmrmrdRecoAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::IsmrmrdRecoAnalyzer< T >::getDiskFormat() const
{

  try
  {

    return gkg::IsmrmrdRecoDiskFormat< T >::getInstance();

  }
  GKG_CATCH( "template < class T > "
             "gkg::DiskFormat& gkg::IsmrmrdRecoAnalyzer< T >::getDiskFormat() "
             "const" );

}


template < class T >
void gkg::IsmrmrdRecoAnalyzer< T >::analyze(
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
  GKG_CATCH( "void gkg::IsmrmrdRecoAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::IsmrmrdRecoAnalyzer< uint16_t >;
template class gkg::IsmrmrdRecoAnalyzer< int16_t >;
template class gkg::IsmrmrdRecoAnalyzer< uint32_t >;
template class gkg::IsmrmrdRecoAnalyzer< int32_t >;
template class gkg::IsmrmrdRecoAnalyzer< float >;
template class gkg::IsmrmrdRecoAnalyzer< double >;


//
// registrating standard Ismrmrd Reco analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRecoDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRecoDiskFormat< int16_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRecoDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoAnalyzer< uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRecoDiskFormat< int32_t >::getInstance().getName(),
         &gkg::IsmrmrdRecoAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRecoDiskFormat< float >::getInstance().getName(),
         &gkg::IsmrmrdRecoAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::IsmrmrdRecoDiskFormat< double >::getInstance().getName(),
         &gkg::IsmrmrdRecoAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Ismrmrd Reco analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
