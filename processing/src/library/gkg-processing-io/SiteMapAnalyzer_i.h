#ifndef _gkg_processing_io_SiteMapAnalyzer_i_h_
#define _gkg_processing_io_SiteMapAnalyzer_i_h_


#include <gkg-processing-io/SiteMapAnalyzer.h>
#include <gkg-processing-io/SiteMapDiskFormat_i.h>


template < class R, class S, class Compare >
inline
gkg::SiteMapAnalyzer< R, S, Compare >::SiteMapAnalyzer()
                                      : gkg::BaseDiskFormatAnalyzer()
{
}


template < class R, class S, class Compare >
inline
gkg::SiteMapAnalyzer< R, S, Compare >::~SiteMapAnalyzer()
{
}


template < class R, class S, class Compare >
inline
gkg::DiskFormat& gkg::SiteMapAnalyzer< R, S, Compare >::getDiskFormat() const
{

  return gkg::SiteMapDiskFormat< R, S, Compare >::getInstance();

}


template < class R, class S, class Compare >
inline
void gkg::SiteMapAnalyzer< R, S, Compare >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::SiteMap< R, S, Compare > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );

    if ( gkg::TypeOf< S >::isSameType( itemType ) )
    {

      itemType = gkg::TypeOf< S >::getName();

    }
    analyzedObject.getHeader().addAttribute( "item_type", itemType );

    std::string rankType;
    object.getHeader().getAttribute( "rank_type", rankType );

    if ( gkg::TypeOf< R >::isSameType( rankType ) )
    {

      rankType = gkg::TypeOf< R >::getName();

    }
    analyzedObject.getHeader().addAttribute( "rank_type", rankType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMapAnalyzer< R, S, Compare >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
