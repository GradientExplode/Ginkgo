#ifndef _gkg_processing_io_TextureMapAnalyzer_i_h_
#define _gkg_processing_io_TextureMapAnalyzer_i_h_


#include <gkg-processing-io/TextureMapAnalyzer.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <boost/algorithm/string.hpp>
 

template < class T >
inline
gkg::TextureMapAnalyzer< T >::TextureMapAnalyzer()
                             : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
inline
gkg::TextureMapAnalyzer< T >::~TextureMapAnalyzer()
{
}


template < class T >
inline
gkg::DiskFormat& gkg::TextureMapAnalyzer< T >::getDiskFormat() const
{

  return gkg::TextureMapDiskFormat< T >::getInstance();

}


template < class T >
inline
void gkg::TextureMapAnalyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::TextureMap< T > object;
    getDiskFormat().readHeader( name, object );

    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType ); 
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );

    // managing the fact that old files could contain ptk_ instead of gkg_
    boost::replace_all( itemType, "ptk", "gkg");


    if ( gkg::TypeOf< T >::isSameType( itemType ) )
    {

      itemType = gkg::TypeOf< T >::getName();
      analyzedObject.getHeader().addAttribute( "item_type",
                                             gkg::TypeOf< T >::getName() );

    }
    else
    {

      throw std::runtime_error( "bad item type" );

    }

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMapAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
