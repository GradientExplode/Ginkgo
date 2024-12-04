#ifndef _gkg_processing_io_GisAnalyzer_i_h_
#define _gkg_processing_io_GisAnalyzer_i_h_


#include <gkg-processing-io/GisAnalyzer.h>
#include <gkg-processing-io/GisDiskFormat_i.h>


template < class T >
inline
gkg::GisAnalyzer< T >::GisAnalyzer()
                      : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
inline
gkg::GisAnalyzer< T >::~GisAnalyzer()
{
}


template < class T >
inline
gkg::DiskFormat& gkg::GisAnalyzer< T >::getDiskFormat() const
{

  return gkg::GisDiskFormat< T >::getInstance();

}


template < class T >
inline
void gkg::GisAnalyzer< T >::analyze( const std::string& name,
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

    if ( gkg::TypeOf< T >::isSameType( itemType ) )
    {

      itemType = gkg::TypeOf< T >::getName();

    }

    analyzedObject.getHeader().addAttribute( "item_type", itemType );

    format = getDiskFormat().getName();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::GisAnalyzer< T >::analyze( const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
