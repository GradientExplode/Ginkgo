#ifndef _gkg_dmri_io_BundleMeasureSpreadSheetAnalyzer_i_h_
#define _gkg_dmri_io_BundleMeasureSpreadSheetAnalyzer_i_h_


#include <gkg-dmri-io/BundleMeasureSpreadSheetAnalyzer.h>
#include <gkg-dmri-io/BundleMeasureSpreadSheetDiskFormat_i.h>


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheetAnalyzer< L, Compare >::
                                              BundleMeasureSpreadSheetAnalyzer()
                                                 : gkg::BaseDiskFormatAnalyzer()
{
}


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheetAnalyzer< L, Compare >::
                                             ~BundleMeasureSpreadSheetAnalyzer()
{
}


template < class L, class Compare >
inline
gkg::DiskFormat& 
gkg::BundleMeasureSpreadSheetAnalyzer< L, Compare >::getDiskFormat() const
{

  return gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::getInstance();

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheetAnalyzer< L, Compare >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::BundleMeasureSpreadSheet< L, Compare > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string labelType;
    object.getHeader().getAttribute( "label_type", labelType );

    if ( gkg::TypeOf< L >::isSameType( labelType ) )
    {

      labelType = gkg::TypeOf< L >::getName();

    }
    analyzedObject.getHeader().addAttribute( "label_type", labelType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheetAnalyzer< L, Compare >::"
             "analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
