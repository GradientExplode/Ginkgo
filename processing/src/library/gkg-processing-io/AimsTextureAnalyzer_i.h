#ifndef _gkg_processing_io_AimsTextureAnalyzer_i_h_
#define _gkg_processing_io_AimsTextureAnalyzer_i_h_


#include <gkg-processing-io/AimsTextureAnalyzer.h>
#include <gkg-processing-io/AimsTextureDiskFormat_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::AimsTextureAnalyzer< T >::AimsTextureAnalyzer()
                              : gkg::BaseDiskFormatAnalyzer()
{
}


template < class T >
inline
gkg::AimsTextureAnalyzer< T >::~AimsTextureAnalyzer()
{
}


template < class T >
inline
gkg::DiskFormat& gkg::AimsTextureAnalyzer< T >::getDiskFormat() const
{

  return gkg::AimsTextureDiskFormat< T >::getInstance();

}


template < class T >
inline
void gkg::AimsTextureAnalyzer< T >::analyze(
                                     const std::string& name,
                                     std::string& format,
                                     gkg::AnalyzedObject& analyzedObject ) const
{

  try
  {

    gkg::TextureMap< T > object;
    getDiskFormat().readHeader( name, object );


    std::string objectType = "TextureMap";
    if ( object.getHeader().hasAttribute( "object_type" ) )
    {

      object.getHeader().getAttribute( "object_type", objectType );
      if ( objectType == "Texture" )
      {

        objectType = "TextureMap";

      }

    }
    analyzedObject.getHeader().addAttribute( "object_type", objectType );

    std::string itemType;
    object.getHeader().getAttribute( "item_type", itemType );

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
             "void gkg::AimsTextureAnalyzer< T >::analyze( "
             "const std::string& name, "
             "std::string& format, "
             "gkg::AnalyzedObject& analyzedObject ) const" );

}


#endif
