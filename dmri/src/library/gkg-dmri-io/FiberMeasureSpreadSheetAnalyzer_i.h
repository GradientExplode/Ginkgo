#ifndef _gkg_dmri_io_FiberMeasureSpreadSheetAnalyzer_i_h_
#define _gkg_dmri_io_FiberMeasureSpreadSheetAnalyzer_i_h_


#include <gkg-dmri-io/FiberMeasureSpreadSheetAnalyzer.h>
#include <gkg-dmri-io/FiberMeasureSpreadSheetDiskFormat_i.h>


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheetAnalyzer< L, Compare >::
                                               FiberMeasureSpreadSheetAnalyzer()
                                                 : gkg::BaseDiskFormatAnalyzer()
{
}


template < class L, class Compare >
inline
gkg::FiberMeasureSpreadSheetAnalyzer< L, Compare >::
                                              ~FiberMeasureSpreadSheetAnalyzer()
{
}


template < class L, class Compare >
inline
gkg::DiskFormat& 
gkg::FiberMeasureSpreadSheetAnalyzer< L, Compare >::getDiskFormat() const
{

  return gkg::FiberMeasureSpreadSheetDiskFormat< L, Compare >::getInstance();

}


template < class L, class Compare >
inline
void gkg::FiberMeasureSpreadSheetAnalyzer< L, Compare >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::FiberMeasureSpreadSheet< L, Compare > object;
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
             "void gkg::FiberMeasureSpreadSheetAnalyzer< L, Compare >::"
             "analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
