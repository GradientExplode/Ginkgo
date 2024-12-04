#ifndef _gkg_processing_io_MeshMapAnalyzer_i_h_
#define _gkg_processing_io_MeshMapAnalyzer_i_h_

#include <gkg-processing-io/MeshMapAnalyzer.h>
#include <gkg-processing-io/MeshMapDiskFormat_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMapAnalyzer< R, S, D, Compare >::MeshMapAnalyzer()
                                         : gkg::BaseDiskFormatAnalyzer()
{
}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMapAnalyzer< R, S, D, Compare >::~MeshMapAnalyzer()
{
}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::DiskFormat& gkg::MeshMapAnalyzer< R, S, D, Compare >::getDiskFormat() const
{

  return gkg::MeshMapDiskFormat< R, S, D, Compare >::getInstance();

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMapAnalyzer< R, S, D, Compare >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::MeshMap< R, S, D, Compare > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    analyzedObject.getHeader().addAttribute( "item_type",
                                             gkg::TypeOf< S >::getName() );
    analyzedObject.getHeader().addAttribute( "rank_type", 
                                             gkg::TypeOf< R >::getName() );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "void gkg::MeshMapAnalyzer< R, S, D, Compare >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
