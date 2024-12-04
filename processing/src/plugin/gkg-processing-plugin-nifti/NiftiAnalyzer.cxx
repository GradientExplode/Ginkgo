#include <gkg-processing-plugin-nifti/NiftiAnalyzer.h>
#include <gkg-processing-plugin-nifti/NiftiDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::NiftiAnalyzer< T >::NiftiAnalyzer()
                        : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::NiftiAnalyzer< T >::~NiftiAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::NiftiAnalyzer< T >::getDiskFormat() const
{

  return gkg::NiftiDiskFormat< T >::getInstance();

}


template < class T >
void gkg::NiftiAnalyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::Volume< T > object;
    object.getHeader().addAttribute( "fast_nifti_check", 1 );
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );
    analyzedObject.getHeader().addAttribute( "item_type", itemType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::NiftiAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::NiftiAnalyzer< int8_t >;
template class gkg::NiftiAnalyzer< uint8_t >;
template class gkg::NiftiAnalyzer< int16_t >;
template class gkg::NiftiAnalyzer< uint16_t >;
template class gkg::NiftiAnalyzer< int32_t >;
template class gkg::NiftiAnalyzer< uint32_t >;
template class gkg::NiftiAnalyzer< float >;
template class gkg::NiftiAnalyzer< double >;
template class gkg::NiftiAnalyzer< gkg::RGBComponent >;


//
// registrating standard Nifti analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< int8_t >::getInstance().getName(),
         &gkg::NiftiAnalyzer< int8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::NiftiAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< int16_t >::getInstance().getName(),
         &gkg::NiftiAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::NiftiAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< int32_t >::getInstance().getName(),
         &gkg::NiftiAnalyzer< int32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::NiftiAnalyzer< uint32_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< float >::getInstance().getName(),
         &gkg::NiftiAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< double >::getInstance().getName(),
         &gkg::NiftiAnalyzer< double >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::NiftiDiskFormat< gkg::RGBComponent >::getInstance().getName(),
         &gkg::NiftiAnalyzer< gkg::RGBComponent >::getInstance() );

    return true;

  }
  GKG_CATCH( "NIFTI analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
