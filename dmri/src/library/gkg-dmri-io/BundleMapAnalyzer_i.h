#ifndef _gkg_dmri_io_BundleMapAnalyzer_i_h_
#define _gkg_dmri_io_BundleMapAnalyzer_i_h_


#include <gkg-dmri-io/BundleMapAnalyzer.h>
#include <gkg-dmri-io/BundleMapDiskFormat_i.h>


template < class L, class Compare >
inline
gkg::BundleMapAnalyzer< L, Compare >::BundleMapAnalyzer()
                            : gkg::BaseDiskFormatAnalyzer()
{
}


template < class L, class Compare >
inline
gkg::BundleMapAnalyzer< L, Compare >::~BundleMapAnalyzer()
{
}


template < class L, class Compare >
inline
gkg::DiskFormat& gkg::BundleMapAnalyzer< L, Compare >::getDiskFormat() const
{

  return gkg::BundleMapDiskFormat< L, Compare >::getInstance();

}


template < class L, class Compare >
inline
void gkg::BundleMapAnalyzer< L, Compare >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::BundleMap< L, Compare > object;
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
             "void gkg::BundleMapAnalyzer< L, Compare >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
