#include <gkg-processing-plugin-dicom/DicomAnalyzer.h>
#include <gkg-processing-plugin-dicom/DicomDiskFormat.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


template < class T >
gkg::DicomAnalyzer< T >::DicomAnalyzer()
                        : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
gkg::DicomAnalyzer< T >::~DicomAnalyzer()
{
}


template < class T >
gkg::DiskFormat& gkg::DicomAnalyzer< T >::getDiskFormat() const
{

  return gkg::DicomDiskFormat< T >::getInstance();

}


template < class T >
void gkg::DicomAnalyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::Volume< T > object;
    object.getHeader().addAttribute( "fast_dicom_check", 1 );
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );
    analyzedObject.getHeader().addAttribute( "item_type", itemType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "void gkg::DicomAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


template class gkg::DicomAnalyzer< uint8_t >;
template class gkg::DicomAnalyzer< int8_t >;
template class gkg::DicomAnalyzer< uint16_t >;
template class gkg::DicomAnalyzer< int16_t >;
template class gkg::DicomAnalyzer< float >;


//
// registrating standard Dicom analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::DicomDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::DicomAnalyzer< uint8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::DicomDiskFormat< int8_t >::getInstance().getName(),
         &gkg::DicomAnalyzer< int8_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::DicomDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::DicomAnalyzer< uint16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::DicomDiskFormat< int16_t >::getInstance().getName(),
         &gkg::DicomAnalyzer< int16_t >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::DicomDiskFormat< float >::getInstance().getName(),
         &gkg::DicomAnalyzer< float >::getInstance() );

    return true;

  }
  GKG_CATCH( "DICOM analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
