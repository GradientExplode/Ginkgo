#ifndef _gkg_processing_io_Curve3dMeshAnalyzer_i_h_
#define _gkg_processing_io_Curve3dMeshAnalyzer_i_h_


#include <gkg-processing-io/Curve3dMeshAnalyzer.h>
#include <gkg-processing-io/Curve3dMeshDiskFormat_i.h>


template < class T >
inline
gkg::Curve3dMeshAnalyzer< T >::Curve3dMeshAnalyzer()
                              : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
inline
gkg::Curve3dMeshAnalyzer< T >::~Curve3dMeshAnalyzer()
{
}


template < class T >
inline
gkg::DiskFormat& gkg::Curve3dMeshAnalyzer< T >::getDiskFormat() const
{

  return gkg::Curve3dMeshDiskFormat< T >::getInstance();

}


template < class T >
inline
void gkg::Curve3dMeshAnalyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::Curve3d< T > object;
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
             "void gkg::Curve3dMeshAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
